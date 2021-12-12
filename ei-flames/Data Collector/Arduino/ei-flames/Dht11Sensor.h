#ifndef DHT11SENSOR_H
#define DHT11SENSOR_H

#include <Arduino.h>
#include "DHT.h"

/* 
 * A sensor to measure the temperature and humidity of the environment.
 * interfaces on the dht.DHT11 environment sensor.
 * new values are measured every 1000 ms.
 * last read values are provided in the meantime.
 */
class Dht11Sensor {
  unsigned long dhtLastMillis = 0;
  int sensorPin = 7;
  DHT *dht;
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
