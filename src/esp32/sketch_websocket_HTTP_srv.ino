
#include <WiFi.h>
#include <HardwareSerial.h>
#include <ESP32Ping.h>
#include <SimpleFTPServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
//#include <esp_timer.h>
#include <FastCRC.h>  // Include the FastCRC library
#include <ArduinoJson.h>
#include <SPI.h>
#include <time.h>
#include <SD.h>
#include <Wire.h>
#include <AHTxx.h>

#define PIN_SPI_CS 5  // The ESP32 pin GPIO5
#define GREEN_LED_PIN 2
#define RED_LED_PIN 15
#define TXD2 16
#define RXD2 17
#define JDY_CSS 0
#define JDY_SET 4

const char* ssidList[] = { "SSID-1", "SSID-2", "SSID-2" };
const char* passwordList[] = { "PASW-1", "PASW-2", "PASW-3" };
const int numberOfNetworks = sizeof(ssidList) / sizeof(ssidList[0]);

const char* www_username = "TEST";  // web page username
const char* www_password = "TEST";     // web page password

const char* settings_username = "TEST";      // web page username
const char* settings_password = "TEST";  // web page password

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7200;  // EET GMT+2
const int dstOffset_sec = 3600;   // DST +1 hour
// Timer handle
//esp_timer_handle_t periodic_timer2;
// Millisecond counter
int ms_counter = 0;
// Variables for reconnect
unsigned long wifi_previousMillis = 0;
const long wifi_interval = 60 * 1000;  // Check connection every 60 seconds
unsigned long ntp_previousMillis = 0;
const long ntp_interval = 12 * 60 * 60 * 1000;  // 12 hours in milliseconds;  // Check connection every 12 hours
//const long ntp_interval = 2 * 60 * 1000;
// Timer variables
unsigned long lastTime2 = 0;
unsigned long timerDelay2 = 50;
unsigned long lastTime3 = 0;
unsigned long timerDelay3 = 500;
unsigned long timeoutDuration = 4000;  // 4s Timeout duration in milliseconds
unsigned long startTime_l1 = 0;        // Record the start time
unsigned long startTime_l2 = 0;
unsigned long startTime_l3 = 0;

bool l1_alive = false;
bool l2_alive = false;
bool l3_alive = false;

bool sentLine1 = false;
bool sentLine2 = false;
bool sentLine3 = false;

unsigned long startTime_ms = 0;  // Record the start time
const long msDuration = 1;       // 1ms Timeout duration in milliseconds

//unsigned long aht_ms_counter = 0;  // 10 seconds in milliseconds
unsigned long aht_previousMillis = 0;

int srvclients = 0;
const int maxClients = 8;
int led_state = LOW;
bool timesynch_ntp = false;
bool aht_flag = false;

const int bufferSize = 65;
char buffer[bufferSize];
int indexr = 0;
bool newData = false;
char incoming;

struct tm timeinfo;

String l1type = "OFF-LINE";
String l1color = "OFF-LINE";
String l1speed = "OFF-LINE";
String l1counter = "OFF-LINE";
String l1dia = "OFF-LINE";
String l1load60 = "OFF-LINE";
String l1load32 = "OFF-LINE";

String l2type = "OFF-LINE";
String l2color = "OFF-LINE";
String l2speed = "OFF-LINE";
String l2counter = "OFF-LINE";
String l2dia = "OFF-LINE";
String l2load45 = "OFF-LINE";
String l2load32 = "OFF-LINE";

String l3type = "OFF-LINE";
String l3color = "OFF-LINE";
String l3speed = "OFF-LINE";
String l3counter = "OFF-LINE";
String l3dia = "OFF-LINE";
String l3load80 = "OFF-LINE";
String l3load40 = "OFF-LINE";

String temp = "";
String hum = "";
String ip = "*.*.*.*";

// Function prototypes
//void timerCallback2(void* arg);
void handleWebSocketMessage(AsyncWebSocketClient* client, AwsFrameInfo* info, uint8_t* data, size_t len);
void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);
void handleRoot(AsyncWebServerRequest* request);
void handleNotFound(AsyncWebServerRequest* request);
void handleError(AsyncWebServerRequest* request);
void handleFileRequest(AsyncWebServerRequest* request);
void handleDirRequest(AsyncWebServerRequest* request);
void handleDownload(AsyncWebServerRequest* request);
void setup();
void loop();
void synchronizeTime();
void setDefaultTime();
void sendData(String id);
void initWebSocket();
void connectToWiFi();
void checkWiFiConnection();
void init_AHT15();
void init_sd();
void logDataLine(String data, String customFilename);
void ntptimesynch();
void extractSubstrings(const String& str, String* parts, int maxParts);
void handleLineTimeouts(unsigned long currentMillis);
void handleLineData(unsigned long currentMillis);
void processRadioMessage(String parts[]);

bool verifyCRC8(const String& receivedString);

String listDirectory(String path);
String readFileContent(String filename);
String getSensorReadings();

AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);  //sensor address, sensor type

FastCRC8 CRC8;  // Create an instance of the FastCRC8 class

FtpServer ftpSrv;

