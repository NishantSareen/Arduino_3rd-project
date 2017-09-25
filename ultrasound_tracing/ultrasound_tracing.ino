#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
//int trigReading;
long duration;
long distance;
int echoPin = 47;
int trigPin = 49;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.print("Distance: ");

  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
}

void loop() {
  lcd.clear();
  lcd.print("Distance: ");
  
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);
  distance = duration/58; // inches
  
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print("cm");
int  distance1 = map(distance, 0, 255, 10, 0);
  Serial.println(distance,DEC);

  delay(250);
}
