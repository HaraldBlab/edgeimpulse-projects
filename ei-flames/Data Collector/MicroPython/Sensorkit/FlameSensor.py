from machine import Pin, ADC

# A sensor to measure the (flame) heat of the environment
# high values indicate not a (flame) heat
# low values indicate a (flame) heat
# trim poti adjusts around value 500 only-
class FlameSensor():
  def __init__(self, DETECT, POT):
    global pinDetect, pot
    pinDetect = Pin(DETECT, Pin.IN)
    pot = ADC(POT)
  def value(self):
    return pot.read()
  def detect(self):
    return pinDetect.value()
