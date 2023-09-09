#define TINY_GSM_MODEM_SIM800
#define CHAT_ID ""
// Теперь подключаем вашу библиотеку
#include <TelegramGSM.h>
#include <SoftwareSerial.h>
SoftwareSerial Serial1(2, 3);
const char* BOT_TOKEN = "";
TinyGsm modem(Serial1);
TinyGsmClientSecure client(modem);
HttpClient http(client, "api.telegram.org", 443);
TelegramGSM telegramBot(client, http, BOT_TOKEN);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial1.begin(115200);
  delay(3000);
  if (!modem.init()) {
    Serial.println("Failed to initialize modem.");
    while (true);
  }

  if (!modem.waitForNetwork()) {
    Serial.println("Failed to connect to network.");
    while (true);
  }
}

void loop() {
  telegramBot.getUpdates();
  telegramBot.sendTelegramMessage("Привет от вашего Arduino бота!");
  delay(10000);
}
