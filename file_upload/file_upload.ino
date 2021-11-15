#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
 
  
const char* ssid = "Meins";
const char* password =  "12345678";
  
AsyncWebServer server(80);
  
void setup(){
  Serial.begin(115200);
  
  if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
  }
//////////////
// Write to file
  File file = SPIFFS.open("/file.html", "w");
  if (!file) {
      Serial.println("Error opening file for writing");
      return;
  }
  int bytesWritten = file.print("<h1>YESSSS IT WORKS</h1>");
 
  if (bytesWritten > 0) {
      Serial.println("File was written");
      Serial.println(bytesWritten);
   
  } else {
      Serial.println("File write failed");
  }
  file.close();
//////////////
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());
  
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("here");
    request->send(SPIFFS, "/file.html", "text/html", true);
  });
   
  server.on("/interpret", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/file.html", "text/html", false);
  });
  
  server.begin();
}
  
void loop(){}
