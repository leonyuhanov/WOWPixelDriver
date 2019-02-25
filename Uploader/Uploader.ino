#include <ESP8266WiFi.h>
#include "FS.h"

const short int numLeds = 255;
const short int bytesPerLed = 4;
const short int rows = 17, cols = 19;
const short int maskMap[rows][cols] = {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,240,239,210,209,180,179,150,149,120,119,90,89,60,59,30,29,0,-1},{-1,241,238,211,208,181,178,151,148,121,118,91,88,61,58,31,28,1,-1},{-1,242,237,212,207,182,177,152,147,122,117,92,87,62,57,32,27,2,-1},{-1,243,236,213,206,183,176,153,146,123,116,93,86,63,56,33,26,3,-1},{-1,244,235,214,205,184,175,154,145,124,115,94,85,64,55,34,25,4,-1},{-1,245,234,215,204,185,174,155,144,125,114,95,84,65,54,35,24,5,-1},{-1,246,233,216,203,186,173,156,143,126,113,96,83,66,53,36,23,6,-1},{-1,247,232,217,202,187,172,157,142,127,112,97,82,67,52,37,22,7,-1},{-1,248,231,218,201,188,171,158,141,128,111,98,81,68,51,38,21,8,-1},{-1,249,230,219,200,189,170,159,140,129,110,99,80,69,50,39,20,9,-1},{-1,250,229,220,199,190,169,160,139,130,109,100,79,70,49,40,19,10,-1},{-1,251,228,221,198,191,168,161,138,131,108,101,78,71,48,41,18,11,-1},{-1,252,227,222,197,192,167,162,137,132,107,102,77,72,47,42,17,12,-1},{-1,253,226,223,196,193,166,163,136,133,106,103,76,73,46,43,16,13,-1},{-1,254,225,224,195,194,165,164,135,134,105,104,75,74,45,44,15,14,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
unsigned short int xCnt, yCnt;
short int fCounter=0, bytesWriten=0;

void setup()
{
  Serial.begin(115200);
  Serial.printf("\r\n\r\n\r\n");
  
  WiFi.mode(WIFI_OFF);

  Serial.printf("\r\n\tFormating SPIFFS...");
  SPIFFS.begin();
  Serial.printf("DONE!");   

}

void loop()
{
  const char* path="/configFile";
  
  File file = SPIFFS.open(path, "w");
  if(!file)
  {
      Serial.println("- failed to open file for writing");
      return;
  }
  Serial.printf("\r\n\tWriting Number of Pixels...");
  //Number of pixels
  bytesWriten += file.write((byte*)&numLeds, sizeof(numLeds));
  Serial.printf("\r\n\tWriting Pixel Type...");
  //bytes per LED for pixel type
  bytesWriten += file.write((byte*)&bytesPerLed, sizeof(bytesPerLed));
  Serial.printf("\r\n\tWriting Number of Rows...");
  //Rows
  bytesWriten += file.write((byte*)&rows, sizeof(rows));
  Serial.printf("\r\n\tWriting Number of Columns...");
  //Columns
  bytesWriten += file.write((byte*)&cols, sizeof(cols));
  Serial.printf("\r\n\tWriting Pixel Map...");
  //LED Map
  for(yCnt=0; yCnt<rows; yCnt++)
  {
    for(xCnt=0; xCnt<cols; xCnt++)
    {
      bytesWriten += file.write((byte*)&maskMap[yCnt][xCnt], sizeof(maskMap[yCnt][xCnt]));
    }
  }
  file.close();
  Serial.printf("DONE!");
  Serial.printf("\r\n\tWrote [%d] Bytes to SPIFFS", bytesWriten);
  Serial.printf("\r\n\tSystem Configuration has completed, please upload the driver binary.");
  delay(9000000);
}
