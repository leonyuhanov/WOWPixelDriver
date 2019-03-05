

# WOW Pixel Driver, LED Mapping & Animation System
A complete solution for pixel mapping, animation and driving for addressable LEDs

# Why?
The real-time programatic animation system used in all commerical products made by [Elec Dash Tron](https://www.instagram.com/wow_elec_tron/) is Private IP. I want to make the entire system accesible to the public without having to distrubute the source code publicly. I want to also restrict usage to specific hardware modules

# How does it work
The system consists of 2 physical, ESP8266 based, modules. The Driver Module (LABELED D) contains the pixel mapping, animation system and LED Driver. The Control module (LABELED C) is user programmable and runs your script of animations. The Control module comunicates via the ESP-NOW protocol for realtime execution of commands. 

# What pixels are supported
The driver uses the ESP8266 SPI port to drive both Clocked and Unclocked Chipsets:
* APA102 APA102C & variants from AP Corp NeonWorld
* SK9822
* WS2812 5050
* WS2812B 5050
* WS2812 2020
* SK6812 5050, 3535 & 2427(With Opaque Lense)(The CLEAR Lense model is unsupported)

# Some limitations
* The maximum pixel map size is 255x255 bytes
* I recomend the maximum amount of pixels to be under 1000 for best perfomance. There are A LOT of variables to consider here such as:
  * Power use
  * Your soldering Skils
  * Cable and conection types
  * Environmental noise
  * specific requirements for specific pixel chipsets
* The Control module uses the ESPNOW protocol to comunicate with the Driver module. I have tested this to be pretty good at about 50 meters line of sight with a clean RF environment. WHen you start introducing RF noise, walls, pockets, cases etc... your range wil be smaller
* A lot of the functions use BYTE (uint_8) for input vars. Please be mindfull of any overun maths you need to do for your code 

# Basic Workflow
 * Create a pixel map
 * Upload it to the Driver module
 * Upload the Driver Binary to the driver module
 * Create your animations and upload them to your Controller module

# Prerequisite Software
* The Arduino IDE via [Arduino](https://www.arduino.cc/)
  * With the ESP8266 Development Library, install instructions from [here](https://github.com/esp8266/Arduino)  
* The Espressif Flash Download Tools from  [Espressif](https://www.espressif.com/en/support/download/other-tools)
* A spreadhseet app like [Open Office](https://www.openoffice.org/) or [Libre Office](https://www.libreoffice.org/) Calc 

# PIN OUTs of the 4PIN JST MALE port on the DRIVER Module
  * Red: +5v
  * Yellow: Clock Pin (only active for APA102/SK9822 pixels) Level shifted to 5+ Logic Level via the [SN74HCT245N](http://www.ti.com/lit/ds/symlink/sn74hct245.pdf) 
  * Green: Data Pin (active for all pixels) Level shifted to 5+ Logic Level via the [SN74HCT245N](http://www.ti.com/lit/ds/symlink/sn74hct245.pdf) 
  * Blue: Ground
  
The Driver module will come with 2 FEMALE JST cables for you to use depending on the type of pixel you wish to use. Both conect to the same 4 Pin MALE conector
  * A 3pin cable for driving WS2812 & SK6812 Pixels
  * A 4pin cable for driving APA102 & SK9822 Pixels
  

# Setting up your pixel map
A template spreadsheet is provided [here](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Pixel%20Map%20Template.ods) This doc will focus on a simple matrix, but Im adding more complex examples into the file as we go

* The template is set up for a 17Px Wide X 15px Tall panel
* The pixels start at the top righ and are conected in a zig-zags configuration(down, left, up, left, down, left etc....)
* The panel has 255 pixels in total
* The pixel map uses "-1" as an indicator of a NULL pixel. A Null pixel is simply a space where a pixel doesnt exist. This becomes very obvious when your layout is complex
* The pixel map requires a buffer of these null pixels at the top, bottom, left and right sides of your pixel map. These buffer lines can be used as off-screen animation helpers
* Each cell inside your map MUST be filled in. A cell is iether a physical address/order of a pixel or "-1" menaing a blank space or null pixel. If you have ANY cells that are empty, the driver will NOT boot
* You can reuse a pixels physical address in  multiple cells, but note that any pixel render meant for that location will be rendered to each location mapped to that address. I can not see any instance where this is usefull, but its definitely something you CAN do 

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/pixelmap.jpg" width="900" />

The template spreadsheet concatenates each Column cell into a single C++ array representing that ROW. It then concatenates each ROW into a C++ array. Once done, COPY the text in the cell (marked in the above image as "Your Pixel Map array") this text is the full string you will need in the next section for the pixelMap array

# Setting up the configuration code uploader utility
The uploader, located he [Uploader.ino](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Uploader/Uploader.ino) contains all the config variable you need to fill out and is responsible for writing this to the Driver modules SPIFFS. Open the file in the Arduino IDE and fill out the following:

* cols: Number of Columns (X range)
* rows: Number of Rows (Y range)
* numLeds: Number of Pixels in your layout
* bytesPerLed: Set to 3 for Neopixels/SK6812/WS2812 or 4 for APA102/SK9822
* maxSPIFrequency: The Default SPI Clock Frequency in (Mhz/1000000) for Driving Clocked Pixels (default is 10 which is 10Mhz). If you are using Neopixels leave this set to 10.
  * In reality your pixel layout may require a much lower drive speed. When starting out set this to 1(for 1Mhz) and ramp it up as you go
* controllerMAC: The MAC Address of the Controller Modules "WIFI_STA"
  * You will be provided with the MAC address 
  * If you want ot use your own ESP8266 module you will need to identify its WIFI_STA address. Use [this code](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Utilities/GetControllerMAC.ino) it will print the MAC address to the serial console and halt
* driverMAC: The MAC Address of the Driver modules "WIFI_STA"
  * You will be provided with the MAC address 
* maskMap: The pixel map array you created in the template file, you can simply paste the text from the pixel map template

The Uploader code will look something like this for the example we will be working with:
```C++
//Number of Physical Pixels in your system
const short int numLeds = 255;
//bytesPerLed lets the system know if you are working with WS2812/SK6812 (3 Bytes) or APA102/SK9822 (4 Bytes) pixels 
const short int bytesPerLed = 4;
//rows =  is the total number of Rows(Y Axis) in your LED set up, this is usualy the number of pixels TALL+2(1 NUll line at the top and bottom)
//cols =  is the total number of Columns(X Axis) in your LED set up, this is usualy the number of pixels WIDE+2(1 NUll line at the LEFT and RIGHT)
const short int rows = 17, cols = 19;
//SPI Frequcny for clocked Pixels
const short int maxSPIFrequency = 10;
//MAC Address of Controller modules "WIFI_STA"
const short int controllerMACAddress[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
//This is the array you create in the template spreadsheet above
const short int maskMap[rows][cols] = {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,240,239,210,209,180,179,150,149,120,119,90,89,60,59,30,29,0,-1},
{-1,241,238,211,208,181,178,151,148,121,118,91,88,61,58,31,28,1,-1},
{-1,242,237,212,207,182,177,152,147,122,117,92,87,62,57,32,27,2,-1},
{-1,243,236,213,206,183,176,153,146,123,116,93,86,63,56,33,26,3,-1},
{-1,244,235,214,205,184,175,154,145,124,115,94,85,64,55,34,25,4,-1},
{-1,245,234,215,204,185,174,155,144,125,114,95,84,65,54,35,24,5,-1},
{-1,246,233,216,203,186,173,156,143,126,113,96,83,66,53,36,23,6,-1},
{-1,247,232,217,202,187,172,157,142,127,112,97,82,67,52,37,22,7,-1},
{-1,248,231,218,201,188,171,158,141,128,111,98,81,68,51,38,21,8,-1},
{-1,249,230,219,200,189,170,159,140,129,110,99,80,69,50,39,20,9,-1},
{-1,250,229,220,199,190,169,160,139,130,109,100,79,70,49,40,19,10,-1},
{-1,251,228,221,198,191,168,161,138,131,108,101,78,71,48,41,18,11,-1},
{-1,252,227,222,197,192,167,162,137,132,107,102,77,72,47,42,17,12,-1},
{-1,253,226,223,196,193,166,163,136,133,106,103,76,73,46,43,16,13,-1},
{-1,254,225,224,195,194,165,164,135,134,105,104,75,74,45,44,15,14,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
```
Once you have filled out all the details, check atht your IDE is set up for the corect module and settings. The most important things to set are:
* Board Type: LOLIN(WEMOS) D1 R2 & MINI
* CPU Frequency: 160Mhz
* Flash Size: 4M (3M SPIFFS)
* Erase Flash: ALL FLASH CONTENTS

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/UploadConfig.jpg" width="200" />

Connect your DRIVER module via a USB cable, Open the serial monitor & Upload your code.
Once complete you will see a message like this on the console...

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/UploaderOutput.jpg" width="500" />

# Uploading the driver Binary
* The driver Binary is the entire precompiled system that will be physicaly delivered with each physical driver module OR emailed to you upon purchase
* It is locked to the physical ESP Chip ID & MAC Address of the module you receive
* It will NOT work on any other module

Open the "Espressif Flash Download Tools" and select "ESP8266 Download Tool" your settings need to be as follows:
* Tick the 1st line and browse to the location of the BINARY file you receievd. On the RIGHT enter 0 as the location where this binary will be burned to
* Crystal Freq: 40M
* SPI SPeed: 80Mhz
* SPI Mode: QIO
* Flash Size: 32Mbit
* Tick "DO NOT CHANGE BINARY"
* Select the port your Driver mofule is conected to and set upload speed to 921600

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/ESPTool%20Settings_esp8266.jpg" width="600" />

Click START to Upload the Binary, when complete, disconect the Driver Module from your PC, conect it to the panel and power it on. You will see a test patern if you have done everything corect. The test patern scans a singe line in RGB in alternate directions to indicate a complete map. Conecting the Driver module to your USB Port and opening the Console will help troubleshoot any issues. This is an exmaple of what you will see on the console if everythign is set up corectly:

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/ExampleOutput.jpg" width="500" />

At this point your Driver is good to go. Unless you change your pixel map or other configuration settings, you wont need to touch it again.

# The Animation API
<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/AnimationAPI.jpg" width="800" />

* For now, the API consists of 15 core draw functions
* The animation system draws inside a FRAME of X+2,Y+2 size as configured in your map
* Nothing is output to the LEDs unless you call the renderLEDS() function
* Colours are generated using the [ColourObject](https://github.com/leonyuhanov/colourObject) API but feel free to use whatever you are used to
* Colours are represented as a 3 byte array of RGB values
* All functions are called in real time, the average roundtrip time is maxed at around 500 micro seconds per command 

## clearBitmap()
Clears the entire frame

## renderLEDs()
Renders the contents of the current frame to the LEDs

## subtractiveFade(byte fadeByValue)
Subtracts fadeByValue from each RGB values of each pixel in the current frame

## rangedSubtractiveFade(byte fadeByValue, byte	LeftBound, byte	RightBound, byte TopBound, byte	BottomBound)
Same as subtractiveFade, but only effects the range sent to the function. The leftBound and RightBound limit your X range and the TopBound and BottomBound limit your Y range

## drawPixel(byte x, byte y, byte* pixelColour)
Draws a pixel of pixelColour at location X,Y in the current frame

## drawHLine(byte x, byte y, byte width, byte* pixelColour)
Draws a Horizontal line of pixelColour at location X,Y with a width of "width" pixels in the current frame

## drawVLine(byte x, byte y, byte height, byte* pixelColour)
Draws a Vertical line of pixelColour at location X,Y with a height of height pixels in the current frame

## drawLine(byte xStart, byte yStart, byte xEnd, byte yEnd, byte* pixelColour)
Draws a line of pixelColour starting at location xStart,YStart ending with location xEnd,yEnd in the current frame

## drawCCircle(byte x, byte y, byte radius, byte* pixelColour)
Draws a circle centred at x,y with a radius of radius pixels with a coloured line of pixelColour. Any pixels out of bounds of the Frame are not drawn

## drawPolly(byte x, byte y, byte radius, unsigned short int rotationAngle, byte N_Points)
Draws a polygon with N_Points points, rotated by rotationAngle, centred at x,y with a radius of radius pixels with a coloured line of pixelColour. Any pixels out of bounds of the Frame are not drawn. 

## fillArea(byte xStart, byte yStart, byte xEnd, byte yEnd, byte* pixelColour)
Fills the areay in range with pixelColour

## shiftDown(byte wrap,	byte LeftBound, byte RightBound, byte TopBound, byte BottomBound)
Shifts the selected range from the current frame by 1 pixel DOWN. If wrap=0 pixels are not wrapped around. if wrap=1 the last line is wrapped around the top

## shiftUp(byte wrap,	byte LeftBound, byte RightBound, byte TopBound, byte BottomBound)
Shifts the selected range from the current frame by 1 pixel UP. If wrap=0 pixels are not wrapped around. if wrap=1 the last line is wrapped around the bottom

## shiftLeft(byte wrap,	byte LeftBound, byte RightBound, byte TopBound, byte BottomBound)
Shifts the selected range from the current frame by 1 pixel LEFT. If wrap=0 pixels are not wrapped around. if wrap=1 the last line is wrapped around the RIGHT

## shiftRight(byte wrap,	byte LeftBound, byte RightBound, byte TopBound, byte BottomBound)
Shifts the selected range from the current frame by 1 pixel RIGHT. If wrap=0 pixels are not wrapped around. if wrap=1 the last line is wrapped around the left

## Utility Functions
There are also some helper fucntions that are designed to help with things like timing, generating modulation, etc...
* void startTimer(unsigned long timeInMilliseconds)
  * 1000ms is 1 second
  * Starts an internal time ticker taht runs for "timeInMilliseconds" milliseconds
* byte hasTimedOut()
  * returns 1 if the "timeInMilliseconds" has elapsed or 0 if not
* int getWave(float intervalTickCounter, float minimumValue, float maximumValue)
  * generates a floored and cield sinusodal wave of integers from "minimumValue" to "maximumValue"
  * you can dynamicly flow through the wave by sending in a float "intervalTickCounter" and incrementing it
  * intervalTickCounter += 0.001 generates very low frequency sinusodal wave
  * intervalTickCounter += 0.01 generates medium frequency sinusodal wave
  * intervalTickCounter += 0.1 generates high frequency sinusodal wave
* Using the [EnvelopeGenerator](https://github.com/leonyuhanov/EnvelopeGenerator) object:
   * void initEnvelope(unsigned short int* points, unsigned short int* ticks, byte numberOfPoints)
   * Dynamic envelope generator
   * Pass "numberOfPoints" number of elemnts via the array of "unsigned short int points"
   * Pass "numberOfPoints" number of "Duration Ticks Per Point" via the array of "unsigned short int ticks"
   * "points" an array of Envelope points, the output will wrap around from the last point to the 1st automaticly
   * "ticks" an array of time ticks PER point. This essentialy gives you a way to scale each point and the speed/gradient of the curve
  ```C++
  envelopeGenerator envelopeOne;
  const byte numberOfPoints = 4;
  unsigned short int frameCounter=0;
  unsigned short int points[numberOfPoints] = {0,10,20,5};
  unsigned short int ticks[numberOfPoints] = {30, 30, 30, 30};
  envelopeOne.initEnvelope(points, ticks, numberOfPoints);
  ```
   * unsigned short int getEnvelope(unsigned short int frameCounter)
   * Generates your curve based on your point list and time blocks using "frameCounter" as the time index
   * "envelopeOne.envelopeBandwidth" can be used to limit your frame counter and keep it in bounds of your dynamic frame ticks
  ```C++
  while(true)
  {
   System.printf("\r\n%d", envelopeOne.getEnvelope(frameCounter));
   frameCounter = (frameCounter+1)%envelopeOne.envelopeBandwidth;
  }
  ```
  The above generates this curve:
  
  <img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/envelopOutput.jpg" width="300" />
  
# An Animation Loop Template

The following is an example template of a simple animation loop. 
* The animations real time steps are inside the function "rainbowSwipe(byte colourIncrement)"
* The animation begins by clearing the frame
* Grabing a colour from the 1st index
* Draw a Vertical line at an indexed X,0 location with height ROWS, using the above colour
* Render the frame to the LEDS
* Fade the Frame by a value of 5
* increment the colour index and the X indexer, wait then repeat untill the timer runs out

You need to make sure the following 2 vars are ste properly:
* controllerMAC: the MAC address of your Controller module
  * You will be provided with the MAC address
  * If you want ot use your own ESP8266 module you will need to identify its WIFI_STA address. Use [this code](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Utilities/GetControllerMAC.ino) it will print the MAC address to the serial console and halt
* driverMAC: the MAC Address of the Driver Modules "WIFI_STA"
  * You will be provided with the MAC address
  * It will also print out on the consoe of your Driver module boot up
  
Once you have that set up, upload the [Control Module](https://github.com/leonyuhanov/WOWPixelDriver/tree/master/Control%20Module) Code to your Controler module

```C++
void loop()
{   
    //Start a timer for 500 seconds
    animationSystem.startTimer(500000);
    //Start your animation
    rainbowSwipe(2);
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
    //Draw a Vertical line of "animationSystem.rows" height 
    // at location ("scanCnt%animationSystem.cols", 0) in your frame, 
    // with colour "tempColour"
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
```
