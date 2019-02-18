# WOWPixelDriver
A semi-hardware based animation library &amp; pixel driver for addressable LEDs

# Why?
The real-time programatic animation system used in all commerical products made by [Elec Dash Tron](https://www.instagram.com/wow_elec_tron/) is Private IP. I want to make the animation system accesible to the public without having to distrubute the source code publicly. I want to also restrict usage to specific hardware modules

# How does it work
The drive consists of 2 physical modules. An ESP32 and an ESP8266. The ESP32 contains the animation system and is responsible for doing the phsyial driving of pixels. The ESP8266 is user progarmmable and runs a script of animations. The ESP8266 sends commands to te ESP32 via the ESP-NOW protcol for realtime execution of commands

# Basic Design & Operation
https://github.com/leonyuhanov/WOWPixelDriver/blob/master/pics/basicconfig.jpg
