// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <M5GFX.h>
M5GFX display;

#include "DHT.h"

#define DHTPIN 26     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

// Battery stuff
#include "AXP192.h"
#include <M5StickC.h>

float maxtemp=0;
float mintemp=999;

void setup() {
  M5.begin();

  Serial.begin(9600);

  dht.begin();
  display.begin();
  display.setRotation(display.getRotation() ^ 3);
}

void loop() {
  delay(1000);
  display.setTextSize(2);
  display.drawString("Temp C", 88, 1);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  float temp = dht.readTemperature();
  Serial.print("Temp - ");
  Serial.println(dht.readTemperature());
  display.setTextSize(7);
  display.drawFloat(dht.readTemperature(),1, 5, 40);


  if (temp > maxtemp) {
    maxtemp=temp; 
    display.setTextSize(2);
    display.drawString("+", 130, 110);
    display.drawFloat(maxtemp,1, 150, 110);
    }
  if (temp < mintemp) {
    mintemp=temp; 
    display.setTextSize(2);
    display.drawString("-", 5, 110);
    display.drawFloat(mintemp,1, 25, 110);
    }

  int x = M5.Axp.GetBatVoltage();
  float sensorMin = 2.10;  // minimum value what sensor measure on analogRead
  float sensorMax = 4.00; // max value measured on analogRead
  float sensorValue = fmap(x, sensorMin, sensorMax,0 ,100 );

  Serial.println(sensorValue);
  display.setTextSize(2);
//  display.drawFloat(sensorValue,0,20,5);
//  display.drawString("%", 5, 5);
  display.drawString("V", 170, 5);
  display.drawFloat(M5.Axp.GetBatVoltage(),2,185,5);

}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
