#include "secrets.h" // wifi credentials
#include <Wire.h>
#include <WiFi.h>
#include <time.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>

Adafruit_BMP085 bmp;

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0; // keep UTC convert to IST in front-end
const int daylightOffset_sec = 0;

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi ...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void initTime() {
  struct tm timeinfo;

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Synchronizing time...");

  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for NTP time...");
    delay(1000);
  }

  Serial.println("Time synchornized.");
}

String getTimeStamp() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time.");
    return "";
  }

  char timestamp[25];

  strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);

  return String(timestamp);
}

void sendJson(float temp, float pressure) {
  JsonDocument doc;

  doc["timestamp"] = getTimeStamp(); // TODO: reject empty timestamps
  doc["temperature"] = temp;
  doc["pressure"] = pressure / 100; // hPa

  serializeJson(doc, Serial); // debug
  Serial.println();

  // TODO: Send telemetry to Azure IoT Hub
}

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  bool status;
  status = bmp.begin(0x77);
  if (!status) {
    Serial.println("Could not find valid BMP180 sensor, check wiring.");
    while(1);
  }

  initWifi();
  initTime();
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();

  sendJson(temp, pressure);

  delay(2000);
}
