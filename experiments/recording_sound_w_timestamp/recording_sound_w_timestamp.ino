
// Connect SD Card Module pins as following:
// CS to 25
// SCK to 27
// MOSI to 14
// MISO to 33
// VCC to 5V!
// GND to GND

#include <driver/i2s.h>
#include <mySD.h>

// Libraries to get time from NTP Server
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// connect Mic pins as following:
#define I2S_WS 22
#define I2S_SD 21
#define I2S_SCK 26

#define I2S_PORT I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)
#define I2S_READ_LEN      (16 * 1024)
#define RECORD_TIME       (3) //Seconds
#define I2S_CHANNEL_NUM   (1)
#define FLASH_RECORD_SIZE (I2S_CHANNEL_NUM * I2S_SAMPLE_RATE * I2S_SAMPLE_BITS / 8 * RECORD_TIME)

// Define deep sleep options
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// Sleep for 10 minutes = 600 seconds
uint64_t TIME_TO_SLEEP = 600;

// Replace with your network credentials - only 2.4??
const char* ssid     = "Meins";
const char* password = "12345678";

File file;
File root;

// Save reading number on RTC memory
RTC_DATA_ATTR int readingID = 0;

// save recording
char filename[] = "rec4.wav";
const int headerSize = 44;

// Define NTP Client to get time
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
//
//// Variables to save date and time
//String formattedDate;
//String dayStamp;
//String timeStamp;

void setup() {
  Serial.begin(115200);
  delay(1000);

//  // Connect to Wi-Fi network with SSID and password
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected.");
//
//  // Initialize a NTPClient to get time
//  timeClient.begin();
//  // Set offset time in seconds to adjust for your timezone, for example:
//  // GMT +1 = 3600
//  // GMT +8 = 28800
//  // GMT -1 = -3600
//  // GMT 0 = 0
//  timeClient.setTimeOffset(3600);

  Serial.print("Initializing SD card...");
  /* initialize SD library with Soft SPI pins, if using Hard SPI replace with this SD.begin()*/
  if (!SD.begin(26, 14, 33, 27)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  root = SD.open("/");
  if (root) {
    printDirectory(root, 0);
    root.close();
  } else {
    Serial.println("error opening root?");
  }

  SD.remove(filename);
  file = SD.open(filename, FILE_WRITE);
//  delay(2000);
  if (!file) {
    Serial.println("File is not available!");
  }

  // Enable Timer wake_up
  //  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

//  getTimeStamp();


  byte header[headerSize];
  wavHeader(header, FLASH_RECORD_SIZE);

  file.write(header, headerSize);


  /*
    Sets i2s config options
    Istalls driver
    Sets pin options
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

  // Start deep sleep
  //  Serial.println("DONE! Going to sleep now.");
  //  esp_deep_sleep_start();
}

void loop() {
  // The ESP32 will be in deep sleep
  // it never reaches the loop()
}

// Function to get date and time from NTPClient
//void getTimeStamp() {
//  while (!timeClient.update()) {
//    timeClient.forceUpdate();
//  }
//  // The formattedDate comes with the following format:
//  // 2018-05-28T16:00:13Z
//  // We need to extract date and time
//  formattedDate = timeClient.getFormattedDate();
//  Serial.println(formattedDate);
//
//  // Extract date
//  int splitT = formattedDate.indexOf("T");
//  dayStamp = formattedDate.substring(0, splitT);
//  Serial.println(dayStamp);
//  // Extract time
//  timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
//  Serial.println(timeStamp);
//}


void i2sInit() {
  /*
    Sets i2s config options
    Istalls driver
    Sets pin options
  */
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = I2S_SAMPLE_RATE,
    .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 64,
    .dma_buf_len = 1024,
    .use_apll = 1
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

  const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = -1,
    .data_in_num = I2S_SD
  };

  i2s_set_pin(I2S_PORT, &pin_config);
}


void i2s_adc_data_scale(uint8_t * d_buff, uint8_t* s_buff, uint32_t len) {
  /*
    save original data from I2S(ADC) into flash
  */
  uint32_t j = 0;
  uint32_t dac_value = 0;
  for (int i = 0; i < len; i += 2) {
    dac_value = ((((uint16_t) (s_buff[i + 1] & 0xf) << 8) | ((s_buff[i + 0]))));
    d_buff[j++] = 0;
    d_buff[j++] = dac_value * 256 / 2048;
  }
}