HardwareSerial mySerial(2);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Get Sensor Readings and return JSON object
String getSensorReadings() {
  // Create a JSON document
  StaticJsonDocument<1024> readings;

  readings["l1_type"] = l1type;
  readings["l1_typec"] = l1color;
  readings["l1_counter"] = l1counter;
  readings["l1_speed"] = l1speed;
  readings["l1_dia"] = l1dia;
  readings["l1_load60"] = l1load60;
  readings["l1_load32"] = l1load32;

  readings["l2_type"] = l2type;
  readings["l2_typec"] = l2color;
  readings["l2_counter"] = l2counter;
  readings["l2_speed"] = l2speed;
  readings["l2_dia"] = l2dia;
  readings["l2_load45"] = l2load45;
  readings["l2_load32"] = l2load32;

  readings["l3_type"] = l3type;
  readings["l3_typec"] = l3color;
  readings["l3_counter"] = l3counter;
  readings["l3_speed"] = l3speed;
  readings["l3_dia"] = l3dia;
  readings["l3_load80"] = l3load80;
  readings["l3_load40"] = l3load40;

  readings["con_clients"] = srvclients;
  readings["send_temp"] = temp;
  readings["send_hum"] = hum;

  String jsonString;
  serializeJson(readings, jsonString);
  return jsonString;
}
/*
void timerCallback2(void* arg) {
  ms_counter += 10;        // Increment by 10 milliseconds
  if (ms_counter == 10) {  // Do something at 10ms
    sendData("LINE1");
  }
  if (ms_counter == 340) {  // Do something at 350ms
    sendData("LINE2");
  }
  if (ms_counter == 680) {  // Do something at 700ms
    sendData("LINE3");
  }
  if (ms_counter == 1000) {
    ms_counter = 0;  // Reset the counter every second
  }
}
*/

void synchronizeTime() {
  // Initial time configuration
  ntptimesynch();
  if (!timesynch_ntp) {
    struct tm timeinfo = { 0 };  // Set default date and time: 2024-12-13 00:00:00
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 11;  // December (0-based index)
    timeinfo.tm_mday = 13;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    time_t t = mktime(&timeinfo);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
    // Serial.println("Default time set to 2024-12-13 00:00:00");
  }
}
/*
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.printf("Connected to: %-32.32s", WiFi.SSID().c_str());
  Serial.println();
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("ESP32 Web Server IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP32 Web Server Hostname: ");
  Serial.println(WiFi.getHostname());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.reconnect();
  //ESP.restart();
}

void handleWebSocketMessage(AsyncWebSocketClient* client, AwsFrameInfo* info, uint8_t* data, size_t len) {
  //AwsFrameInfo* info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    // Ensure we do not access out-of-bounds memory
    char message[len + 1];
    strncpy(message, (char*)data, len);
    message[len] = '\0';
    // Check if the message is "getReadings"
    if (strcmp(message, "getReadings") == 0) {
      //if it is, send current sensor readings
      String sensorReadings = getSensorReadings();
      client->text(sensorReadings);
      return;
    }
  }
}
*/
void handleWebSocketMessage(AsyncWebSocketClient* client, AwsFrameInfo* info, uint8_t* data, size_t len) {
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    // Ensure we do not access out-of-bounds memory
    char message[len + 1];
    strncpy(message, (char*)data, len);
    message[len] = '\0';

    // Check if the message is "getReadings"
    if (strcmp(message, "getReadings") == 0) {
      // If it is, send current sensor readings
      String sensorReadings = getSensorReadings();
      client->text(sensorReadings);
      return;
    }
    if (strcmp(message, "restart") == 0) {
      ws.textAll("redirect");
      delay(500);
      ESP.restart();
      return;
    }
    if (strcmp(message, "get_jdy") == 0) {
      client->text(get_jdy());
      return;
    }
    // Handle configuration messages (assuming format "config=value1,value2")
    if (strncmp(message, "config=", 7) == 0) {
      String configData = String(message + 7);
      int delimiterIndex = configData.indexOf(',');
      if (delimiterIndex != -1) {
        String value1 = configData.substring(0, delimiterIndex);
        String value2 = configData.substring(delimiterIndex + 1);
        String response = configureJDY41(value1, value2);
        client->text(response);  // Send the response from JDY-41 to the client return;
        return;
      }
    }
  }
}

String get_jdy() {
  digitalWrite(JDY_SET, LOW);
  delay(200);

  uint8_t bytesToSend[] = { 0xAA, 0xE2, 0x0D, 0x0A };
  mySerial.write(bytesToSend, sizeof(bytesToSend));

  delay(200);
  mySerial2RX();  // Read the response using mySerial2RX

  //printHexBuffer((uint8_t*)buffer); // Print the hex buffer for debugging

  // Extract byte 4 and byte 5 and convert to int
  int value1 = buffer[3];  // 4th byte
  int value2 = buffer[4];  // 5th byte

  digitalWrite(JDY_SET, HIGH);

  // Return the values as a concatenated string
  return "ch:" + String(value1) + "tx:" + String(value2);
}

