#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

// Update these pin configurations for NodeMCU
byte rowPins[ROWS] = { 13, 12, 11, 10 };  // D1, D2, D3, D4 are GPIO pins
byte colPins[COLS] = { 9, 8, 7, 6 };      // D5, D6, D7, D8 are GPIO pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char password[] = "*1234#";  // Predefined password

char inputBuffer[7];  // Buffer to store user input (including null terminator)
int inputIndex = 0;   // Index to keep track of the current position in the input buffer
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Function to generate a new 6-digit password
void generateNewPassword() {
  int newPassword = random(1000, 9999);  // Generate a random 6-digit number
  sprintf(password, "*%04d#", newPassword);
  Serial.println(newPassword);
  // Format the new password and store it in the password array
}

void setup() {
  Serial.begin(9600);
  lcd.begin();  // Initialize the LCD with 16 columns and 2 rows
  lcd.backlight();
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    // Check if the input is within the allowed length
    if (inputIndex < 6) {
      inputBuffer[inputIndex++] = key;
      Serial.print("Current Input: ");
      lcd.setCursor(0, 0);
      lcd.print("Current Input:");
      Serial.println(inputBuffer);
      lcd.setCursor(0, 1);
      lcd.print(inputBuffer);
      // Check if the input is complete
      if (key == '#' && inputIndex == 6) {
        lcd.clear();
        Serial.println("Checking password...");
        lcd.print("Checking password...");
        // Compare the entered password with the predefined password
        if (strcmp(inputBuffer, password) == 0) {
          lcd.clear();
          Serial.println("Correct password!");
          lcd.print("Correct password!");
          lcd.clear();
          lcd.print("You can enter");
          // Generate and update the new password
          generateNewPassword();

          // Print the new password
          Serial.print("newpassword:");

          Serial.println(password);

        } else {
          Serial.println("Incorrect password!");
          lcd.clear();
          lcd.print("Incorrect password!");
        }

        // Reset the input buffer and index for the next input
        inputIndex = 0;
        memset(inputBuffer, 0, sizeof(inputBuffer));
      }
    } else {
      // Reset the input buffer and index if the input is too long
      inputIndex = 0;
      memset(inputBuffer, 0, sizeof(inputBuffer));
      Serial.println("Invalid input. Resetting!");
      lcd.clear();
          lcd.print("Invalid input");
    }
  }
}
