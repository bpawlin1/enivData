
 
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Adafruit_SHT4x.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>
#include "SdsDustSensor.h"
 
const char* ssid = "wifi";  // Enter SSID here
const char* password = "Bandit2015_69236";
 
ESP8266WebServer server(80);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

Adafruit_BMP280 bmp; // I2C

float humidityReading;
float bmpTemp;
float bmpPressure;
float bmpAltitude;
float pm25Reading;
float pm10Reading;

int rxPin = 2;
int txPin = 14;
SdsDustSensor sds(rxPin, txPin);

// Serving Hello world
void getSensorData() {

  PmResult pm = sds.readPm();

  
  sensors_event_t humidity, temp;
  
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

     DynamicJsonDocument doc(512);
     doc["Location"] = "Outdoor Weather Station";
    doc["humidityReading"] = humidity.relative_humidity;
    doc["bmpTemp"] = bmp.readTemperature() * 9/5 + 32;
    doc["bmpPressure"] = bmp.readPressure() / 3386.39;
    doc["bmpAltitude"] = bmp.readAltitude(1013.25);
    doc["pm25Reading"] = pm.pm25;
    doc["pm10Reading"] = pm.pm10;
 
 
      Serial.print(F("Stream..."));
      String buf;
      serializeJson(doc, buf);
      server.send(200, "application/json", buf);
      Serial.print(F("done."));


}
 
// Define routing
void restServerRouting() {
    server.on("/", handle_OnConnect);
    
    server.on(F("/sensorData"), HTTP_GET, getSensorData);
}

void handle_OnConnect() {

  sensors_event_t humidity, temp;
  PmResult pm = sds.readPm();
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  humidityReading = humidity.relative_humidity;
  bmpTemp = bmp.readTemperature() * 9/5 + 32;
    bmpPressure = bmp.readPressure() / 3386.39;
    bmpAltitude = bmp.readAltitude(1013.25);
    pm25Reading = pm.pm25;
    pm10Reading = pm.pm10;
  
  server.send(200, "text/html", SendHTML(bmpTemp,humidityReading,bmpPressure,bmpAltitude,pm25Reading,pm10Reading)); 
  Serial.println("Connection");
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

String SendHTML(float bmpTemp,float humidityReading,float bmpPressure,float bmpAltitude, float pm25Reading, float pm10Reading){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Weather Station</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Weather Station</h1>\n";
  ptr +="<p>Temperature: ";
  ptr +=bmpTemp;
  ptr +="&deg;f</p>";
  ptr +="<p>Humidity: ";
  ptr +=humidityReading;
  ptr +="%</p>";
  ptr +="<p>Pressure: ";
  ptr +=bmpPressure;
  ptr +="hg</p>";
  ptr +="<p>Altitude: ";
  ptr +=bmpAltitude;
  ptr +="m</p>";
  ptr +="<p>PM2.5: ";
  ptr +=pm25Reading;
  ptr +="</p>";
  ptr +="<p>PM10: ";
  ptr +=pm10Reading;
  ptr +="</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
 
void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 sds.begin();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane esp8266.local
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  server.onNotFound(handleNotFound);
  // Start server
  server.begin();
  Serial.println("HTTP server started");

  while (!Serial)
    delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit SHT4x test");
  if (! sht4.begin()) {
    Serial.println("Couldn't find SHT4x");
    while (1) delay(1);
  }
  Serial.println("Found SHT4x sensor");
  Serial.print("Serial number 0x");
  Serial.println(sht4.readSerial(), HEX);

  // You can have 3 different precisions, higher precision takes longer
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  
  // You can have 6 different heater settings
  // higher heat and longer times uses more power
  // and reads will take longer too!
  sht4.setHeater(SHT4X_NO_HEATER);

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
   

   
 
}
 
void loop(void) {
  server.handleClient();
}

void readSHT ()
{
   sensors_event_t humidity, temp;
  
  uint32_t timestamp = millis();
  sht4.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

  humidityReading = humidity.relative_humidity;

  delay(2000);
  
}

void readBMP ()
{

    bmpTemp = bmp.readTemperature();
    bmpPressure = bmp.readPressure();
    bmpAltitude = bmp.readAltitude(1013.25); /* Adjusted to local forecast! */

    delay(2000);
}