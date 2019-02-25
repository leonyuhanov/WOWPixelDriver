### All Code is currently NON functional for demonstrational purposes only.

# WOWPixelDriver
A semi-hardware based animation library &amp; pixel driver for addressable LEDs

# Why?
The real-time programatic animation system used in all commerical products made by [Elec Dash Tron](https://www.instagram.com/wow_elec_tron/) is Private IP. I want to make the animation system accesible to the public without having to distrubute the source code publicly. I want to also restrict usage to specific hardware modules

# How does it work
The drive consists of 2 physical, ESP8266 based, modules. The Driver Module contains the animation system and is responsible for doing the physical driving of pixels. The Control module is user programmable and runs a script of animations. The Control module comunicates via the ESP-NOW protcol for realtime execution of commands. 

# What pixels are supported
The driver uses the ESP8266 SPI port to drive both Clocked and Unclocked Chipsets:
* APA102 APA102C & variants from AP Corp NeonWorld
* SK9822
* WS2812 5050
* WS2812B 5050
* WS2812 2020
* SK6812 5050, 3535 & 2427(With Opaque Lense) New models of the 2427 chipset is NOT suported

# Prerequisite Software
* The Arduino IDE via [Arduino](https://www.arduino.cc/)
* The Espressif Flash Download Tools from  [Espressif](https://www.espressif.com/en/support/download/other-tools)
* A spreadhseet app like [Open Office](https://www.openoffice.org/) or [Libre Office](https://www.libreoffice.org/) Calc 

# Setting up your pixel map
A template spreadsheet is provided [here](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Pixel%20Map%20Template.ods) This doc will focus on a simple matrix, but Im adding more complex example templates into the file as we go

* The template is set up for a 17Px Wide X 15px Tall panel
* The panel starts at the top right, and zig-zags down, left, up, left, down, left etc....
* The panel has 255 pixels in total
* Note that the Virtual Bitmap requires a buffer of null pixels at the top bottom, left and right sides of your pixel map

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/pixelmap.jpg" width="900" />

The template concatenates each Column cell into a single C++ array. It then concatenates each row into a 2D C++ array. In the example  the array would be set up like this:

Our LED Panel Has:

* 17 ROWS + 2 Null rows on each side
* 15 Columns + 2 Null columns on each side

```C++
short int pixelMap[NumberOfRows][NumberOfCollumns];
```

# Setting up the pixel map configuration code uploader
You will need the following to set up your config file:

* Number of Columns (X range)
* Number of Rows (Y range)
* Number of Pixels
* Bytes per Pixel: 3 for Neopixels/SK6812/WS2812 or 4 for APA102/SK9822
* The Maskmap Array you created in the template file, you can simply paste the text from Cell from the pixel map template

The Uploader code will look something like this for the example we will be working with:
```C++
//Number of Physical Pixels in your system
const short int numLeds = 255;
//bytesPerLed lets the system know if you are working with WS2812/SK6812 (3 Bytes) or APA102/SK9822 (4 Bytes) pixels 
const short int bytesPerLed = 4;
//rows =  is the total number of Rows(Y Axis) in your LED set up, this is usualy the number of pixels TALL+2(1 NUll line at the top and bottom)
//cols =  is the total number of Columns(X Axis) in your LED set up, this is usualy the number of pixels WIDE+2(1 NUll line at the LEFT and RIGHT)
const short int rows = 17, cols = 19;
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


Open the [Uploader.ino](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Uploader/Uploader.ino) file and enter all the details at the top of the file. Then make sure you have the corect board selected and all the settings are as follows:

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/UploadConfig.jpg" width="400" />

Connect your DRIVER module via a USB cable, Open the serial monitor & Uplaod your code. Depending on the size of your pixel map it will take about 30 secods to write the config file. Once complete you will see a message on the console.

# Uploading the driver Binary
* The driver Binary is the entire precompiled system
* It is locked to the physical ESP Chip ID of the module you receive
* It will NOT work on any other ESP8266 module

Once you have installed the Espressif Flash Download Tools open it and select "ESP8266 Download Tool" your settings need to be as follows:

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/ESPTool%20Settings_esp8266.jpg" width="600" />

Upload the Binary, when complete, disconect the Driver Module from your PC, conect it to the panel and power it on. You will see a test patern if you have done everything corect. The test patern scans a singe line in RGB in alternate directions to indicate a complete map

This is some example output for the template:

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/ExampleOutput.jpg" width="600" />

# The Animation API
<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/AnimationAPI.jpg" width="800" />

* For now, the API consists of 15 functions
* The animation system is based around an X,Y address system of a flat 2D BITMAP
* Colours are generated using the [ColourObject](https://github.com/leonyuhanov/colourObject) API but feel free to use whatever you are used to
* Colours are represented as a 3 byte array of RGB values
* All functions are called in real time, however there is a small LAG due to the way the ESPNOW RX/TX system is configured. The driver has a buffer to make sure it can handle huge amounts of real time commands

## clearBitmap()
Clears the entire frame

## renderLEDs()
Renders the contents of the current frame to the LEDs

## subtractiveFade(byte fadeByValue)
Subtracts fadeByValue from each RGB values of each pixel in the current frame

## rangedSubtractiveFade(byte fadeByValue, byte	LeftBound, byte	RightBound, byte TopBound, byte	BottomBound)
Same as subtractiveFade, but only effects the range sent to the function. the leftBound and RightBound limit your X range and the TopBound and BottomBound limit your Y range

## drawPixel(byte x, byte y, byte* pixelColour)
Draws a pixel of pixelColour at location X,Y in the current frame

## drawHLine(byte x, byte y, byte width, byte* pixelColour)
Draws a Horizontal line of pixelColour at location X,Y with a width of width pixels in the current frame

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


# An Animation Loop Template

The following is an example template of a simple animation loop. 

```C++
void loop()
{   
    //Start your animtion timer for a period of 5 seconds
    startTimer(5000);
    //Begin your animation
    yourFancyAnimation();
}
void yourFancyAnimation()
{
  byte tempColour[3];
  byte xLoc=cols/2, yLoc=rows/2;
  byte colourIndex=0;
  
  while(true)
  {
    //Continue inside this loop untill your timer runs out
    if(hasTimedOut()){return;}   
    //Pull a colour from the palete at index "colourIndex" and place it inside the "tempColour" colour array
    colourObject.getColour(cIndex%colourObject._bandWidth, tempColour);
    //draw a pixel with colour "tempColour" at position xLoc, YLoc
    drawPixel(xLoc, yLoc, tempColour);
    //Render your frame to the LEDs
    renderLEDs();
    //Increment the "colourIndex" variable so that your next pass has a new colour value
    colourIndex++;
    //Some delay to make sure you dont get Epilepsy  
    delay(10);
    //Restart the animation again    
  }
}
```

This example will draw a single pixel n the centre of your pixel mapped LED system, the pixels colour will chagne ever 10ms. The basic operation of any animation is:

* Draw something in the virtual frame
* Dump that frame to your LEDS so that its visbile
* Repeat

This Simple template is missing A LOT of the things that make up complex animations. For example blanking the frame or fading the frame gently to give the effect of motion. Here is an example that scans a Vertical Line and leaves a trail of pixels behind it:

```C++
void trailingVerticalLine()
{
  byte tempColour[3];
  byte xLoc=0, yLoc=0;
  unsigned short int frameCounter=0;
  byte colourIndex=0;
  
  while(true)
  {
    //Continue inside this loop untill your timer runs out
    if(hasTimedOut()){return;}   
    //Pull a colour from the palete at index "colourIndex" and place it inside the "tempColour" colour array
    colourObject.getColour(cIndex%colourObject._bandWidth, tempColour);
    //draw a vertical line of height "rows" with colour "tempColour" at position (frameCounter % xLoc), yLoc=0;
    drawVLine(frameCounter%cols, yLoc, rows, tempColour);
    //Render your frame to the LEDs
    renderLEDs();
    //Increment the "colourIndex" variable so that your next pass has a new colour value
    colourIndex++;
    //Increment the frameCounter so that our line moves to te next X pixel
    frameCounter++;
    //Fade the entire frame by 5
    subtractiveFade(5);
    //Delay your next step by 50ms 
    delay(50);
    //Restart the animation again    
  }
}
```
