#ifndef DHT11SENSOR_H
#define DHT11SENSOR_H

#include <Arduino.h>
#if defined(__AVR__)
#include "DHT.h"
#elif defined(ESP8266)
#include "DHTesp.h"
#endif

/* 
 * A sensor to measure the temperature and humidity of the environment.
 * interfaces on the dht.DHT11 environment sensor.
 * new values are measured every 1000 ms.
 * last read values are provided in the meantime.
 */
class Dht11Sensor {
  unsigned long dhtLastMillis = 0;
  int sensorPin = 7;
#if defined(__AVR__)
  DHT *dht;
#elif defined(ESP8266)
  DHTesp *dht;
#endif
  public:
  float dhtTemperature;
  float dhtHumidity;
  Dht11Sensor(int sensor);
  ~Dht11Sensor();
  void setup();
  float temperature();
  float humidity();
  private:
  void read();
};
#endif
