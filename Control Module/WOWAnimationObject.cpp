#include "WOWAnimationObject.h"
extern "C" {
  #include <espnow.h>
}

WOWAnimationObject::WOWAnimationObject()
{

}

void WOWAnimationObject::init(byte rows_y, byte cols_x)
{
    rows = rows_y;
    cols = cols_x;
    blockSize = 10;
    ack=1;
    remoteDevice[0] = 0x84;
    remoteDevice[1] = 0xF3;
    remoteDevice[2] = 0xEB;
    remoteDevice[3] = 0xB3;
    remoteDevice[4] = 0x62;
    remoteDevice[5] = 0xC7;
    dataBlock = new byte[blockSize];

    clearDataBlock();
    //Default timeout for an ESPNOW ACK response
    _ackTimer[2] = 2000; //default response is about 500max
}

void WOWAnimationObject::startTimer(unsigned long durationInMillis)
{
  timeData[0] = millis(); 
  timeData[2] = durationInMillis;
}
byte WOWAnimationObject::hasTimedOut()
{
  timeData[1] = millis();
  if(timeData[2] < timeData[1]-timeData[0])
  {
    return 1;
  }
  return 0;
}
void WOWAnimationObject::startAckTimeout()
{
  _ackTimer[0] = micros(); 
}
byte WOWAnimationObject::hasAckTimedOut()
{
  _ackTimer[1] = micros();
  if(_ackTimer[2] < _ackTimer[1]-_ackTimer[0])
  {
    return 1;
  }
  return 0;
}

void WOWAnimationObject::initEnvelope(unsigned short int* points, unsigned short int* ticks, byte numberOfPoints)
{
  //Init Envelops in memory
  numberOfEnvelopPoints = numberOfPoints;
  envelopePoints = new float*[numberOfEnvelopPoints];
  for(_counter=0; _counter<numberOfEnvelopPoints; _counter++)
  {
    envelopePoints[_counter] = new float[3];
  }
  _envelopeIndex=0;
  envelopeBandwidth=0;
  
  //set up Envelop Points
  for(_counter=0; _counter<numberOfEnvelopPoints; _counter++)
  {
    envelopePoints[_counter][0] = points[_counter];
    if(_counter+1==numberOfEnvelopPoints)
    {
      envelopePoints[_counter][1] = points[0];
    }
    else
    {
      envelopePoints[_counter][1] = points[_counter+1];
    }
    envelopePoints[_counter][2] = ticks[_counter];
    envelopeBandwidth += ticks[_counter];
  }
  //Reset the ticker to 0
  _envelopeIndex = 0;
}

int WOWAnimationObject::getWave(float sinCounter, float minWave, float maxWave)
{
  return abs( (sin(sinCounter)*(maxWave+1-minWave)))+minWave ;
}

unsigned short int WOWAnimationObject::getEnvelope(unsigned short int tickCounter)
{
  //grab block ID
  _envelopeCounter = envelopePoints[0][2];
  for(_counter=0; _counter<numberOfEnvelopPoints; _counter++)
  {
    if(tickCounter>=0 && tickCounter<_envelopeCounter)
    {
      _envelopeIndex = _counter;
      _counter=numberOfEnvelopPoints;
    }
    else if (tickCounter>=_envelopeCounter && tickCounter<_envelopeCounter+envelopePoints[_counter+1][2])
    {
      _envelopeIndex = _counter+1;
      _counter=numberOfEnvelopPoints;
    }
    else
    {
      _envelopeCounter += envelopePoints[_counter][2];
    }
  }

  //grab gradient
  if(_envelopeIndex==0)
  {
    _blockGradient = ((envelopePoints[_envelopeIndex][1] - envelopePoints[_envelopeIndex][0]) / envelopePoints[_envelopeIndex][2])*tickCounter;
    if(_blockGradient<0)
    {
      return envelopePoints[_envelopeIndex][0] - abs(_blockGradient);
    }
    else
    {
      return envelopePoints[_envelopeIndex][0] + abs(_blockGradient);
    }
  }
  else
  {
    _envelopeCounter=0;
    _counter=0;
    while(_counter<_envelopeIndex)
    {
      _envelopeCounter+=envelopePoints[_counter][2];
      _counter++;
    }
    
    _blockGradient = (((envelopePoints[_envelopeIndex][1] - envelopePoints[_envelopeIndex][0]) / envelopePoints[_envelopeIndex][2]) * (tickCounter-_envelopeCounter));
    if(_blockGradient<0)
    {
      return envelopePoints[_envelopeIndex][0] - abs(_blockGradient);
    }
    else
    {
      return envelopePoints[_envelopeIndex][0] + abs(_blockGradient);
    }
  }

}

void WOWAnimationObject::clearDataBlock()
{
  byte bIndex=0;
  for(bIndex=0; bIndex<blockSize; bIndex++)
  {
    dataBlock[bIndex]=0;
  }
}

void WOWAnimationObject::transmitImediate()
{
    startAckTimeout();
    esp_now_send(remoteDevice, dataBlock, blockSize);
    clearDataBlock();
    ack=0;
    while(!ack)
    {
      if(hasAckTimedOut())
      {
        ack=1;
      }
      yield();
    }
    
}