/*
void printHexBuffer(uint8_t* buffer) {
  Serial.print("Hex Buffer: ");
  for (int i = 0; buffer[i] != '\0'; i++) {
    char upperNibble = (buffer[i] >> 4) & 0x0F;
    char lowerNibble = buffer[i] & 0x0F;
    // Convert nibbles to their corresponding hex characters
    upperNibble = (upperNibble > 9) ? (upperNibble - 10 + 'A') : (upperNibble + '0');
    lowerNibble = (lowerNibble > 9) ? (lowerNibble - 10 + 'A') : (lowerNibble + '0');
    Serial.print("0x");
    Serial.print(upperNibble);
    Serial.print(lowerNibble);
    Serial.print(" ");
  }
  Serial.println();
}
*/

String configureJDY41(String value1, String value2) {
  // Serial.println("Configuring JDY-41 with:");
  // Serial.println("Channel: " + value1);
  // Serial.println("Tx Power: " + value2);

  // Convert value1 and value2 from string representations of numbers to byte values
  int channelValue = value1.toInt();
  int txpowerValue = value2.toInt();

  uint8_t channel = (uint8_t)channelValue;
  uint8_t txpower = (uint8_t)txpowerValue;
  // Set JDY-41 module to configuration mode
  digitalWrite(JDY_SET, LOW);
  delay(200);

  // Construct the byte sequence
  uint8_t configSequence[] = {
    0xA9, 0xE1, 0x06, channel, txpower, 0xA0,
    0x66, 0x77, 0x88, 0x99, 0x00, 0x05, 0x0D, 0x0A
  };

  // Send the byte sequence to JDY-41
  mySerial.write(configSequence, sizeof(configSequence));
  // Wait for response from JDY-41
  delay(200);     // Adjust delay as needed for your setup
  mySerial2RX();  // Read the response using mySerial2RX
  String response = String(buffer);
  if (!response.equals("+OK\r")) {
    response = "failed...";
  }
  // Serial.println("Received: " + response);
  // Set JDY-41 module back to normal mode
  digitalWrite(JDY_SET, HIGH);
  return response;
}

void onEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      srvclients++;
      if (srvclients >= maxClients) {
        client->text("redirect");
        digitalWrite(RED_LED_PIN, HIGH);
        // Serial.printf("WebSocket client #%u rejected from %s\n", client->id(), client->remoteIP().toString().c_str());
      }  //else {
         // Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
         // Serial.printf("Web Server clients: %i\n", srvclients);
      //}
      break;
    case WS_EVT_DISCONNECT:
      srvclients--;
      // Serial.printf("WebSocket client #%u disconnected\n", client->id());
      // Serial.printf("Web Server clients: %i\n", srvclients);
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(client, (AwsFrameInfo*)arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      // Serial.printf("WebSocket Error Type: %d\n", type);
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void connectToWiFi() {
  for (int i = 0; i < numberOfNetworks; i++) {
    // Serial.print("Trying to connect to: ");
    // Serial.println(ssidList[i]);
    WiFi.begin(ssidList[i], passwordList[i]);
    int attemptCount = 0;
    while (WiFi.status() != WL_CONNECTED && attemptCount < 10) {
      delay(1000);
      // Serial.print(".");
      attemptCount++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      // Serial.print("Connected to: ");
      // Serial.println(ssidList[i]);
      // Serial.print("ESP32 Web Server IP address: ");
      // Serial.println(WiFi.localIP());
      // Serial.print("ESP32 Web Server Hostname: ");
      // Serial.println(WiFi.getHostname());
      ip = WiFi.localIP().toString();
      digitalWrite(RED_LED_PIN, LOW);
      return;
    } else {
      // Serial.println("Failed to connect");
      WiFi.disconnect();
      delay(1000);
    }
  }
  // Serial.println("Could not connect to any WiFi network");
  WiFi.disconnect();
  delay(1000);
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    // Serial.println("WiFi connection lost. Reconnecting...");
    connectToWiFi();
  }
}

void handleRoot(AsyncWebServerRequest* request) {

 // if (!request->authenticate(www_username, www_password)) {
    //Serial.println("Web Server: Unauthorized Acces !");
  //  return request->requestAuthentication();
 // }
  // Serial.println("Web Server: home page");
  request->send(SD, "/html/index.html", "text/html");
}

void handleNotFound(AsyncWebServerRequest* request) {
  // Serial.println("Web Server: error 404 page");
  request->send(SD, "/html/error_404.html", "text/html");
}

void handleSettings(AsyncWebServerRequest* request) {

  if (!request->authenticate(settings_username, settings_password)) {
    //Serial.println("Web Server: Unauthorized Acces !");
    return request->requestAuthentication();
  }
  // Serial.println("Web Server: Settings page");
  request->send(SD, "/html/settings.html", "text/html");
}
/*
void handleError(AsyncWebServerRequest* request) {
  Serial.println("Web Server: error 405 page");
  request->send(SD, "/html/error_405.html", "text/html");
}
*/
void handleFileRequest(AsyncWebServerRequest* request) {
  // Serial.println("Handling file request");
  if (request->hasParam("name")) {
    String filename = request->getParam("name")->value();
    // Serial.println("Requested file: " + filename);
    String fileContentHTML = readFileContent(filename);
    request->send(200, "text/html", fileContentHTML);
  } else {
    //  Serial.println("Missing 'name' parameter");
    request->send(SD, "/html/error_404.html", "text/html");
  }
}

