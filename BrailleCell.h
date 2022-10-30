#ifndef BrailleCell_h
#define BrailleCell_h

const int LONG_DURATION = 500;

class BrailleChar {
  private:
    char name[10];
    int value;
    int duration;

  public:
    BrailleChar(char* name, int value, int duration = LONG_DURATION);

    char* getName();

    int getValue();

    int getDuration();
};



extern BrailleChar brailleChars[];

extern BrailleChar blankBrailleChar;

#endif
