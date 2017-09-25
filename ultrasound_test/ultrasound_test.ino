#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
//int trigReading;
long duration;
long distance;
int echoPin1 = 49;
int trigPin1 = 47;
int echoPin2 = 7; // should be 34
int trigPin2 = 8; // should be 32
boolean left;
boolean right;
boolean bottom;
boolean laserstatus;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(38400);
  lcd.print("Distance: ");

  pinMode(echoPin1,INPUT);
  pinMode(trigPin1,OUTPUT);
}

void loop() {
 int dleft= finddistance(trigPin1,echoPin1);
 int dright=finddistance(trigPin2,echoPin2);
 Serial.print("Left distance:");
 Serial.print(dleft,DEC);
 Serial.print("  Right distance:");
 Serial.println(dright,DEC);
//setultrasoundstatus(left,dleft);
//setultrasoundstatus(right,dright);
// add bottom sensor as well
  delay(250);
  
}

long finddistance(int trigPin, int echoPin){ 
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  //lcd.print("cm");
  //int  mapped_distance = map(distance, 0, 4000, 10, 0);
  duration = pulseIn(echoPin,HIGH);
  return distance = duration/58; // inches  
  }