void handleDirRequest(AsyncWebServerRequest* request) {
  // Serial.println("Handling directory request");
  if (request->hasParam("dir")) {
    String dirPath = request->getParam("dir")->value();
    // Serial.println("Requested directory: " + dirPath);
    String directoryHTML = listDirectory(dirPath);
    request->send(200, "text/html", directoryHTML);
  } else {
    // Serial.println("Missing 'dir' parameter");
    request->send(SD, "/html/error_404.html", "text/html");
  }
}

void handleDownload(AsyncWebServerRequest* request) {
  if (!SD.begin(PIN_SPI_CS)) {
    request->send(SD, "/html/error_404.html", "text/html");
    return;
  }
  if (!request->hasParam("name")) {
    request->send(SD, "/html/error_404.html", "text/html");
    return;
  }
  String fileName = request->getParam("name")->value();
  // Serial.print("Requested file for download: ");
  // Serial.println(fileName);
  File file = SD.open(fileName, FILE_READ);
  if (!file) {
    //  Serial.println("Failed to open file: " + fileName);
    request->send(SD, "/html/error_404.html", "text/html");
    return;
  }
  // Serial.println("File opened successfully");
  // Serial.print("File size: ");
  // Serial.println(file.size());
  // Check if file size is greater than 0
  if (file.size() > 0) {
    AsyncWebServerResponse* response = request->beginChunkedResponse("application/octet-stream", [file](uint8_t* buffer, size_t maxLen, size_t index) mutable -> size_t {
      size_t bytesRead = file.read(buffer, maxLen);
      if (bytesRead == 0) {
        file.close();
      }
      return bytesRead;
    });
    response->addHeader("Content-Disposition", "attachment; filename=\"" + fileName + "\"");
    request->send(response);
    //  Serial.println("File sent successfully");
  } else {
    //  Serial.println("File is empty");
    request->send(SD, "/html/error_404.html", "text/html");
    file.close();
  }
  // Serial.println("File closed");
}
// Function to list a directory and handle subdirectories
String listDirectory(String path) {
  if (!SD.begin(PIN_SPI_CS)) {
    // Serial.println("SD Card initialization failed");
    return "SD Card initialization failed";
  }
  File root = SD.open(path);
  if (!root) {
    // Serial.println("Failed to open directory: " + path);
    return "Failed to open directory: " + path;
  }
  // Get SD card total and free space
  uint64_t totalSpace = getSDCardTotalSpace();
  uint64_t freeSpace = getSDCardFreeSpace();

  String directoryListing = "<html lang=\"en\"><head>";
  directoryListing += "<title>Electric-Group production monitor</title>";
  directoryListing += "<link rel=\"icon\" href=\"/html/egricon.ico\" type=\"image/x-icon\">";
  directoryListing += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  directoryListing += "<style>";
  directoryListing += "body {font-size: 24px; background-color: #121212; color: #ffffff;}";
  directoryListing += "a {text-decoration: none; color: #bb86fc; margin-right: 40px;}";
  directoryListing += "a:hover {color: #ffffff;}";
  directoryListing += ".center {display: flex; justify-content: center;}";
  directoryListing += "</style></head><body><h1>Directory: " + path + "</h1>";
  // Add SD card space information
  directoryListing += "<p>SD Card Total Capacity: <b>" + String(totalSpace) + "</b> Mb || SD Card Free Space: <b>" + String(freeSpace) + "</b> Mb</p>";
  // Add link to homepage
  // Centering links
  directoryListing += "<div class='center'>";
  directoryListing += "<a href=\"/\"><b>Home page</b></a>";
  // Add << Back link if not in the /log root directory
  if (path != "/log" && path.startsWith("/log")) {
    String parentPath = "/log";
    int lastSlashIndex = path.lastIndexOf('/');
    if (lastSlashIndex > 4) {
      // Ensure we don't go back beyond /log
      parentPath = path.substring(0, lastSlashIndex);
    }
    directoryListing += "<a href=\"/dir?dir=" + parentPath + "\"><b>Parent directory</b></a>";
  }
  directoryListing += "</div>";
  directoryListing += "<ul>";
  // List directories first
  File entry;
  while ((entry = root.openNextFile())) {
    if (entry.isDirectory()) {
      directoryListing += "<li><b><img src=\"/html/folder_icon.png\" alt=\"Folder\"><a href=\"/dir?dir=" + path + "/" + entry.name() + "\"> [ " + entry.name() + " ]</a></b></li>\n";
    }
    entry.close();
  }
  // Reopen root to list files
  root.rewindDirectory();
  while ((entry = root.openNextFile())) {
    if (!entry.isDirectory()) {
      directoryListing += "<li><img src=\"/html/file_icon.png\" alt=\"File\"><a href=\"/file?name=" + path + "/" + entry.name() + "\">  " + entry.name() + "</a> <a href=\"/download?name=" + path + "/" + entry.name() + "\" download>- download</a></li>\n";
    }
    entry.close();
  }
  directoryListing += "</ul></body></html>";
  root.close();
  return directoryListing;
}

// Function to get total space of SD card in MB
uint64_t getSDCardTotalSpace() {
  uint64_t totalBytes = SD.totalBytes();
  return totalBytes / (1024 * 1024);  // Convert to MB
}
// Function to get used space of SD card in MB
uint64_t getSDCardUsedSpace() {
  uint64_t usedBytes = SD.usedBytes();
  return usedBytes / (1024 * 1024);  // Convert to MB
}
// Function to get free space of SD card in MB
uint64_t getSDCardFreeSpace() {
  uint64_t totalBytes = SD.totalBytes();
  uint64_t usedBytes = SD.usedBytes();
  uint64_t freeBytes = totalBytes - usedBytes;
  return freeBytes / (1024 * 1024);  // Convert to MB
}

