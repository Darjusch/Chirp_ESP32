
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>
//#include <mySD.h>

File root;
// Replace with your network credentials
const char* ssid = "Meins";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void initSDCard(){

 Serial.print("Initializing SD card...");
 
  if (!SD.begin(26)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  /* Begin at the root "/" */
  root = SD.open("/");
  if (root) {
    root.close();
  } else {
    Serial.println("error opening file"); 
  }
  /*
  root = SD.open("index.html", FILE_WRITE);
  if (root) {
    root.println("<h1>Hello world!</h1>");
    root.flush();
    /* close the file */
    root.close();
  } else {
    Serial.println("error writing to file");
  }
  */
  delay(1000);

  Serial.println("done!");
  
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  SD.begin(26);
  initSDCard();

  
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/TEST.HTML", "text/html", true);
  });

  server.serveStatic("/", SD, "/");
  server.begin();
}

void loop() {
  
}
