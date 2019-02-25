#ifndef colourObject_h
#define colourObject_h
#include "Arduino.h"

class colourObject
{
  public:
    colourObject();
    colourObject(char maxValue);
    colourObject(char maxValue, char colourBlockCount, char *rgbColourArray);
    void getColour(short int colourIndex, byte *colourBlock);
    void gradientGenerator(unsigned short int colourIndex, unsigned short int bandwidth);
    
    char** _primaryColours;
    char _colourTable[3][3];
    short int _modifierTable[2][3];
    
    char _colourBlockCount;
    unsigned short int _coloursPerBlock;
    unsigned short int _bandWidth;
    char _cnt;
    unsigned short int _primColIndex;
    
    private:
};

#endif
