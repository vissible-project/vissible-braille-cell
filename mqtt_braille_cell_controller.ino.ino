#include "BrailleCell.h"

const int serialData = D1;   // Arduino 5 to Pin 14; SER (Serial Input)
const int latchClock = D2;   // Arduino 6 to Pin 12, RCLK (Storage/latch)
const int shiftClock = D5;   // Arduino 7 to Pin 11; SRCLK (Shift Clock)
const int clearPin = D4;   // Arduino 8 to Pin 10; SRCLR (Clear/Reset)

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
}

void setup()
{
  pinMode(clearPin, OUTPUT);
  pinMode(shiftClock, OUTPUT);
  pinMode(latchClock, OUTPUT);
  pinMode(serialData, OUTPUT);  

  Serial.begin(115200);
  
  digitalWrite(clearPin, LOW);    // Clear the shift register
  digitalWrite(clearPin, HIGH);   // Hold the state of the shift register
}

void loop()
{ 
  displayBrailleCell(brailleChars[0]);
  displayBrailleCell(brailleChars[1]);
  displayBrailleCell(brailleChars[2]);
}