void i2s_adc(void *arg) {
  /*
    I2S (Inter-IC Sound) is a serial, synchronous communication protocol that is usually used for transmitting audio data between two digital audio devices.
    Assigns the size I2S_READ_LEN takes in space in memory once for read and once for write
    Then reads two times -> Metadata or some protocol ?
    Then reads until the defined limit is reached
    After each read it is also writing the read values to the file

    Frees the memory
    Displays all files
  */

  int i2s_read_len = I2S_READ_LEN;
  int flash_wr_size = 0;
  size_t bytes_read;

  char* i2s_read_buff = (char*) calloc(i2s_read_len, sizeof(char));
  uint8_t* flash_write_buff = (uint8_t*) calloc(i2s_read_len, sizeof(char));

  i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);

  Serial.println(" *** Recording Start *** ");
  while (flash_wr_size < FLASH_RECORD_SIZE) {
    //read data from I2S bus, in this case, from ADC.
    i2s_read(I2S_PORT, (void*) i2s_read_buff, i2s_read_len, &bytes_read, portMAX_DELAY);
    example_disp_buf((uint8_t*) i2s_read_buff, 64);
    //save original data from I2S(ADC) into flash.
    i2s_adc_data_scale(flash_write_buff, (uint8_t*)i2s_read_buff, i2s_read_len);
    file.write((const byte*)flash_write_buff, i2s_read_len);
    flash_wr_size += i2s_read_len;
    ets_printf("Sound recording %u%%\n", flash_wr_size * 100 / FLASH_RECORD_SIZE);
    ets_printf("Never Used Stack Size: %u\n", uxTaskGetStackHighWaterMark(NULL));
  }
  file.close();
  printDirectory(root, 0);


  free(i2s_read_buff);
  i2s_read_buff = NULL;
  free(flash_write_buff);
  flash_write_buff = NULL;
  vTaskDelete(NULL);
}

void example_disp_buf(uint8_t* buf, int length)
{
  printf("======\n");
  for (int i = 0; i < length; i++) {
    printf("%02x ", buf[i]);
    if ((i + 1) % 8 == 0) {
      printf("\n");
    }
  }
  printf("======\n");
}

void wavHeader(byte* header, int wavSize) {
  header[0] = 'R';
  header[1] = 'I';
  header[2] = 'F';
  header[3] = 'F';
  unsigned int fileSize = wavSize + headerSize - 8;
  header[4] = (byte)(fileSize & 0xFF);
  header[5] = (byte)((fileSize >> 8) & 0xFF);
  header[6] = (byte)((fileSize >> 16) & 0xFF);
  header[7] = (byte)((fileSize >> 24) & 0xFF);
  header[8] = 'W';
  header[9] = 'A';
  header[10] = 'V';
  header[11] = 'E';
  header[12] = 'f';
  header[13] = 'm';
  header[14] = 't';
  header[15] = ' ';
  header[16] = 0x10;
  header[17] = 0x00;
  header[18] = 0x00;
  header[19] = 0x00;
  header[20] = 0x01;
  header[21] = 0x00;
  header[22] = 0x01;
  header[23] = 0x00;
  header[24] = 0x80;
  header[25] = 0x3E;
  header[26] = 0x00;
  header[27] = 0x00;
  header[28] = 0x00;
  header[29] = 0x7D;
  header[30] = 0x00;
  header[31] = 0x00;
  header[32] = 0x02;
  header[33] = 0x00;
  header[34] = 0x10;
  header[35] = 0x00;
  header[36] = 'd';
  header[37] = 'a';
  header[38] = 't';
  header[39] = 'a';
  header[40] = (byte)(wavSize & 0xFF);
  header[41] = (byte)((wavSize >> 8) & 0xFF);
  header[42] = (byte)((wavSize >> 16) & 0xFF);
  header[43] = (byte)((wavSize >> 24) & 0xFF);

}

void printDirectory(File dir, int numTabs) {

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');   // we'll have a nice indentation
    }
    // Print the name
    Serial.print(entry.name());
    /* Recurse for directories, otherwise print the file size */
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      /* files have sizes, directories do not */
      Serial.print("\t\t");
      Serial.println(entry.size());
    }
    entry.close();
  }
}
