#include <Servo.h>. 
const int trigPin = 10;
const int echoPin = 11;
const int buzzer = 13;

long duration;
int distance;
int safeDistance;
Servo myServo; 
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);
  myServo.attach(12); 
}
void loop() {
  for(int i=15;i<=165;i++){  
  myServo.write(i);
  delay(30);
  distance = calculateDistance();
  
  Serial.print(i); 
  Serial.print(","); 
  Serial.print(distance); 
  Serial.print("."); 
  }
   for(int i=165;i>15;i--){  
  myServo.write(i);
  delay(30);
  
  
  distance = calculateDistance();
  Serial.print(i);
  Serial.print(",");
  Serial.print(distance);
  Serial.print(".");
  }
  }
int calculateDistance(){ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = duration*0.034/2;
  safeDistance = distance;
 if(safeDistance<=30)
 {
    digitalWrite(buzzer,HIGH);
  }
  else{
    digitalWrite(buzzer,LOW);
  }
  return distance;
}
