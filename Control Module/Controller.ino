/*
  Control module
  DRIVER MAC        84:F3:EB:B3:62:C7
  Controller MAC    84:F3:EB:B3:65:CE

*/
extern "C" {
  #include "user_interface.h"
  #include <espnow.h>
}
byte localDevice[] = {0x84,0xF3,0xEB,0xB3,0x65,0xCE};

void initVariant()
{
   wifi_set_opmode(STATIONAP_MODE);
   wifi_set_macaddr(STATION_IF, &localDevice[0]);
}
#include <ESP8266WiFi.h>
#include "colourObject.h"
#include "WOWAnimationObject.h"

#define WIFI_CHANNEL 1

const byte rows = 17, cols = 19;
const byte maxValue = 64;
byte tempColour[3] = {0,0,0};
char colourList[9*3]={maxValue,0,0, maxValue,maxValue,0, 0,maxValue,0, 0,maxValue,maxValue, 0,0,maxValue, maxValue,0,maxValue, maxValue,maxValue,maxValue, maxValue,0,0, 0,0,0};
colourObject dynColObject(maxValue, 9, colourList);
float waveIndex = 0;


WOWAnimationObject animationSystem;

void setup()
{
  animationSystem.init(rows, cols);
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  esp_now_init();
  delay(10);
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_add_peer(animationSystem.remoteDevice, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, NULL, 0);
  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len)
  {
    animationSystem.ack=1;
  });
}

void loop()
{   
    animationSystem.startTimer(500000);
    rainbowSwipe(2);
}

void rainbowSwipe(byte colourIncrement)
{
  unsigned short int scanCnt = 0, cIndex=0;

  animationSystem.clearBitmap();
  
  while(true)
  {
    if(animationSystem.hasTimedOut()){return;}
    
    dynColObject.getColour(cIndex%dynColObject._bandWidth, tempColour);
    animationSystem.drawVLine(scanCnt%animationSystem.cols, 0, animationSystem.rows, tempColour);
    animationSystem.renderLEDs();
    cIndex += colourIncrement;
    scanCnt++;
    animationSystem.subtractiveFade(5);
    delay(50); 
  }
}