// Function to read file content
String readFileContent(String filename) {
  if (!SD.begin(PIN_SPI_CS)) {
    //  Serial.println("SD Card initialization failed");
    return "SD Card initialization failed";
  }
  File file = SD.open(filename);
  if (!file) {
    //  Serial.println("Failed to open file: " + filename);
    return "Failed to open file: " + filename;
  }
  // Extract parent directory from filename
  int lastSlashIndex = filename.lastIndexOf('/');
  String parentPath = (lastSlashIndex > 0) ? filename.substring(0, lastSlashIndex) : "/log";
  String fileContent = "<html lang=\"en\"><head>";
  fileContent += "<title>Electric-Group production monitor</title>";
  fileContent += "<link rel=\"icon\" href=\"/html/egricon.ico\" type=\"image/x-icon\">";
  fileContent += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  fileContent += "<style>";
  fileContent += "body {font-size: 24px; background-color: #121212; color: #ffffff;}";
  fileContent += "pre {font-size: 24px; color: #ffffff; background-color: #212121; padding: 10px; border-radius: 5px;}";
  fileContent += "a {text-decoration: none; color: #bb86fc; margin-right: 40px;}";
  fileContent += "a:hover {color: #ffffff;}";
  fileContent += "div {text-align: center; margin-top: 20px;}";
  fileContent += "</style></head><body>";
  fileContent += "<div><p><a href=\"/\"><b>Home page</b></a>";
  if (filename.startsWith("/log") && filename != "/log") {
    fileContent += "<a href=\"/dir?dir=" + parentPath + "\"><b>Back</b></a>";
  }
  fileContent += "</p></div>";
  fileContent += "<h1>File: " + filename + "</h1><pre>";
  while (file.available()) {
    fileContent += char(file.read());
  }
  fileContent += "</pre></body></html>";
  file.close();
  return fileContent;
}

void setup() {
  pinMode(GREEN_LED_PIN, OUTPUT);  // set ESP32 pin to output mode
  pinMode(RED_LED_PIN, OUTPUT);    // set ESP32 pin to output mode
  pinMode(JDY_CSS, OUTPUT);        // set ESP32 pin to output mode
  pinMode(JDY_SET, OUTPUT);        // set ESP32 pin to output mode
  digitalWrite(JDY_CSS, LOW);
  digitalWrite(JDY_SET, HIGH);
  digitalWrite(RED_LED_PIN, HIGH);
  //delay(5000);
  Serial.begin(115200);
  // Serial.flush();
  mySerial.setRxTimeout(1);
  mySerial.setRxBufferSize(65);
  mySerial.setTxBufferSize(65);
  mySerial.onReceive(mySerial2RX);
  mySerial.begin(38400, SERIAL_8N1, RXD2, TXD2);  // UART setup
                                                  // Serial.printf("CPU Freq = %i Mhz\n", getCpuFrequencyMhz());
                                                  // Serial.printf("XTAL Freq = %i Mhz\n", getXtalFrequencyMhz());
                                                  // Serial.println("janos.szabo@gmail.com");
                                                  // Serial.println("janos.szabo@electricgroup.ro");
  WiFi.setHostname("esp32-server");
  init_sd();

  init_AHT15();

  connectToWiFi();

  synchronizeTime();

  // Web Server init
  server.on("/", HTTP_GET, handleRoot);
  server.on("/settings", HTTP_GET, handleSettings);
  //  server.on("/error", HTTP_GET, handleError);
  server.on("/file", HTTP_GET, handleFileRequest);
  server.on("/dir", HTTP_GET, handleDirRequest);
  server.on("/download", HTTP_GET, handleDownload);
  server.onNotFound(handleNotFound);
  server.serveStatic("/", SD, "/");
  server.begin();

  initWebSocket();

  // Serial.println("ESP32 Web server ready!");

  ftpSrv.begin("TEST", "TEST");  //username, password for ftp.  set ports in ESP8266FtpServer.h  (default 21, 50009 for PASV)

  // Serial.println("FTP file server ready!");

  // Configure the timer to trigger every 10ms (10000 microseconds)
  // const esp_timer_create_args_t periodic_timer2_args = { .callback = &timerCallback2, .name = "periodic_timer2" };
  // esp_timer_create(&periodic_timer2_args, &periodic_timer2);
  // esp_timer_start_periodic(periodic_timer2, 10 * 1000);  // 10ms in microseconds
  /*
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  */
}

void init_AHT15() {
  unsigned long startTime = millis();
  unsigned long timeout = 30000;  // 30 seconds timeout

  while (aht10.begin() != true) {
    // Serial.println("AHT15 not connected or fail to load calibration coefficient");

    if (millis() - startTime >= timeout) {
      // Serial.println("AHT15 Sensor broken or not connected!");
      aht_flag = false;
      temp = "Sensor malfunction!";
      hum = "Sensor malfunction!";
      digitalWrite(RED_LED_PIN, HIGH);
      return;
    }

    delay(5000);
  }
  // Serial.println("AHT15 sensor initialized!");
  aht_flag = true;
}


