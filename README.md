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

