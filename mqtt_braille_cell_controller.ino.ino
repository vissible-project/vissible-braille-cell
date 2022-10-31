#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "BrailleCell.h"

const int serialData = D1;   // Arduino 5 to Pin 14; SER (Serial Input)
const int latchClock = D2;   // Arduino 6 to Pin 12, RCLK (Storage/latch)
const int shiftClock = D5;   // Arduino 7 to Pin 11; SRCLK (Shift Clock)
const int clearPin = D4;   // Arduino 8 to Pin 10; SRCLR (Clear/Reset)

WiFiClient espClient;
PubSubClient client(espClient);
char mqtt_server_location[50];
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

void configureWiFi() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.
  
  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
//  wm.resetSettings();

  WiFiManagerParameter mqtt_location("mqtt_location", "MQTT Location", "192.168.166.223", 50); 
  wm.addParameter(&mqtt_location);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "VISSibleAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("VISSibleAP","v1551bility"); // password protected ap

  if(!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("connected...yeey :)");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
//    mqtt_server_location = mqtt_location.getValue();
//    Serial.println(mqtt_server_location);
    const char* received_location = mqtt_location.getValue();
    strcpy(mqtt_server_location, received_location);
  }
}

void sendToBrailleCell(int value) {
  digitalWrite(latchClock, LOW);  // So that the outputs do not change as we shift out
  shiftOut(serialData, shiftClock, LSBFIRST, value);
  digitalWrite(latchClock, HIGH); // Update the outputs
}

void displayBrailleCell(BrailleChar brailleChar) {
  sendToBrailleCell(brailleChar.getValue());
  Serial.print("BrailleChar::displaying::");
  Serial.print(brailleChar.getName());
  Serial.print("::duration::");
  Serial.println(brailleChar.getDuration());
  delay(brailleChar.getDuration());
  
  sendToBrailleCell(blankBrailleChar.getValue());
  delay(blankBrailleChar.getDuration());
  snprintf (msg, MSG_BUFFER_SIZE, "%s", brailleChar.getName());
//  Serial.print("Publish message: ");
//  Serial.println(msg);
  client.publish("vissible/char/resp", msg);
}

// https://stackoverflow.com/a/34055805/7450617
void append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

void receivedMqttMessage(char* topic, byte* payload, unsigned int length) {
  char received_message[10] = {'\0'};
  for (int i = 0; i < length; i++) {
    char tmp_char = (char)payload[i];
    Serial.println(tmp_char);
    append(received_message, tmp_char);
  }
  strcat(received_message, "\0");

//  Serial.print("received::");
//  Serial.print(topic);
//  Serial.print("::");
//  Serial.println(received_message);
  
  for(int i = 0; i < 26; i++) {
    const char* this_name = brailleChars[i].getName();

    if(strcmp(this_name, received_message) == 0) {
//      Serial.print("Found::");
//      Serial.print(received_message);
//      Serial.print("::");
//      Serial.println(this_name);

      displayBrailleCell(brailleChars[i]);
    }
  }
}

void reconnectToMqtt() {
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "VISSibleClient-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("vissible/char/resp", "connected");
      // ... and resubscribe
      client.subscribe("vissible/char/show");
    } else {
      Serial.println(mqtt_server_location);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");      
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115100);
  
  configureWiFi();
  Serial.print("MQTT STARTING AT ");
  Serial.println(mqtt_server_location);
  client.setServer(mqtt_server_location, 1883);
  client.setCallback(receivedMqttMessage);
  
  pinMode(clearPin, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(latchClock, OUTPUT);
  pinMode(serialData, OUTPUT);  
  
  digitalWrite(clearPin, LOW);    // Clear the shift register
  digitalWrite(clearPin, HIGH);   // Hold the state of the shift register
  sendToBrailleCell(blankBrailleChar.getValue());
}

void loop()
{ 
  if (!client.connected()) {
    reconnectToMqtt();
  }
  client.loop();
}
