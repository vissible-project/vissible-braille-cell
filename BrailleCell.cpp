#include "BrailleCell.h"
#include <cstring>

BrailleChar::BrailleChar(char* name, int value, int duration) {
 strcpy(this->name, name);
 this->value = value;
 this->duration = duration;
}
    
char* BrailleChar::getName() {
  return this->name;
}

int BrailleChar::getValue() {
  return this->value;
}

int BrailleChar::getDuration() {
  return this->duration;
}

BrailleChar brailleChars[] = {
  BrailleChar("a", 0B00000001),
  BrailleChar("b", 0B00000011),
  BrailleChar("c", 0B00001001),
  BrailleChar("d", 0B00011001),
  BrailleChar("e", 0B00010001),
  BrailleChar("f", 0B00001011),
  BrailleChar("g", 0B00011011),
  BrailleChar("h", 0B00010011),
  BrailleChar("i", 0B00001010),
  BrailleChar("j", 0B00011010),
  BrailleChar("k", 0B00000101),
  BrailleChar("l", 0B00000111),
  BrailleChar("m", 0B00001101),
  BrailleChar("n", 0B00011101),
  BrailleChar("o", 0B00010101),
  BrailleChar("p", 0B00001111),
  BrailleChar("q", 0B00011111),
  BrailleChar("r", 0B00010111),
  BrailleChar("s", 0B00001110),
  BrailleChar("t", 0B00011110),
  BrailleChar("u", 0B00100101),
  BrailleChar("v", 0B00100111),
  BrailleChar("w", 0B00111010),
  BrailleChar("x", 0B00101101),
  BrailleChar("y", 0B00111101),
  BrailleChar("z", 0B00110101)
};

BrailleChar blankBrailleChar("blank", 0B00000000, 1000);
