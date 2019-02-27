#ifndef envelopeGenerator_h
#define envelopeGenerator_h
#include "Arduino.h"

class envelopeGenerator
{
  public:
    envelopeGenerator();
    void initEnvelope(unsigned short int* points, unsigned short int* ticks, byte numberOfPoints); 
    unsigned short int getEnvelope(unsigned short int tickCounter);
    
    float** envelopePoints;
    byte numberOfEnvelopPoints;
    unsigned short int envelopeBandwidth;
    
    private:
    unsigned short int _counter, _envelopeIndex, _envelopeCounter;
    float _blockGradient;
};

#endif
