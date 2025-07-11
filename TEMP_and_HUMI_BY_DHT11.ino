#include<Wire.h>
#include<DHT.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  dht.begin();
  Serial.begin(9600);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("CMM TEM&HUM13");  
   delay(6000);
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  float temp=dht.readTemperature();
  float humi=dht.readHumidity();
  if(isnan(temp) || isnan(humi))
  {
    Serial.println("DATA IS NOT AVAILABLE");
    return;
  }
  Serial.print("temperature is :");
  Serial.print(temp); 
  Serial.print(" ");
  Serial.print("humidity is :");
  Serial.println(humi); 
  lcd.setCursor(0, 0);  
  lcd.print("Temp:");
  lcd.print(temp);
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.print(humi);  
   delay(500);
  }
