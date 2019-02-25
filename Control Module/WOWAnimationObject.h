#ifndef WOWAnimationObject_h
#define WOWAnimationObject_h
#include "Arduino.h"
extern "C" {
  #include <espnow.h>
}

class WOWAnimationObject
{
  public:
    WOWAnimationObject();
    void init(byte rows_y, byte cols_x);
    void startTimer(unsigned long durationInMillis);
    byte hasTimedOut();
    void startAckTimeout();
    byte hasAckTimedOut();
    int getWave(float sinCounter, float minWave, float maxWave);
    void clearDataBlock();
    void clearESPNowDataBlock();
    void transmit();
    void transmitImediate();
    
    void clearBitmap();
    void renderLEDs();
    void subtractiveFade(byte fadeValue);
    void drawPixel(byte xPos,byte yPos, byte* colObject);
    void drawHLine(byte x, byte y, byte lineLength, byte *pixelColour);
    void drawVLine(byte x, byte y, byte lineLength, byte *pixelColour);
    void drawLine(short int x0, short int y0, short int x1, short int y1, byte *pixelColour);
    void drawCCircle(short int x0, short int y0, short int radius, byte *pixelColour);
    void drawPolly(short int cX, short int cY, short int radius, byte *pixelColour, short int rotationAngle, byte points);
    void fillArea(byte xStart, byte xEnd, byte yStart, byte yEnd, byte *pixelColour);
    void rangedSubtractiveFade(byte fadeLevel, byte leftBound, byte rightBound, byte topBound, byte bottomBound);
    void shiftDown(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound);
    void shiftUp(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound);
    void shiftLeft(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound);
    void shiftRight(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound);
    
    byte rows;
    byte cols;
    byte tempColour[3];
    unsigned long timeData[3];
    byte blockSize;
    byte ESPNOWBlockSize;
    byte stackIndex;
    byte ack;
    byte remoteDevice[6];
    byte* dataBlock;
    byte* espNowDataBlock;
    
    private:
    unsigned long _ackTimer[3];
};

#endif
