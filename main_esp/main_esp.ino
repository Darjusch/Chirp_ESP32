#include <driver/i2s.h>
#include <SPI.h>
#include <NTPClient.h>
#include <mySD.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32WebServer.h>
#include <ESPmDNS.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
ESP32WebServer server(80);

const char* ssid = "Meins";
const char* password = "12345678";

//const char* ssid = "Vodafone-63B4";
//const char* password = "qcCh6yRbxxX4rq6N";

void setup() {
  Serial.begin(115200);

  wifiInit();

  timeClient.begin();
  timeClient.setTimeOffset(3600);

  getTimeStamp();

  sdInit();
  /*
    Sets i2s config options
    Istalls driver
  */
  i2sInit();

  /*
    xTaskCreate creates a RTOSTask and keeps track of the state of the task
    I2S (Inter-IC Sound) is a serial, synchronous communication protocol that is usually used for transmitting audio data between two digital audio devices.
    Assigns the size I2S_READ_LEN takes in space in memory once for read and once for write
    Then reads two times -> Metadata or some protocol ?
    Then reads until the defined limit is reached
    After each read it is also writing the read values to the file
    Frees the memory
    Displays all files
  */

  xTaskCreate(i2s_adc, "i2s_adc", 1024 * 2, NULL, 1, NULL);
  createAudioFileWithHeader();
  startServer();
}

void loop() {
  server.handleClient();
}
