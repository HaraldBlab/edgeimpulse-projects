#ifndef DataTelegramBOT_h
#define DataTelegramBOT_h

#include "Arduino.h"

/*
 * A telegram BOT to communicate with the oven sensor.
 * 
 * Project based on Rui Santos's example: https://RandomNerdTutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/
 */
class DataTelegramBOT {
  unsigned long botRequestDelay;  /* time span for handling requests */
  unsigned long lastTimeBotRan;
  bool keepAlive;
  unsigned long botKeepAliveDelay;  /* time span for keep alive */
  unsigned long lastTimeKeepAliveSent;
public:  
  String state;  /* overall oven status */
  bool anySendMessage;  /* indicator to send some message to BOT */
public:
  /* setup communication with the telegram bot */
  void setup(unsigned long requestDelay = 1000, unsigned long keepAliveDelay = 60000);
  /* handle receiving messages */
  void loop();
  /* send a message to the bot */
  void sendMessage(const char *msg);
private:
  /* Handle what happens when you receive new messages */
  void handleNewMessages(int numNewMessages);
};

#endif
