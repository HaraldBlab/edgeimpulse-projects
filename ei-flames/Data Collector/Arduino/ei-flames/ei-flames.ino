/*
 * ei-flames Data collector
 * 
 * Board: LOLIN(WEMOS) D1 R2 & mini
 * CPU Frequence: 160 MHz
 */

#include "Flamesensor.h"
#include "Dht11Sensor.h"

FlameSensor flameSensor(D2, A0);
Dht11Sensor dhtSensor(D5);

void setup() {
  Serial.begin(115200);
  flameSensor.setup();
  dhtSensor.setup();
}

void loop() {
  float flames = (float) flameSensor.value();
  float temperature = dhtSensor.temperature();
  float air = dhtSensor.humidity();
  
  Serial.print(flames); Serial.print(",");
  Serial.print(air); Serial.print(",");
  Serial.println(temperature);
  
  delay(200);
}
