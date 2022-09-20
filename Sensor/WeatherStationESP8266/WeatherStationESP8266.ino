
 
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Adafruit_SHT4x.h"
#include "Adafruit_PM25AQI.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>
//#include "SdsDustSensor.h"
#include "Adafruit_LC709203F.h"

Adafruit_LC709203F lc;
const char* ssid = "wifi";  // Enter SSID here
const char* password = "Bandit2015_69236";
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();
 
ESP8266WebServer server(80);

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

Adafruit_BMP280 bmp; // I2C

float humidityReading = 0;
float bmpTemp = 0;
float bmpPressure = 0;
float bmpAltitude = 0;
int pm25Reading = 0;
int pm10Reading = 0;
float batt_volt = 0;
float batt_perc = 0;
float batt_temp = 0;




void getSensorData() {

  readPM25();
  readBMP();
  battery();
  readSHT();
  
  uint32_t timestamp = millis();


     DynamicJsonDocument doc(512);
     doc["Location"] = "Outdoor Weather Station";
    doc["humidityReading"] = humidityReading;
    doc["bmpTemp"] = bmpTemp;
    doc["bmpPressure"] = bmpPressure;
    doc["bmpAltitude"] = bmpAltitude;
    doc["pm25Reading"] = pm25Reading;
    doc["pm10Reading"] = pm10Reading;
    doc["batt_volt"] = batt_volt;
    doc["batt_perc"] = batt_perc;
    doc["batt_temp"] = batt_temp;
 
 
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
  
  readPM25();
  readBMP();
  battery();
  readSHT();
  
  server.send(200, "text/html", SendHTML(bmpTemp,humidityReading,bmpPressure,bmpAltitude,pm25Reading,pm10Reading,batt_volt,batt_perc,batt_temp)); 
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

String SendHTML(float bmpTemp,float humidityReading,float bmpPressure,float bmpAltitude, float pm25Reading, float pm10Reading, float batt_volt, float batt_perc, float batt_temp){
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
  ptr +="<p>Battery Volt: ";
  ptr +=batt_volt;
  ptr +="</p>";
  ptr +="<p>Battery Percent: ";
  ptr +=batt_perc;
  ptr +="</p>";
  ptr +="<p>Battery Temp: ";
  ptr +=batt_temp;
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
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
    sht4.setHeater(SHT4X_NO_HEATER);

  if (!lc.begin()) {
    Serial.println(F("Couldnt find Adafruit LC709203F?\nMake sure a battery is plugged in!"));
    while (1) delay(10);
  }
  Serial.println(F("Found LC709203F"));
  Serial.print("Version: 0x"); Serial.println(lc.getICversion(), HEX);

  lc.setThermistorB(3950);
  Serial.print("Thermistor B = "); Serial.println(lc.getThermistorB());
  lc.setPackSize(LC709203F_APA_3000MAH);
  lc.setAlarmRSOC(25);

  

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
   
   // There are 3 options for connectivity!
  if (! aqi.begin_I2C()) { 
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
  Serial.println("PM25 found!");
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

void battery() {
  Serial.print("Batt Voltage: "); Serial.println(lc.cellVoltage(), 3);
  Serial.print("Batt Percent: "); Serial.println(lc.cellPercent(), 1);
  Serial.print("Batt Temp: "); Serial.println(lc.getCellTemperature(), 1);
  batt_volt = lc.cellVoltage();
  batt_perc = lc.cellPercent();
  batt_temp = lc.getCellTemperature();
  delay(2000);  // dont query too often!
}

void readBMP ()
{
    bmpTemp = bmp.readTemperature()* 9/5 + 32;
    bmpPressure = bmp.readPressure() / 3386.39;
    bmpAltitude = bmp.readAltitude(1013.25); /* Adjusted to local forecast! */

    delay(2000);
}

void readPM25 ()
{
  PM25_AQI_Data data;
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  Serial.println("AQI reading success");

  pm25Reading = data.pm25_env;
  pm10Reading = data.pm100_env;
  

  delay(1000);


}
