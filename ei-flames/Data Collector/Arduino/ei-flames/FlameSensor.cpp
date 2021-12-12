#include "FlameSensor.h"

FlameSensor::FlameSensor(int poti, int sensor)
{
  sensorPin = sensor;
  potiPin = poti;
}
void FlameSensor::setup() {
  pinMode(sensorPin, INPUT);
}
int FlameSensor::value() {
  potiValue = analogRead(potiPin);
  return potiValue;
}
bool FlameSensor::detect() {
  int val=digitalRead(sensorPin);
  return val == HIGH;
}
