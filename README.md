# WOWPixelDriver
A semi-hardware based animation library &amp; pixel driver for addressable LEDs

# Why?
The real-time programatic animation system used in all commerical products made by [Elec Dash Tron](https://www.instagram.com/wow_elec_tron/) is Private IP. I want to make the animation system accesible to the public without having to distrubute the source code publicly. I want to also restrict usage to specific hardware modules

# How does it work
The drive consists of 2 physical, ESP8266 based, modules. The Driver Module contains the animation system and is responsible for doing the phsyial driving of pixels. The Control module is user progarmmable and runs a script of animations. The Control module comunicates via the ESP-NOW protcol for realtime execution of commands. 

# Prerequisite Software
* The Arduino IDE via [Arduino](https://www.arduino.cc/)
* The Espressif Flash Download Tools from  [Espressif](https://www.espressif.com/en/support/download/other-tools)
* A spreadhseet app like [Open Office](https://www.openoffice.org/) or [Libre Office](https://www.libreoffice.org/) Calc 

# Setting up your pixel map
A template spreadsheet is provided [here](https://github.com/leonyuhanov/WOWPixelDriver/blob/master/Pixel%20Map%20Template.ods)

* The template is set up for a 17Px Wide X 15px Tall panel
* The panel starts at the top right, and zig-zags down, left, up, left, down, left etc....
* The panel has 255 pixels in total
* Note that the Virtual Bitmap requires a buffer of null pixels at the top bottom, left and right sides of your pixel map

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/pixelmap.jpg" width="800" />

The template concatenates each Column cell into a single C++ array. It then concatenates each row into a 2D C++ array. In the example  the array would be set up like this:

Our LED Panel Has:

* 17 ROWS + 2 Null rows on each side
* 15 Columns + 2 Null columns on each side

```C++
short int pixelMap[NumberOfRows][NumberOfCollumns];
```

# Setting up the pixel map configuration code uplaoder
You will need the following to set up your config file:

* Number of Columns (X range)
* Number of Rows (Y range)
* Number of Pixels
* Bytes per Pixel: 3 for Neopixels/SK6812/WS2812 or 4 for APA102/SK9822
* The Maskmap Array you created in the template file, you can simply paste the text from Cell from the pixel map template

Open the Uploader.ino file and enter all the details at the top of the file. Then make sure you have the corect board selected and all the settings are as follows:

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/UploadConfig.jpg" width="400" />

Connect your DRIVER module via a USB cable, Open the serial monitor & Uplaod your code. Depending on the size of your pixel map it will take about 30 secods to write the config file. Once complete you will see a message on the console.

# Uploading the driver Binary
* The driver Binary is the entire precompiled system
* It is locked to the physical ESP Chip ID of the module you receive
* It will NOT work on any other ESP8266 module

Once you have installed the Espressif Flash Download Tools open it and select "ESP8266 Download Tool" your settings need to be as follows:

<img src="https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/ESPTool%20Settings.jpg" width="400" />

Upload the Binary, when complete, disconect the Driver Module from your PC, conect it to the panel and power it on. You will see a test patern if you have done everything corect. The test patern scans a singe line in RGB in alternate directions to indicate a complete map

