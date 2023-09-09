
// TelegramBot.h

#ifndef TelegramGSM_h
#define TelegramGSM_h
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <Arduino.h>
// sending you an SOS of love ... 
//DEFINE HERE YOUR CHAT_ID AND BOT TOKEN IF IT IS NOT DEFINED IN SKETCH
#ifndef CHAT_ID
#define CHAT_ID ""
#endif


class TelegramGSM {
private:
    String lastUpdateID;
    TinyGsmClientSecure& client;
    HttpClient& http;
    const char* botToken;

public:
    TelegramGSM(TinyGsmClientSecure& client, HttpClient& http, const char* BOT_TOKEN) : client(client), http(http), botToken(BOT_TOKEN) {
        lastUpdateID = "0";
    }

    void getUpdates() {
        String url = "/bot" + String(botToken) + "/getUpdates";
        url += "?offset=" + String(lastUpdateID.toInt() + 1);

        if (!client.connect("api.telegram.org", 443)) {
            Serial.println("Failed to connect to Telegram API");
            return;
        }

        client.print("GET " + url + " HTTP/1.1\r\n");
        client.print("Host: api.telegram.org\r\n");
        client.println("Connection: close\r\n");

        if (client.connected()) {
            while (client.available()) {
                String line = client.readStringUntil('\r');
                if (line.indexOf("\"text\":\"") > 0) {
                    int startPos = line.indexOf("\"text\":\"") + 8;
                    int endPos = line.indexOf("\",", startPos);
                    String messageText = line.substring(startPos, endPos);
                    Serial.println("Received message: " + messageText);
                    // Добавьте здесь свой код для обработки принятого сообщения
                }
            }
            client.stop();
        }
    }

    void sendTelegramMessage(String message) {
        String url = "/bot" + String(botToken) + "/sendMessage";
        String payload = "chat_id=" + String(CHAT_ID) + "&text=" + message;

        http.beginRequest();
        http.post(url, "application/x-www-form-urlencoded", payload);
        http.endRequest();
    }

    void showMenu(String menu) {
        String url = "/bot" + String(botToken) + "/sendMessage";
        String payload = "chat_id=" + String(CHAT_ID) + "&text=" + menu;

        // Создаем JSON для клавиатуры с кнопками
        String keyboard = "{\"keyboard\":[";

        int prevPos = 0;
        int newPos = menu.indexOf('\n', prevPos);

        while (newPos != -1) {
            keyboard += "[";
            String buttonRow = menu.substring(prevPos, newPos);

            int prevTabPos = 0;
            int newTabPos = buttonRow.indexOf('\t', prevTabPos);

            while (newTabPos != -1) {
                keyboard += "\"" + buttonRow.substring(prevTabPos, newTabPos) + "\",";
                prevTabPos = newTabPos + 1;
                newTabPos = buttonRow.indexOf('\t', prevTabPos);
            }

            keyboard += "\"" + buttonRow.substring(prevTabPos) + "\"],";
            prevPos = newPos + 1;
            newPos = menu.indexOf('\n', prevPos);
        }

        keyboard = keyboard.substring(0, keyboard.length() - 1);  // Убираем последнюю запятую
        keyboard += "],\"resize_keyboard\":true}";

        payload += "&reply_markup=" + keyboard;

        http.beginRequest();
        http.post(url, "application/x-www-form-urlencoded", payload);
        http.endRequest();
    }

};

#endif
