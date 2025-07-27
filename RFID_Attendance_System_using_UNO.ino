#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RESET_PIN 9
#define SELECT_PIN 10
#define BUZZER_PIN 8  //  Buzzer connected to D8

MFRC522 rfidReader(SELECT_PIN, RESET_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int serialCounter = 1;
unsigned long startMillis;
int startHours = 12, startMinutes = 55, startSeconds = 50;

struct UserData {
    String cardUID;
    String rollNumber;
    String name;
    String lastTimeIn;
    bool isInside;
};

UserData userList[] = {
    {"43fa6b2fb7381", "ECE-21", "C.M.Manikandan", "", false},
    {"444b5b2fb7380", "ECE-71", "Tushar Yadav", "", false},
    {"4e321b2fb7380", "ECE-69", "Sohel Khan", "", false},
    {"45ac9b2fb7380", "ECE-53", "Sachin Upadhyay", "", false},
};

const int userCount = sizeof(userList) / sizeof(userList[0]);

// Get time from millis()
String getCurrentTime() {
    unsigned long elapsedSeconds = (millis() - startMillis) / 1000;
    int currentHours = startHours + (elapsedSeconds / 3600);
    int currentMinutes = startMinutes + ((elapsedSeconds % 3600) / 60);
    int currentSeconds = startSeconds + (elapsedSeconds % 60);

    if (currentSeconds >= 60) { currentMinutes += currentSeconds / 60; currentSeconds %= 60; }
    if (currentMinutes >= 14) { currentHours += currentMinutes / 60; currentMinutes %= 60; }
    if (currentHours >= 24) { currentHours %= 24; }

    char formattedTime[9];
    sprintf(formattedTime, "%02d:%02d:%02d", currentHours, currentMinutes, currentSeconds);
    return String(formattedTime);
}

int findUserIndex(String cardNumber) {
    for (int i = 0; i < userCount; i++) {
        if (userList[i].cardUID == cardNumber) {
            return i;
        }
    }
    return -1;
}

void showWelcomeScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WELCOME TO");
    lcd.setCursor(0, 1);
    lcd.print("CMM RFID 13");
}

// 🎵 Buzzer sound using tone()
void beepTone(int freq, int duration) {
    tone(BUZZER_PIN, freq);
    delay(duration);
    noTone(BUZZER_PIN);
}

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfidReader.PCD_Init();

    lcd.init();
    lcd.backlight();

    pinMode(BUZZER_PIN, OUTPUT);  // Optional with tone(), but good practice

    showWelcomeScreen();

    startMillis = millis();

    Serial.println("Serial Number,RFID Card Number,Roll Number,Name,Time In,Time Out,Status");
}

void loop() {
    static bool firstScanDone = false;
    static bool welcomeShown = false;

    if (!firstScanDone && !welcomeShown) {
        showWelcomeScreen();
        welcomeShown = true;
    }

    if (!rfidReader.PICC_IsNewCardPresent() || !rfidReader.PICC_ReadCardSerial()) {
        return;
    }

    firstScanDone = true;
    welcomeShown = false;

    String cardNumber = "";
    for (byte i = 0; i < rfidReader.uid.size; i++) {
        cardNumber += String(rfidReader.uid.uidByte[i], HEX);
    }

    String currentTime = getCurrentTime();
    String timeIn = "-", timeOut = "-", status = "Unknown";

    int userIndex = findUserIndex(cardNumber);

    if (userIndex != -1) {
        UserData &user = userList[userIndex];

        if (!user.isInside) {
            user.lastTimeIn = currentTime;
            user.isInside = true;
            timeIn = user.lastTimeIn;
            status = "ENTERED";
        } else {
            timeIn = user.lastTimeIn;
            timeOut = currentTime;
            user.isInside = false;
            status = "EXITED";
        }

        //  Known card — short beep
        beepTone(1000, 100);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(user.name.substring(0, 16));
        lcd.setCursor(0, 1);
        lcd.print(status + " " + user.rollNumber.substring(0, 9));
        delay(4000);
        showWelcomeScreen();

        Serial.print(serialCounter++);
        Serial.print(",");
        Serial.print(cardNumber);
        Serial.print(",");
        Serial.print(user.rollNumber);
        Serial.print(",");
        Serial.print(user.name);
        Serial.print(",");
        Serial.print(timeIn);
        Serial.print(",");
        Serial.print(timeOut);
        Serial.print(",");
        Serial.println("STUDENT " + status);

    } else {
        //  Unknown card — long beep
        beepTone(700, 300);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Unknown RFID");
        lcd.setCursor(0, 1);
        lcd.print(cardNumber.substring(0, 16));
        delay(4000);
        showWelcomeScreen();

        Serial.print(serialCounter++);
        Serial.print(",");
        Serial.print(cardNumber);
        Serial.print(",");
        Serial.print("Unknown");
        Serial.print(",");
        Serial.print("Unknown");
        Serial.print(",");
        Serial.print(currentTime);
        Serial.print(",");
        Serial.print("-");
        Serial.print(",");
        Serial.println("STUDENT ENTERED");
    }

    delay(500);
    rfidReader.PICC_HaltA();
    rfidReader.PCD_StopCrypto1();
}