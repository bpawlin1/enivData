
#include "Adafruit_SHT4x.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

Adafruit_SHT4x sht4 = Adafruit_SHT4x();

Adafruit_BMP280 bmp; // I2C

float humidityReading;
float bmpTemp;
float bmpPressure;
float bmpAltitude;


void setup() {
  Serial.begin(115200);

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

void loop() {
  readSHT();
  
  readBMP();

  Serial.println(bmpTemp);
}

