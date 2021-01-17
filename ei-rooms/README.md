# Edge Impulse Example: Gestures
This runs a trained model of the the rooms model and performs verifications.

## The story
I want to get my hands on machine learning at the edge.
This is the fifth example of edge impulse to get in touch with their data acquisition format.

I use humidity and temperature data collected with the Sensate ESP8266 application.

## Workflow
* I collect humidity and temperature data with application. Data is collected every 30 seconds.
* I export the data to xls format and convert it to (valid) csv format.
* I use the DataAquisition phython script to create json files for 30 minutes data.
* I upload the json files to Edge Impulse.

## feature
### cooking
### living
### sleeping

## Platform
Developed in Ardunio IDE for the Nano BLE 33 Sense board.
