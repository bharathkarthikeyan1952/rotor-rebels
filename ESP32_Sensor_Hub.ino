#include <Wire.h>
#include <TFLuna.h>
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22
#define SMOKE_PIN 34
TFLuna tfluna;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin(); // ESP32 default I2C (SDA: 21, SCL: 22)
  dht.begin();
  
  Serial.println("--- System Online ---");
}

void loop() {
  uint16_t distance = 0;
  uint16_t strength = 0;
  float chipTemp = 0;
  if (tfluna.getRaw(distance, strength, chipTemp)) {
    Serial.print("Dist: ");
    Serial.print(distance); 
    Serial.print("cm | Sig: ");
    Serial.print(strength); // Tells you if the surface is reflective enough
    Serial.print(" | ");
  } else {
    Serial.print("Lidar Error | ");
  }

 
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    Serial.print("Ambient: ");
    Serial.print(temp);
    Serial.print("°C | ");
  }


  int gasValue = analogRead(SMOKE_PIN);
  float gasPercentage = (gasValue / 4095.0) * 100.0; // ESP32 has 12-bit ADC (0-4095)

  Serial.print("Gas: ");
  Serial.print(gasPercentage);
  Serial.println("%");

  // SAFETY LOGIC
  if (gasValue > 2500) { // Adjust threshold based on your environment
    Serial.println(">> ALERT: GAS DETECTED! <<");
  }

  delay(500);
}