void WOWAnimationObject::clearBitmap()
{
  clearDataBlock();
  dataBlock[0] = 0;
  transmitImediate();
}
void WOWAnimationObject::renderLEDs()
{
  clearDataBlock();
  dataBlock[0] = 1;
  transmitImediate();  
}
void WOWAnimationObject::subtractiveFade(byte fadeValue)
{
  clearDataBlock();
  dataBlock[0] = 2;
  dataBlock[1] = fadeValue;
  transmitImediate();  
}
void WOWAnimationObject::drawPixel(byte xPos,byte yPos, byte* colObject)
{
  clearDataBlock();
  dataBlock[0] = 3;
  dataBlock[1] = xPos;
  dataBlock[2] = yPos;
  dataBlock[3] = colObject[0];
  dataBlock[4] = colObject[1];
  dataBlock[5] = colObject[2];
  transmitImediate();  
}
void WOWAnimationObject::drawHLine(byte x, byte y, byte lineLength, byte *pixelColour)
{
  clearDataBlock();
  dataBlock[0] = 4;
  dataBlock[1] = x;
  dataBlock[2] = y;
  dataBlock[3] = lineLength;
  dataBlock[4] = pixelColour[0];
  dataBlock[5] = pixelColour[1];
  dataBlock[6] = pixelColour[2]; 
  transmitImediate();  
}
void WOWAnimationObject::drawVLine(byte x, byte y, byte lineLength, byte *pixelColour)
{
  clearDataBlock();
  dataBlock[0] = 5;
  dataBlock[1] = x;
  dataBlock[2] = y;
  dataBlock[3] = lineLength;
  dataBlock[4] = pixelColour[0];
  dataBlock[5] = pixelColour[1];
  dataBlock[6] = pixelColour[2];  
  transmitImediate();  
}
void WOWAnimationObject::drawLine(short int x0, short int y0, short int x1, short int y1, byte *pixelColour)
{
  clearDataBlock();
  dataBlock[0] = 6;
  dataBlock[1] = x0;
  dataBlock[2] = y0;
  dataBlock[3] = x1;
  dataBlock[4] = y1;
  dataBlock[5] = pixelColour[0];
  dataBlock[6] = pixelColour[1];  
  dataBlock[7] = pixelColour[2];  
  transmitImediate();  
}
void WOWAnimationObject::drawCCircle(short int x0, short int y0, short int radius, byte *pixelColour)
{
  clearDataBlock();
  dataBlock[0] = 7;
  dataBlock[1] = x0;
  dataBlock[2] = y0;
  dataBlock[3] = radius;
  dataBlock[4] = pixelColour[0];
  dataBlock[5] = pixelColour[1];
  dataBlock[6] = pixelColour[2];  
  transmitImediate();  
}
void WOWAnimationObject::drawPolly(short int cX, short int cY, short int radius, byte *pixelColour, short int rotationAngle, byte points)
{  
  clearDataBlock();
  dataBlock[0] = 8;
  dataBlock[1] = cX;
  dataBlock[2] = cY;
  dataBlock[3] = radius;
  dataBlock[4] = (&rotationAngle)[1];
  dataBlock[5] = (&rotationAngle)[0];
  dataBlock[6] = points;
  dataBlock[7] = pixelColour[0];
  dataBlock[8] = pixelColour[1];  
  dataBlock[9] = pixelColour[2];  
  transmitImediate();  
}
void WOWAnimationObject::fillArea(byte xStart, byte xEnd, byte yStart, byte yEnd, byte *pixelColour)
{
  clearDataBlock();
  dataBlock[0] = 9;
  dataBlock[1] = xStart;
  dataBlock[2] = xEnd;
  dataBlock[3] = yStart;
  dataBlock[4] = yEnd;
  dataBlock[5] = pixelColour[0];
  dataBlock[6] = pixelColour[1];
  dataBlock[7] = pixelColour[2];  
  transmitImediate();    
}
void WOWAnimationObject::rangedSubtractiveFade(byte fadeLevel, byte leftBound, byte rightBound, byte topBound, byte bottomBound)
{
  clearDataBlock();
  dataBlock[0] = 10;
  dataBlock[1] = fadeLevel;
  dataBlock[2] = leftBound;
  dataBlock[3] = rightBound;
  dataBlock[4] = topBound;  
  dataBlock[5] = bottomBound;  
  transmitImediate();  
}
void WOWAnimationObject::shiftDown(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound)
{
  clearDataBlock();
  dataBlock[0] = 11;
  dataBlock[1] = wrap;
  dataBlock[2] = leftBound;
  dataBlock[3] = rightBound;
  dataBlock[4] = topBound;  
  dataBlock[5] = bottomBound;    
  transmitImediate();  
}
void WOWAnimationObject::shiftUp(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound)
{
  clearDataBlock();
  dataBlock[0] = 12;
  dataBlock[1] = wrap;
  dataBlock[2] = leftBound;
  dataBlock[3] = rightBound;
  dataBlock[4] = topBound;  
  dataBlock[5] = bottomBound;    
  transmitImediate();  
}
void WOWAnimationObject::shiftLeft(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound)
{
  clearDataBlock();
  dataBlock[0] = 13;
  dataBlock[1] = wrap;
  dataBlock[2] = leftBound;
  dataBlock[3] = rightBound;
  dataBlock[4] = topBound;  
  dataBlock[5] = bottomBound;    
  transmitImediate();  
}
void WOWAnimationObject::shiftRight(bool wrap, byte leftBound, byte rightBound, byte topBound, byte bottomBound)
{
  clearDataBlock();
  dataBlock[0] = 14;
  dataBlock[1] = wrap;
  dataBlock[2] = leftBound;
  dataBlock[3] = rightBound;
  dataBlock[4] = topBound;  
  dataBlock[5] = bottomBound;    
  transmitImediate();  
}
