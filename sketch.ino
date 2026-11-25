#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);

  bool status;

  status = bmp.begin();
  if (!status) {
    Serial.println("Could not find valid BMP180 sensor, check wiring.");
    while(1);
  }
}

void loop() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("°C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println("hPa");

  Serial.println("------------------------------");
  delay(10);
}
