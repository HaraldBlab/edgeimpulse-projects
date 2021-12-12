from machine import Pin
from time import ticks_ms
import dht;

# A sensor to measure the temperature and humidity of the environment.
# interfaces on the dht.DHT11 environment sensor.
# new values are measured every 1000 ms.
# last read values are provided in the meantime.
class Dht11Sensor():
  dhtLastMillis = 0
  dhtMillis = 0
  dhtMeasureMillis = 1000
  dhtTemperature = 0.0
  dhtHumidity = 0.0
  dhtErrors = 0
  def __init__(self, MEASURE):
    global sensor, dhtMillis, dhtTemperature, dhtHumidity
    sensor = dht.DHT11(Pin(MEASURE));
    self.dhtLastMillis = ticks_ms()
  def temperature(self):
    self.read()
    return self.dhtTemperature
  def humidity(self):
    self.read()
    return self.dhtHumidity
  def read(self):
    try:
      self.dhtMillis = ticks_ms()
      if self.dhtMillis - self.dhtLastMillis > self.dhtMeasureMillis:
        sensor.measure()
        self.dhtTemperature = sensor.temperature()
        self.dhtHumidity = sensor.humidity()
        self.dhtLastMillis = ticks_ms();
    except OSError as e:
      self.dhtErrors = self.dhtErrors + 1
      #print('Failed to read sensor.')
