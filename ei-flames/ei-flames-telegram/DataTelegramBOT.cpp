#include "DataTelegramBOT.h"

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#include "WifiCredentials.h"
//const char* ssid = "REPLACE_WITH_YOUR_SSID";
//const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Initialize Telegram BOT
#include "TelegramBOT.h"
//#define BOTtoken "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)

// Use @IDBot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
//#define CHAT_ID "XXXXXXXXXX"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Handle what happens when you receive new messages
void DataTelegramBOT::handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following commands to view your oven.\n\n";
      welcome += "/state to request current oven state.\n";
      welcome += "/keep_alive to switch keep alive messages.\n";
      bot.sendMessage(chat_id, welcome, "");
    }
    
    if (text == "/state") {
      String stateMsg = "Oven is " + state;
      bot.sendMessage(chat_id, stateMsg, "");
    }
    if (text == "/keep_alive") {
      keepAlive = HIGH - keepAlive;
        if (keepAlive == HIGH) {
          bot.sendMessage(chat_id, "Keep alive is ON", "");
        }
        else{
          bot.sendMessage(chat_id, "Keep alive is OFF", "");
        }
    }
  }
}

void DataTelegramBOT::sendMessage(const char *msg) {
      bot.sendMessage(CHAT_ID, msg, "");
}
void DataTelegramBOT::setup(unsigned long requestDelay, unsigned long keepAliveDelay) {

  // check for new bot messages
  botRequestDelay = requestDelay;
  botKeepAliveDelay = keepAliveDelay;
  keepAlive = LOW;
  state = "at unknown state";

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
}

void DataTelegramBOT::loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

  /* Send a "keep alive" message to the bot */
  if (keepAlive) {
    if (millis() > lastTimeKeepAliveSent + botKeepAliveDelay) {
      sendMessage("System is up and running");
      lastTimeKeepAliveSent = millis();
    }
  }
}
