#include <Wire.h>
#include <WiFi.h>
#include <time.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <ArduinoJson.h>

Adafruit_BMP085 bmp;

const char* ssid = "Wokwi-GUEST";
const char* password = "";

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

void sendJson(float temp, float pressure) {
  JsonDocument doc;

  doc["temperature"] = temp;
  doc["pressure"] = pressure / 100; // hPa

  // TODO: Add timestamp

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
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();

  sendJson(temp, pressure);

  delay(2000);
}
