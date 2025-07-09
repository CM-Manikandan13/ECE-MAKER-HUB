//CMM GAS LEVEL DETECTOR 13
//this info you will get from blynk app 
#define BLYNK_TEMPLATE_ID "**************"
#define BLYNK_TEMPLATE_NAME "**************"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define sensor 34         // Analog pin for gas sensor
#define buzzer 2          // Digital pin for buzzer

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16x2 LCD
BlynkTimer timer;
WidgetLED LED(V1); // Virtual LED on Blynk app

// WiFi + Blynk credentials
char auth[] = "*******************";
char ssid[] = "********";
char pass[] = "********";

// Hysteresis thresholds
const int highThreshold = 55;
const int lowThreshold = 45;
bool gasHigh = false;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  
  lcd.init();
  lcd.backlight();
  pinMode(buzzer, OUTPUT);

  // Boot animation
  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("project name here");
  delay(3000);
  lcd.clear();

  // Run GASLevel function every 500ms
  timer.setInterval(500L, GASLevel);
}

void GASLevel() {
  int raw = analogRead(sensor);
  int value = map(raw, 0, 4095, 0, 100); // Convert to 0–100 scale

  // Display gas level on LCD
  lcd.setCursor(0, 0);
  lcd.print("GAS Level: ");
  lcd.print(value);
  lcd.print("   "); // Clear residue digits

  // Hysteresis logic for stable warning
  if (!gasHigh && value >= highThreshold) {
    gasHigh = true;
    digitalWrite(buzzer, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Warning!       ");
    LED.on();
  } else if (gasHigh && value <= lowThreshold) {
    gasHigh = false;
    digitalWrite(buzzer, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Normal         ");
    LED.off();
  }

  // Send gas level to Blynk
  Blynk.virtualWrite(V0, value);
  Serial.println(value);
}

void loop() {
  Blynk.run();
  timer.run();
}
 
