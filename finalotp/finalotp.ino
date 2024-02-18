#include "Arduino.h"
#include <EMailSender.h>
#include <ESP8266WiFi.h>
#include <Keypad.h>

const char* ssid = "Sumithero";
const char* password = "bdcs2034";
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { D1, D2, D3, D4 };  // D1, D2, D3, D4 are GPIO pins
byte colPins[COLS] = { D5, D6, D7, D8 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


char keypassword[] = "*3568#";

char inputBuffer[D0];  // Buffer to store user input (including null terminator)
int inputIndex = 0;

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("sumitherodon159@gmail.com", "hken cuam zlqb ggsd");

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr) {
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  if (nSSID) {
    WiFi.begin(nSSID, nPassword);
    Serial.println(nSSID);
  }

  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50) {
    delay(200);
    Serial.print(".");
  }
  ++attempt;
  Serial.println("");
  if (i == 51) {
    Serial.print("Connection: TIMEOUT on attempt: ");
    Serial.println(attempt);
    if (attempt % 2 == 0)
      Serial.println("Check if access point available or SSID and Password\r\n");
    return false;
  }
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void Awaits() {
  uint32_t ts = millis();
  while (!connection_state) {
    delay(50);
    if (millis() > (ts + reconnect_interval) && !connection_state) {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void generateNewkeyPassword() {
  int newkeyPassword = random(1000, 9999);         // Generate a random 6-digit number
  sprintf(keypassword, "*%04d#", newkeyPassword);  // Format the new password and store it in the password array

  connection_state = WiFiConnect(ssid, password);
  if (!connection_state)  // if not connected to WIFI
    Awaits();             // constantly trying to connect

  EMailSender::EMailMessage message;
  message.subject = "PASSWORD CHANGED TO";
  message.message = newkeyPassword;

  EMailSender::Response resp = emailSend.send("sumsigdel@gmail.com", message);
  Serial.println("Sending status: ");
  Serial.println(resp.status);
  Serial.println(resp.code);
  Serial.println(resp.desc);
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    // Check if the input is within the allowed length
    if (inputIndex < 6) {
      inputBuffer[inputIndex++] = key;
      Serial.print("Current Input: ");
      Serial.println(inputBuffer);

      // Check if the input is complete
      if (key == '#' && inputIndex == 6) {
        Serial.println("Checking keypassword...");

        // Compare the entered password with the predefined password
        if (strcmp(inputBuffer, keypassword) == 0) {
          Serial.println("Correct keypassword!");

          // Generate and update the new password
          generateNewkeyPassword();

          // Print the new password
          Serial.print("New keyPassword: ");
          Serial.println(keypassword);
        } else {
          Serial.println("Incorrect keypassword!");
        }

        // Reset the input buffer and index for the next input
        inputIndex = 0;
        memset(inputBuffer, 0, sizeof(inputBuffer));
      }
    } else {
      // Reset the input buffer and index if the input is too long
      inputIndex = 0;
      memset(inputBuffer, 0, sizeof(inputBuffer));
      Serial.println("Invalid input. Resetting.");
    }
  }
}