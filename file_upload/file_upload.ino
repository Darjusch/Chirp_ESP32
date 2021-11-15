#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>



File root;
// Replace with your network credentials
const char* ssid = "Meins";
const char* password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void initSDCard(){
   Serial.print("Initializing SD card...");
  /* initialize SD library with Soft SPI pins, if using Hard SPI replace with this SD.begin()*/
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
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
  /* open "file" for writing */
  root = SD.open("index.html", FILE_WRITE);
  /* if open succesfully -> root != NULL
    then write string  to it
  */
  if (root) {
    root.write("<h1>Hello world!</h1>");
    root.flush();
    /* close the file */
    root.close();
  } else {
    /* if the file open error, print an error */
    Serial.println("error writing to file");
  }
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
  initSDCard();

  
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, "/index.html", "text/html");
  });

  server.serveStatic("/", SD, "/");

  server.begin();
}



void loop() {
  
}