void init_sd() {
  // init MicroSD Card
  if (!SD.begin(PIN_SPI_CS)) {
    // Serial.println("SD Card initialization failed");
    digitalWrite(RED_LED_PIN, HIGH);
    return;
  }
  //Serial.println("SD Card initialized!");
  // Create the /log directory if it doesn't exist
  if (!SD.exists("/log")) {
    if (SD.mkdir("/log")) {
      // Serial.println("Created /log directory");
    } else {
      //  Serial.println("Failed to create /log directory");
      digitalWrite(RED_LED_PIN, HIGH);
    }
  }
}

void loop() {
  unsigned long currentMillis = millis();
  // Handle LED blinking logic
  if (((currentMillis - lastTime2) >= timerDelay2) && (srvclients > 0)) {
    lastTime2 = currentMillis;
    led_state = !led_state;
    digitalWrite(GREEN_LED_PIN, led_state);
  }
  if (((currentMillis - lastTime3) >= timerDelay3) && (srvclients == 0) && (WiFi.status() == WL_CONNECTED)) {
    lastTime3 = currentMillis;
    led_state = !led_state;
    digitalWrite(GREEN_LED_PIN, led_state);
    digitalWrite(RED_LED_PIN, LOW);
  }
  /*
  // Handle mySerial input
  if (mySerial.available()) {
    String radiomessage = mySerial.readStringUntil('\n');
    const int maxParts = 9;
    String parts[maxParts];

    if (verifyCRC8(radiomessage)) {
      // Serial.println("Radio RX: " + radiomessage);
      extractSubstrings(radiomessage, parts, maxParts);
      processRadioMessage(parts);
      digitalWrite(RED_LED_PIN, LOW);
    } else {
      digitalWrite(RED_LED_PIN, HIGH);
      Serial.println("Radio Rx message discarded.");
      sendData("RX error-----------------");
    }
  }
  */
  // Handle new data if available
  if (newData) {
    handleSerialData();
    newData = false;  // Reset flag after handling data
  }

  // Handle line data
  handleLineData(currentMillis);

  // Handle line timeout
  handleLineTimeouts(currentMillis);

  // Check WiFi connection
  if ((currentMillis - wifi_previousMillis) >= wifi_interval) {
    wifi_previousMillis = currentMillis;
    checkWiFiConnection();
    if (!timesynch_ntp) {
      ntptimesynch();
    }
  }

  // Check NTP time connection
  if ((currentMillis - ntp_previousMillis) >= ntp_interval) {
    ntp_previousMillis = currentMillis;
    ntptimesynch();
  }

  // Handle AHT15 sensor readings
  if (((currentMillis - aht_previousMillis) >= 10000) && aht_flag) {
    aht_previousMillis = currentMillis;

    float temperature = aht10.readTemperature(AHTXX_FORCE_READ_DATA);  // Takes around 80 milliseconds;
    temp = String(temperature, 2) + "°C";

    float humidity = aht10.readHumidity(AHTXX_USE_READ_DATA);
    hum = String(humidity, 2) + "%";
  }

  // Cleanup and handle FTP
  ws.cleanupClients();
  ftpSrv.handleFTP();
}

void mySerial2RX() {
  while (mySerial.available()) {
    incoming = mySerial.read();
    if (incoming == '\n') {
      buffer[indexr] = '\0';  // Null-terminate the buffer
      newData = true;
      // Reset the buffer for the next message
      indexr = 0;
      return;
    } else if (indexr < bufferSize - 1) {
      buffer[indexr++] = incoming;  // Add character to buffer
    } else {
      indexr = 0;
    }
  }
}

// Function to handle the serial data
void handleSerialData() {
  String radiomessage = String(buffer);
  //Serial.println("Radio RX: " + radiomessage);
  const int maxParts = 9;
  String parts[maxParts];

  if (radiomessage == "+OK\r") {
    return;
  }

  // Verify CRC8 checksum
  if (verifyCRC8(radiomessage)) {

    // Extract substrings from the radio message
    extractSubstrings(radiomessage, parts, maxParts);

    // Process the extracted message parts
    processRadioMessage(parts);

    // Turn off the RED LED
    digitalWrite(RED_LED_PIN, LOW);
  } else {
    // Turn on the RED LED to indicate an error
    digitalWrite(RED_LED_PIN, HIGH);
    //Serial.println("Radio RX: message discarded.");
    //mySerial.print("Radio RX: " + radiomessage);
    //mySerial.println("->>> message discarded.");
  }
}

