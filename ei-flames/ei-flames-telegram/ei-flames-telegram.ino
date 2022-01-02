/*
 * ei-flames-telegram: handle the telgram BOT of my oven sensor
 * 
 * project based on Rui Santos's example: https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
 * 
 * Board: LOLIN(WEMOS) D1 R2 & mini
 * CPU Frequence: 160 MHz
*/

#include "DataTelegramBOT.h"

DataTelegramBOT telegramBOT;


/* Arduino setup and loop */
void setup() {
  Serial.begin(115200);

  telegramBOT.setup(1000);
}

void loop() {
  telegramBOT.loop();
}
