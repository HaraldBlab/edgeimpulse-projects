#include "Dht11Sensor.h"

Dht11Sensor::Dht11Sensor(int sensor) {
  sensorPin = sensor;
  dhtTemperature = 0.0;
  dhtHumidity = 0.0;
#if defined(__AVR__)
  dht = new DHT(sensorPin, DHT11);
#elif defined(ESP8266)
  dht = new DHTesp();
#endif
}
Dht11Sensor::~Dht11Sensor() {
  delete dht;
}

void Dht11Sensor::setup() {
#if defined(__AVR__)
  dht->begin();
#elif defined(ESP8266)
  dht->setup(sensorPin, DHTesp::DHT11);
#endif
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
  if (dhtMillis - dhtLastMillis > 2000) {
#if defined(__AVR__)
    dhtTemperature = dht->readTemperature();
    dhtHumidity = dht->readHumidity();
    dhtLastMillis = millis();
#elif defined(ESP8266)
    TempAndHumidity lastValues = dht->getTempAndHumidity();
    if (isnan(lastValues.temperature) || isnan(lastValues.humidity)) {
      setup();
      return;
    }
    dhtTemperature = lastValues.temperature;
    dhtHumidity = lastValues.humidity;
    dhtLastMillis = millis();
#endif
  }
}
