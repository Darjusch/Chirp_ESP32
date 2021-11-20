## Sound recording to SD card with an ESP32

# Setting up the project

## Software Setup

### Clone the repository

### Instal the Arduino IDE - https://www.arduino.cc/en/software

### Install the following Requirments:

#include <NTPClient.h> - https://github.com/arduino-libraries/NTPClient

#include <mySD.h> - https://github.com/nhatuan84/esp32-micro-sdcard

#include <ESP32WebServer.h> - https://github.com/Pedroalbuquerque/ESP32WebServer

## Hardware Setup

### SD Card Module

Connect SD Card Module pins as follows:

- CS to 5
- SCK to 18
- MOSI to 23
- MISO to 19 (12 won't work)
- VCC to 5V!
- GND to GND

### INMP441 Microphone sound recording

connect Mic pins as following:
[source](https://bestofcpp.com/repo/atomic14-esp32-i2s-mic-test)

- VDD to 3V
- GND to GND
- L/R to GND (Left channel or right channel)
- WS to 22 (Left right clock)
- SD to 21 (Serial data)
- SCK to 25 (Serial clock)

### Reading files from the SD Card
- important! you must [add some extra](https://github.com/arduino-libraries/NTPClient/issues/113
) methods to the NTPClient 
- [via](http://www.iotsharing.com/2019/07/how-to-turn-esp-with-sdcard-or-spiffs-a-web-file-server.html)
- [(could be insteresting too)](https://www.iotsharing.com/2018/01/esp32-multipart-upload-file-and-download-via-http.html)

### ESP32-DHT22-MQTT (deprecated)

see in the folder
