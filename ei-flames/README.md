# Edge Impulse Example: Flames
This runs a trained model of the flames sample and performs verifications.

## The story
I want to get my hands on machine learning at the edge. 
This is the sixth example of edge impulse to get in touch with their data forwarder interfacing an ESP8266 running MicroPhyton.

The ESP8266 sends data collected from a flame sensor, a temperature sensor and a humidity (air) sensor.

Data is sampled at a rate of 6 Hz. 
Sometimes the data forwarder reports 4 Hz.
In this case I overwrite the frequency using the '--frequency 6' parameter.

```
edge-impulse-data-forwarder --frequency 6
```

Data is collected 190 cm in front of the oven at a height of 70 cm.

## Labels
The dataset labels defines the state of the oven.

### loading
Wood was put in the oven.

### burning
Wood catches fires. Some flames are visible in the oven.

### full burning
Wood is burning. A lot of flames are visible in the oven.

### glowing
Wood is glowing. No more flames in the oven.

### loading
If the glowing decreases, I have to load the oven.

## Results
Detecting the loading state is handled by checking the treshhold of the flames value over some time.
The ML model is not that exact enough to deliver the information every time.

Sending the telegram sometimes fails due to erros in the async library of the ESP.
Behaviour is well known but off topic here.

## Platform
Data collection in MicroPython and Arduino. Arduino libray used to run model with same hardware.

## Folders	

### Data Collector
Source of the data collectors used.

#### Arduino
The Arduino source code of the data collector.

#### MicroPython
The MicroPhyton source code of the data collector.

### ei-flames-telegram
Arduino code to run the data telegram BOT to report states of the oven sensor.

### ei-flames-arduino
Arduino code to run the data collector the data analyzer implementing the ML inferencing and the data telegram BOT.

### OvenSensor
OpenSCAD model of the sensor enclosure.
