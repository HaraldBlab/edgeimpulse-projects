/*
 * DataCollector used to read tuples from the sensor.
 */
#include "DataCollector.h"
#include "Flamesensor.h"
#include "Dht11Sensor.h"

/* TODO: pins for the WEMOS D1 mini boards */
FlameSensor flameSensor(D2, A0);
Dht11Sensor dhtSensor(D5);

void DataCollector::setup() {
  flameSensor.setup();
  dhtSensor.setup();
}

void DataCollector::loop() {
  flames = (float) flameSensor.value();
  temperature = dhtSensor.temperature();
  air = dhtSensor.humidity();
}

void DataCollector::collect(float *values, unsigned int numValues) {
  int index = 0;

  while (index < numValues-3) {
    loop();
    values[index++] = flames;
    values[index++] = air;
    values[index++] = temperature;
  }
}
