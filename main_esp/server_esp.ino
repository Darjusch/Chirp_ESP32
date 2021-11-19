File root;

void startServer() {
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void handleNotFound() {
  if (loadFromSDCARD(server.uri())) return;
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

bool loadFromSDCARD(String path) {
  path.toLowerCase();
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";

  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".txt")) dataType = "text/plain";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  Serial.println(dataType);
  File dataFile = SD.open(path.c_str());

  if (!dataFile)
    return false;

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    Serial.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleRoot() {
  root = SD.open("/");
  String res = printDirectory(root, 0);
  server.send(200, "text/html", res);
}

void sdInit() {
  Serial.print("Initializing SD card...");
  /* initialize SD library with Soft SPI pins, if using Hard SPI replace with this SD.begin()*/
  if (!SD.begin(5, 23, 19, 18)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

String printDirectory(File dir, int numTabs) {
  String response = "";
  dir.rewindDirectory();

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');   // we'll have a nice indentation
    }
    // Recurse for directories, otherwise print the file size
    if (entry.isDirectory()) {
      printDirectory(entry, numTabs + 1);
    } else {
      response += String("<a href='") + String(entry.name()) + String("'>") + String(entry.name()) + String("</a>") + String("</br>");
    }
    entry.close();
  }
  return String("List files:</br>") + response;
}
