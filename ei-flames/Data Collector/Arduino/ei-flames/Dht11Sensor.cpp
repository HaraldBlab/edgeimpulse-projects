#include "Dht11Sensor.h"
#define DHTTYPE DHT11 // DHT 11

Dht11Sensor::Dht11Sensor(int sensor) {
  sensorPin = sensor;
  dhtTemperature = 0.0;
  dhtHumidity = 0.0;
  dht = new DHT(sensorPin, DHTTYPE);
}
Dht11Sensor::~Dht11Sensor() {
  delete dht;
}

void Dht11Sensor::setup() {
  dht->begin();
  dhtLastMillis = millis();
}
float Dht11Sensor::temperature(){
  read();
  return dhtTemperature;
}
float Dht11Sensor::humidity(){
  read();
  return dhtHumidity;
}
void Dht11Sensor::read(){
  unsigned long dhtMillis = millis();
  if (dhtMillis - dhtLastMillis > 1000) {
    dhtTemperature = dht->readTemperature();
    dhtHumidity = dht->readHumidity();
    dhtLastMillis = millis();
  }
}
