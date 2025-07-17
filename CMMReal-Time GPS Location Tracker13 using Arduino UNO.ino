#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize GPS and LCD
SoftwareSerial gpsSerial(8, 9); // RX, TX
TinyGPSPlus gps;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address might be 0x27 or 0x3F

float latitude, longitude;

void setup() {
  gpsSerial.begin(9600);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
   lcd.setCursor(0, 1);
  lcd.print("CMMGPS13");
  delay(6000);
  lcd.clear();
}

void loop() {
  while (gpsSerial.available()) {
    int data = gpsSerial.read();
    if (gps.encode(data)) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();

      // Print to Serial Monitor
      Serial.print("Latitude: ");
      Serial.println(latitude);
      Serial.print("Longitude: ");
      Serial.println(longitude);

      // Print to LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lat:");
      lcd.setCursor(4, 0);
      lcd.print(latitude, 4); // 4 decimal precision

      lcd.setCursor(0, 1);
      lcd.print("Lng:");
      lcd.setCursor(4, 1);
      lcd.print(longitude, 4);
    }
  }
}
