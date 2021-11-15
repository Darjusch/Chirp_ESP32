## Sound recording to SD card then sending the .wav file to the cloud

### SD Card Module

Connect SD Card Module pins as following:

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

### ESP32-DHT22-MQTT (deprecated)

see in the folder
