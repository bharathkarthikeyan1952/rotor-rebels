#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <TFLuna.h>
#include "DHT.h"


const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";


#define DHTPIN 4
#define DHTTYPE DHT22
#define SMOKE_PIN 34

TFLuna tfluna;
DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);


uint16_t dist = 0, str = 0;
float ambTemp = 0, gasPer = 0;

void handleRoot() {
 
  String html = "<html><head><meta http-equiv='refresh' content='2'></head><body>";
  html += "<h1>ESP32 Sensor Station</h1>";
  html += "<p><b>Lidar Distance:</b> " + String(dist) + " cm</p>";
  html += "<p><b>Temperature:</b> " + String(ambTemp) + " &deg;C</p>";
  html += "<p><b>Gas Level:</b> " + String(gasPer) + " %</p>";
  if(gasPer > 60) html += "<h2 style='color:red;'>WARNING: GAS DETECTED!</h2>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  dht.begin();


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP Address: ");
  Serial.println(WiFi.localIP()); 

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient(); 
 
  float chipTemp;
  tfluna.getRaw(dist, str, chipTemp);

 
  float t = dht.readTemperature();
  if (!isnan(t)) ambTemp = t;

  
  int gasValue = analogRead(SMOKE_PIN);
  gasPer = (gasValue / 4095.0) * 100.0;

  delay(100); 
}