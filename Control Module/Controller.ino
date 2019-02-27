/*
  Control Module 
  This file will conatin all your real time animations
  
  Remote DRIVER MAC Address        84:F3:EB:B3:62:C7
  THIS Controllers MAC ADdress     84:F3:EB:B3:65:CE

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
#include "envelopeGenerator.h"
#define WIFI_CHANNEL 1

//Enter your pixel maps Width(cols) and Height(rows)
const byte rows = 17, cols = 19;
//maxValue sets the physical maximunm brightness of the colour system to limit power usage. 64 is 25% brightness. 255 is the maximum and NOT recomended
const byte maxValue = 64;
//tempColour stores an RGB colour value to be used in your system
byte tempColour[3] = {0,0,0};
/*
  The following sets up the colourObject for details of this API check https://github.com/leonyuhanov/colourObject
    -colourList is an array of RGB colours in your palete Below is a standard ROYGBIV rainbow width a trailing BLACK
    -you can create your own colourList array with as many or few colours
    -dynColObject is the colour object that will generate all your colours its initiated with:
      -maxValue setting the upper bounds of the RGB rainbow for power limitation
      -9 the number of UNIQUE RGB values in the colourList
      -colourList an array of RGB colours
*/
char colourList[9*3]={maxValue,0,0, maxValue,maxValue,0, 0,maxValue,0, 0,maxValue,maxValue, 0,0,maxValue, maxValue,0,maxValue, maxValue,maxValue,maxValue, maxValue,0,0, 0,0,0};
colourObject dynColObject(maxValue, 9, colourList);

//waveIndex is a frame tracking ticker for the getWave function
float waveIndex = 0;

//Create an instance of the WOWAnimationObject
WOWAnimationObject animationSystem;
envelopeGenerator envelopeOne;

void setup()
{
  Serial.begin(115200);
  Serial.printf("\r\n\r\n");
  //Initiate the animation system passing your pixel maps Width(cols) and Height(rows)
  animationSystem.init(rows, cols);
  //DO NOT CHANGE ANYTHING IN THE REST OF THE SET UP FUNCTION
  //Set up ESPNOW
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
  
  const byte numberOfPoints = 4;
  unsigned short int envelopePoints[numberOfPoints] = {0,10,20,5};
  unsigned short int ticks[numberOfPoints]={30,30,30,30};
  unsigned short int scanCnt = 0;
  envelopeOne.initEnvelope(envelopePoints, ticks, 4);
  
  while(true)
  {
    Serial.printf("\r\n%d", envelopeOne.getEnvelope(scanCnt));
    scanCnt = (scanCnt+1)%envelopeOne.envelopeBandwidth;
    delay(40);
  }
  
  /*
  //Start a timer for 500 seconds
  animationSystem.startTimer(500000);
  //Start your animation
  rainbowSwipe(2);
  */

}

void rainbowSwipe(byte colourIncrement)
{
  unsigned short int scanCnt = 0, cIndex=0;

  //Clear the animation FRAME
  animationSystem.clearBitmap();
  
  while(true)
  {
    //Check if the animation has timed out
  if(animationSystem.hasTimedOut()){return;}
    //Place a colour into "tempColour" using index "cIndex" from the dynColObject palete
    dynColObject.getColour(cIndex%dynColObject._bandWidth, tempColour);
    //Draw a Vertical line of "animationSystem.rows" height at location ("scanCnt%animationSystem.cols", 0) in your frame, with colour "tempColour"
  animationSystem.drawVLine(scanCnt%animationSystem.cols, 0, animationSystem.rows, tempColour);
    //Render your FRAME to the LEDs
  animationSystem.renderLEDs();
  //Increment the colour indexer "cIndex" by "colourIncrement"
    cIndex += colourIncrement;
  //Indrecment the X modifier fr the next loop by 1
    scanCnt++;
  //Fade your entire frame by a value of 5
    animationSystem.subtractiveFade(5);
  //Delay for 50ms
    delay(50); 
  }
}
