# Edge Impulse Example: Gestures
This runs a trained model of the fruits model and performs verifications.

## The story
I want to get my hands on machine learning at the edge.
This is the fourth example of edge impulse to get in touch with image recognition.

Images taken with the mobile phone and rescaled on a laptop before uploading.

## Workflow
* I make images woth the mobile phone.
* I batch convert the image with InfraView converter to 320x240 size.
* I upload the images to Edge Impulse.

## feature
### apple
### banana
### orange
### pear
### kiwi
### strawberry
### nectarine
### apricot
### grape

## Detection
### floating point model
Detects with 89%
There are errors detecting nectarine and apricot.
### optimited EON model model
Detects poorly with 32%. The confusion matrix is almost red. :(

## Platform
Arduino IDE 1.8.15 sample for the AI Thinker ESP32CAM board with Partition scheme Huge App.

Based on work provided by EdgeImpulse. 
https://forum.edgeimpulse.com/t/esp32-cam-support/797
Great Job. 
Thank's folks.
