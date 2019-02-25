#include "colourObject.h"
#include <stdlib.h> 
#include <math.h>

colourObject::colourObject()
{

}

colourObject::colourObject(char maxValue)
{
  _colourBlockCount = 6;
  _primaryColours = new char*[_colourBlockCount];
  for(_cnt=0; _cnt<_colourBlockCount; _cnt++)
  {
    _primaryColours[_cnt] = new char[3];
  }
  for(_cnt=0; _cnt<3; _cnt++)
  {
    _colourTable[_cnt][0] = 0;
    _colourTable[_cnt][1] = 0;
    _colourTable[_cnt][2] = 0;
  }
  for(_cnt=0; _cnt<2; _cnt++)
  {
    _modifierTable[_cnt][0] = 0;
    _modifierTable[_cnt][1] = 0;
    _modifierTable[_cnt][2] = 0;
  }    
  _coloursPerBlock = maxValue;
  _bandWidth = _coloursPerBlock*_colourBlockCount;
  //Fill out default primary colours
  //[[255,0,0],[255,255,0],[0,255,0],[0,255,255],[0,0,255],[255,0,255]]
  _primaryColours[0][0] = maxValue;
  _primaryColours[0][1] = 0;
  _primaryColours[0][2] = 0;
  
  _primaryColours[1][0] = maxValue;
  _primaryColours[1][1] = maxValue;
  _primaryColours[1][2] = 0;
  
  _primaryColours[2][0] = 0;
  _primaryColours[2][1] = maxValue;
  _primaryColours[2][2] = 0;
  
  _primaryColours[3][0] = 0;
  _primaryColours[3][1] = maxValue;
  _primaryColours[3][2] = maxValue;
  
  _primaryColours[4][0] = 0;
  _primaryColours[4][1] = 0;
  _primaryColours[4][2] = maxValue;
  
  _primaryColours[5][0] = maxValue;
  _primaryColours[5][1] = 0;
  _primaryColours[5][2] = maxValue;
}

colourObject::colourObject(char maxValue, char colourBlockCount, char *rgbColourArray)
{
  _colourBlockCount = colourBlockCount;
  _primaryColours = new char*[_colourBlockCount];
  for(_cnt=0; _cnt<_colourBlockCount; _cnt++)
  {
    _primaryColours[_cnt] = new char[3];
  }
  //Init the Colour Table
  for(_cnt=0; _cnt<3; _cnt++)
  {
    _colourTable[_cnt][0] = 0;
    _colourTable[_cnt][1] = 0;
    _colourTable[_cnt][2] = 0;
  }
  //Init the Modofier Table
  for(_cnt=0; _cnt<2; _cnt++)
  {
    _modifierTable[_cnt][0] = 0;
    _modifierTable[_cnt][1] = 0;
    _modifierTable[_cnt][2] = 0;
  }
  //number of colours between 1 block and its neighbour    
  _coloursPerBlock = maxValue;
  //Total number of colours in this spectrum
  _bandWidth = _coloursPerBlock*_colourBlockCount;
  //Fill out primary colours based on array passed
  for(_cnt=0; _cnt<colourBlockCount; _cnt++)
  {
    _primaryColours[_cnt][0] = rgbColourArray[_cnt*3];
    _primaryColours[_cnt][1] = rgbColourArray[(_cnt*3)+1];
    _primaryColours[_cnt][2] = rgbColourArray[(_cnt*3)+2];
  }
}

void colourObject::getColour(short int colourIndex, byte *colourBlock)
{
  _primColIndex = colourIndex/_coloursPerBlock ;
  _colourTable[0][0] = _primaryColours[_primColIndex][0]; 
  _colourTable[0][1] = _primaryColours[_primColIndex][1];
  _colourTable[0][2] = _primaryColours[_primColIndex][2];
  
  _colourTable[1][0] = _primaryColours[(_primColIndex+1)%_colourBlockCount][0]; 
  _colourTable[1][1] = _primaryColours[(_primColIndex+1)%_colourBlockCount][1]; 
  _colourTable[1][2] = _primaryColours[(_primColIndex+1)%_colourBlockCount][2];

  gradientGenerator(colourIndex%_coloursPerBlock, _coloursPerBlock);
  
  colourBlock[0] = _colourTable[2][0];
  colourBlock[1] = _colourTable[2][1];
  colourBlock[2] = _colourTable[2][2];
}

void colourObject::gradientGenerator(unsigned short int colourIndex, unsigned short int bandWidth)
{
  for(_cnt=0; _cnt<3; _cnt++)
  {
    //fill modifier
    if(_colourTable[1][_cnt]>_colourTable[0][_cnt]) { _modifierTable[0][_cnt]=1; }
    else if(_colourTable[1][_cnt]<_colourTable[0][_cnt]) { _modifierTable[0][_cnt]=-1; }
    else if(_colourTable[1][_cnt]==_colourTable[0][_cnt]) { _modifierTable[0][_cnt]=0; }

    //fill step value
    if(_modifierTable[0][_cnt]==1)
    {
      _modifierTable[1][_cnt] = _colourTable[1][_cnt] - _colourTable[0][_cnt];
    }
    else if(_modifierTable[0][_cnt]==-1)
    {
      _modifierTable[1][_cnt] = _colourTable[0][_cnt] - _colourTable[1][_cnt];
    }
    else if(_modifierTable[0][_cnt]==0)
    {
      _modifierTable[1][_cnt] = 0;
    }
    //calculate current gradient between 2 based on the percentile index
    _colourTable[2][_cnt] = _colourTable[0][_cnt] + (((float)_modifierTable[1][_cnt]*((float)colourIndex/(float)bandWidth))*_modifierTable[0][_cnt]);
  }
}