void processRadioMessage(String parts[]) {
  if ((parts[0] == "L1LIVE") || (parts[0] == "L1LIVESTART") || (parts[0] == "L1LIVESTOP")) {
    l1type = parts[1];
    l1color = parts[2];
    l1counter = parts[3];
    l1speed = parts[4];
    l1dia = parts[5];
    l1load60 = parts[6];
    l1load32 = parts[7];
    startTime_l1 = millis();
    l1_alive = true;
  }
  if (parts[0] == "L1LIVESTART") {
    logDataLine("Started:\t" + l1type + "\t" + l1color + "\t -> " + l1counter + "\t" + "starting...", "Production Line 60");
  }
  if (parts[0] == "L1LIVESTOP") {
    logDataLine("Finished:\t" + l1type + "\t" + l1color + "\t -> " + l1counter + "\t" + "TOTAL METER", "Production Line 60");
  }
  if ((parts[0] == "L2LIVE") || (parts[0] == "L2LIVESTART") || (parts[0] == "L2LIVESTOP")) {
    l2type = parts[1];
    l2color = parts[2];
    l2counter = parts[3];
    l2speed = parts[4];
    l2dia = parts[5];
    l2load45 = parts[6];
    l2load32 = parts[7];
    startTime_l2 = millis();
    l2_alive = true;
  }
  if (parts[0] == "L2LIVESTART") {
    logDataLine("Started:\t" + l2type + "\t" + l2color + "\t -> " + l2counter + "\t" + "starting...", "Production Line 45");
  }
  if (parts[0] == "L2LIVESTOP") {
    logDataLine("Finished:\t" + l2type + "\t" + l2color + "\t -> " + l2counter + "\t" + "TOTAL METER", "Production Line 45");
  }
  if ((parts[0] == "L3LIVE") || (parts[0] == "L3LIVESTART") || (parts[0] == "L3LIVESTOP")) {
    l3type = parts[1];
    l3color = parts[2];
    l3counter = parts[3];
    l3speed = parts[4];
    l3dia = parts[5];
    l3load80 = parts[6];
    l3load40 = parts[7];
    startTime_l3 = millis();
    l3_alive = true;
  }
  if (parts[0] == "L3LIVESTART") {
    logDataLine("Started:\t" + l3type + "\t" + l3color + "\t -> " + l3counter + "\t" + "starting...", "Production Line 80");
  }
  if (parts[0] == "L3LIVESTOP") {
    logDataLine("Finished:\t" + l3type + "\t" + l3color + "\t -> " + l3counter + "\t" + "TOTAL METER", "Production Line 80");
  }
}

void handleLineData(unsigned long currentMillis) {
  if ((currentMillis - startTime_ms) >= msDuration) {
    startTime_ms = currentMillis;
    ms_counter++;  // Increment by 1, assuming msDuration is 1 millisecond

    if (ms_counter == 200 && !sentLine1) {  // Do something at 200ms
      sendData("*LINE1");
      sentLine1 = true;
    } else if (ms_counter == 500 && !sentLine2) {  // Do something at 500ms
      sendData("*LINE2");
      sentLine2 = true;
    } else if (ms_counter == 800 && !sentLine3) {  // Do something at 800ms
      sendData("*LINE3");
      sentLine3 = true;
    } else if (ms_counter >= 1000) {  // Reset the counter every second
      ms_counter = 0;
      sentLine1 = false;
      sentLine2 = false;
      sentLine3 = false;
    }
  }
}

void sendData(String id) {
  getLocalTime(&timeinfo);
  char timeString[50];
  strftime(timeString, sizeof(timeString), "%d/%m,%H:%M:%S", &timeinfo);
  // Create a data string with all the information
  String stemp = temp.substring(0, 4);
  String shum = hum.substring(0, 4);
  //String dataString = id + "," + String(timeString) + "," + ip + "," + temp + "," + hum + ",";
  String dataString = id + "," + String(timeString) + "," + ip + "," + stemp + "," + shum + ",";
  // Calculate CRC8 checksum using the FastCRC library
  uint8_t crc = CRC8.smbus((uint8_t*)dataString.c_str(), dataString.length());
  // Convert CRC8 byte to a two-character hexadecimal string
  char crcHex[3];
  sprintf(crcHex, "%02X", crc);
  // Append the CRC hexadecimal string to the data string
  dataString += String(crcHex);
  dataString += "\r\n";
  // Send data string character by character using Serial.write
  mySerial.print(dataString);
  /*
  for (unsigned int i = 0; i < dataString.length(); i++) {
    mySerial.write(dataString[i]);
  }
  // Optionally send a newline character at the end of the data string
  mySerial.write('\r');
  mySerial.write('\n');
  */
  // mySerial.flush();  // Flush the buffer after sending
}

void handleLineTimeouts(unsigned long currentMillis) {
  if (l1_alive) {
    l1_alive = false;
  } else {
    if ((currentMillis - startTime_l1) >= timeoutDuration) {
      l1type = "OFF-LINE";
      l1color = "OFF-LINE";
      l1counter = "OFF-LINE";
      l1speed = "OFF-LINE";
      l1dia = "OFF-LINE";
      l1load60 = "OFF-LINE";
      l1load32 = "OFF-LINE";
      startTime_l1 = currentMillis;
    }
  }
  if (l2_alive) {
    l2_alive = false;
  } else {
    if ((currentMillis - startTime_l2) >= timeoutDuration) {
      l2type = "OFF-LINE";
      l2color = "OFF-LINE";
      l2counter = "OFF-LINE";
      l2speed = "OFF-LINE";
      l2dia = "OFF-LINE";
      l2load45 = "OFF-LINE";
      l2load32 = "OFF-LINE";
      startTime_l2 = currentMillis;
    }
  }
  if (l3_alive) {
    l3_alive = false;
  } else {
    if ((currentMillis - startTime_l3) >= timeoutDuration) {
      l3type = "OFF-LINE";
      l3color = "OFF-LINE";
      l3counter = "OFF-LINE";
      l3speed = "OFF-LINE";
      l3dia = "OFF-LINE";
      l3load80 = "OFF-LINE";
      l3load40 = "OFF-LINE";
      startTime_l3 = currentMillis;
    }
  }
}

