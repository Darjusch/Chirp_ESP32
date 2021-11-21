## Sound recording to SD card with an ESP32

# Setting up the project

## Software Setup

#### Clone the repository

#### Instal the Arduino IDE - https://www.arduino.cc/en/software

##### Set the Arduino IDE up to be able to work with ESP32 - https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

#### Install the following Requirments:

- [<NTPClient.h>](https://github.com/arduino-libraries/NTPClient) important! to work as expected, please [add some extra lines](https://github.com/arduino-libraries/NTPClient/issues/113
) to the library
- [<mySD.h>](https://github.com/nhatuan84/esp32-micro-sdcard)
- [<ESP32WebServer.h>](https://github.com/Pedroalbuquerque/ESP32WebServer)

#### Change the SSID and Password to the one you are using:
##### WIFI_SSID YOURSSIDNAME 
##### WIFI_PASSWORD YOURWIFIPASSWORD


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

- [code via](http://www.iotsharing.com/2019/07/how-to-turn-esp-with-sdcard-or-spiffs-a-web-file-server.html)

## The audio recording and the collection of sensor data is currently seperated
## For the setup of MQTT, NodeRED, Sensordata collection please read the instructions here:

https://github.com/Darjusch/Chirp_ESP32/tree/main/sensor_via_mqtt
