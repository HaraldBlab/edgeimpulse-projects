#ifndef FLAMESENSOR_H
#define FLAMESENSOR_H

#include <Arduino.h>

/* 
 * A sensor to measure the (flame) heat of the environment
 * high values indicate not a (flame) heat
 * low values indicate a (flame) heat
 * trim poti adjusts around value 500 only
 */
class FlameSensor {
  int sensorPin = 7;
  int potiPin = A0;
  int potiValue;
  public:
  FlameSensor(int poti, int sensor);
  void setup();
  int value();
  bool detect();
};

#endif