bool verifyCRC8(const String& receivedString) {
  if (receivedString.length() < 3) {
    // The string is too short to contain data and a CRC
    return false;
  }

  // Extract the data part of the string
  String data = receivedString.substring(0, receivedString.length() - 3);

  // Extract the received CRC8 (last 2 characters of the string)
  //String receivedCRC8Str = receivedString.substring(receivedString.length() - 3);
  String receivedCRC8Str = receivedString.substring(receivedString.length() - 3, receivedString.length() - 1);

  uint8_t receivedCRC8 = strtol(receivedCRC8Str.c_str(), NULL, 16);

  // Calculate the CRC8 of the data part using FastCRC
  uint8_t calculatedCRC8 = CRC8.smbus((uint8_t*)data.c_str(), data.length());

  // Compare the calculated CRC8 with the received CRC8
  return (receivedCRC8 == calculatedCRC8);
}


// Function to extract substrings between delimiters and store them in provided variables
void extractSubstrings(const String& str, String* parts, int maxParts) {
  char delimiter = ',';  // Declare and initialize the delimiter here
  String temp = "";
  int partIndex = 0;

  // Ensure parts array is cleared or initialized
  for (int i = 0; i < maxParts; i++) {
    parts[i] = "";
  }

  for (int i = 0; i < str.length(); i++) {
    if (str[i] == delimiter) {
      if (partIndex < maxParts) {
        parts[partIndex] = temp;
        partIndex++;
        temp = "";
      } else {
        break;
      }
    } else {
      temp += str[i];
    }
  }

  if (partIndex < maxParts) {
    parts[partIndex] = temp;
  }
}

void logDataLine(String data, String customFilename) {
  if (!timesynch_ntp) {
    //Serial.println("Failed to synchronize time! Data logging disabled!");
    return;
  }

  //struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time!");
    return;
  }

  const char* monthNames[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
  char path[64];
  snprintf(path, sizeof(path), "/log/%04d/%s/%02d/%s.log", timeinfo.tm_year + 1900, monthNames[timeinfo.tm_mon], timeinfo.tm_mday, customFilename.c_str());

  // Create directories if they don't exist
  String yearDir = "/log/" + String(timeinfo.tm_year + 1900);
  String monthDir = yearDir + "/" + String(monthNames[timeinfo.tm_mon]);
  char dayDir[16];
  snprintf(dayDir, sizeof(dayDir), "/%02d", timeinfo.tm_mday);  // Ensure day has leading zero

  String fullDayDir = monthDir + String(dayDir);

  // Combine directory creation into one loop
  String dirPaths[] = { yearDir, monthDir, fullDayDir };

  for (String dir : dirPaths) {
    if (!SD.exists(dir)) {
      if (SD.mkdir(dir.c_str())) {
        // Serial.println("Created directory: " + dir);
      } else {
        // Serial.println("Failed to create directory: " + dir);
      }
    }
  }

  // Open file for appending
  File file = SD.open(path, FILE_APPEND);
  if (!file) {
    // Serial.println("Failed to open file for writing");
    return;
  }

  char timestamp[32];
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &timeinfo);
  file.printf("%s - %s\n", timestamp, data.c_str());
  file.close();
  //Serial.println("Data logged: " + String(timestamp));
}

void ntptimesynch() {
  if (WiFi.status() == WL_CONNECTED) {
    if (Ping.ping(ntpServer)) {
      //struct tm timeinfo;
      configTime(gmtOffset_sec, 0, ntpServer);
      // Serial.println("Time synched with: pool.ntp.org");

      if (getLocalTime(&timeinfo)) {
        // Serial.print("Current Time&Date: ");
        //  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

        // Adjust for DST: Last Sunday in March to the last Sunday in October
        int totalOffset_sec = gmtOffset_sec;
        if ((timeinfo.tm_mon > 2 && timeinfo.tm_mon < 9) || (timeinfo.tm_mon == 2 && (timeinfo.tm_mday - timeinfo.tm_wday) >= 25) || (timeinfo.tm_mon == 9 && (timeinfo.tm_mday - timeinfo.tm_wday) < 25)) {
          totalOffset_sec += dstOffset_sec;
        }

        // Re-configure time with DST adjustment
        configTime(totalOffset_sec, 0, ntpServer);
        if (getLocalTime(&timeinfo)) {
          //  Serial.print("Adjusting Time&Date for DST: ");
          //  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        } else {
          // Serial.println("Failed to get local time after DST adjustment.");
        }
        timesynch_ntp = true;
      } else {
        // Serial.println("Failed to get local time.");
        if (!timesynch_ntp) {
          timesynch_ntp = false;
        }
      }
    } else {
      //  Serial.println("Time server pool.ntp.org is not available!");
      if (!timesynch_ntp) {
        timesynch_ntp = false;
      }
    }
  } else {
    // Serial.println("Internet connection is not available!");
    if (!timesynch_ntp) {
      timesynch_ntp = false;
    }
  }
}
