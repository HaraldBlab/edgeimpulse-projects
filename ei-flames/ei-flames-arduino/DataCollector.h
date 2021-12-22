#ifndef DATACOLLECTOR_H
#define DATACOLLECTOR_H

#include <Arduino.h>

class DataCollector {
  float flames;
  float temperature;
  float air;
 public:
  void setup();
  void loop();
  void collect(float *buffer, unsigned int values);
};

#endif
