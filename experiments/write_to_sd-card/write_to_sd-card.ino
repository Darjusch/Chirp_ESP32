// source: http://www.iotsharing.com/2017/05/how-to-use-arduino-esp32-to-store-data-to-sdcard.html
// Connect pins as following:

// CS to 26
// SCK to 27
// MOSI to 14
// MISO to 33
// VCC to 5V!
// GND to GND

// SD Card has to be formatted to MS-DOS(FAT)
// formatting via cli:
// $diskutil list
// $diskutil info /dev/disk2s1|grep "Block Size"
// sudo newfs_msdos -F 32 -c 1 disk2s1 
// diskutil umount disk2s1

#include <mySD.h>

File root;

void setup()
{
  Serial.begin(115200);

  Serial.print("Initializing SD card...");
  /* initialize SD library with Soft SPI pins, if using Hard SPI replace with this SD.begin()*/
  if (!SD.begin(26, 14, 33, 27)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  /* Begin at the root "/" */
  root = SD.open("/");
  if (root) {
//    printDirectory(root, 0);
    root.close();
  } else {
    Serial.println("error opening test.txt"); 
  }
  /* open "test.txt" for writing */
  root = SD.open("test.txt", FILE_WRITE);
  /* if open succesfully -> root != NULL
    then write string "Hello world!" to it
  */
  if (root) {
    root.println("Hello world!");
    root.flush();
    /* close the file */
    root.close();
  } else {
    /* if the file open error, print an error */
    Serial.println("error opening test.txt");
  }
  delay(1000);
  /* after writing then reopen the file and read it */
  root = SD.open("test.txt");
  if (root) {
    /* read from the file until there's nothing else in it */
    while (root.available()) {
      /* read the file and print to Terminal */
      Serial.write(root.read());
    }
    root.close();
  } else {
    Serial.println("error opening test.txt");
  }

  Serial.println("done!");
}

void loop()
{
}

//void printDirectory(File dir, int numTabs) {
//
//  while (true) {
//    File entry =  dir.openNextFile();
//    if (! entry) {
//      break;
//    }
//    for (uint8_t i = 0; i < numTabs; i++) {
//      Serial.print('\t');   // we'll have a nice indentation
//    }
//    // Print the name
//    Serial.print(entry.name());
//    /* Recurse for directories, otherwise print the file size */
//    if (entry.isDirectory()) {
//      Serial.println("/");
//      printDirectory(entry, numTabs + 1);
//    } else {
//      /* files have sizes, directories do not */
//      Serial.print("\t\t");
//      Serial.println(entry.size());
//    }
//    entry.close();
//  }
//}
