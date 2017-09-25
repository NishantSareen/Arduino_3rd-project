// This code moves the tracks the object horizontal and vertical
#include <Servo.h>
Servo horizontalservo;
Servo verticalservo;
//int trigReading;
long duration;
long distance;
int currentHorizontalServoPos;
int currentVerticalServoPos;
int leftshiftcheck;
int rightshiftcheck;
int upshiftcheck;
int downshiftcheck;

int echoPinL = 49;
int trigPinL = 47;
int echoPinR = 33; // should be 33
int trigPinR = 31; // should be 31
int trigPinB = 37;
int echoPinB = 39;
int laser_din = 7;


boolean left;
boolean right;
boolean bottom;
boolean laserstatus;
boolean sensor;
boolean targetengaged;
boolean setInitialServoPos;
boolean automatictrace;

void setup() {

  Serial.begin(250000);
  pinMode(laser_din, INPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(echoPinB, INPUT);
  pinMode(trigPinB, OUTPUT);
  horizontalservo.attach(12); // attaches to pin 12 of MEGA
  verticalservo.attach(13); // attaches to pin 13 of MEGA

}



void loop() {
   
  int dleft = finddistance(trigPinL, echoPinL);
  int dright = finddistance(trigPinR, echoPinR);
  int dbottom = finddistance(trigPinB, echoPinB);

  
  setlaserstatus();
  if (setInitialServoPos == false) {
    horizontalservo.write(100);
    verticalservo.write(100);
    setInitialServoPos = true;
    currentHorizontalServoPos = 100;
    currentVerticalServoPos = 100;
  }
  leftshiftcheck = currentHorizontalServoPos + 10;
  rightshiftcheck = currentHorizontalServoPos - 10;
  upshiftcheck = currentVerticalServoPos + 10;
  downshiftcheck = currentVerticalServoPos - 5;

  Serial.print("Left distance:");
  Serial.print(dleft, DEC);
  setultrasoundstatus(dleft, 'L');
  Serial.print(" " + getSensorStatusString());
  Serial.print("  Right distance:");
  Serial.print(dright, DEC);
  setultrasoundstatus(dright, 'R');
  Serial.print(" " + getSensorStatusString());
  Serial.print("  Bottom distance: ");
  Serial.print(dbottom, DEC);
  setultrasoundstatus(dbottom, 'B');
  Serial.print(" " + getSensorStatusString());
  Serial.print("  Horizontal Servo Pos:");
  Serial.print(currentHorizontalServoPos, DEC);
  Serial.print("  Vertical Servo Pos:");
  Serial.print(currentVerticalServoPos, DEC);
  Serial.print("  laser Status:");
  Serial.println(getlaserStatusString());
  delay(200);
  // Horizontal shift
  if (laserstatus == true) {
    if (left == true) {
      ServoShiftLeft();
    }
    if (right == true) {
      ServoShiftRight();
    }
  }
/*
  // vertical shift
  if (laserstatus == true && bottom == false) {
    // move up
    ServoShiftUp();
  }
  if (laserstatus == false && bottom == true) {
    // move down
    ServoShiftDown();
  }
  */


}

// Functions

long finddistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return distance = duration / 58; // inches
}

// set the ultrasound status
void setultrasoundstatus(int sensorFeed, char individualsensor) {
  if (sensorFeed < 100) {
    sensor = true;
    switch (individualsensor) {
      case'L':
        left = true;
        break;
      case'R':
        right = true;
        break;
      case'B':
        bottom = true;
        break;
    }

  }
  else {
    sensor = false;
    switch (individualsensor) {
      case'L':
        left = false;
        break;
      case'R':
        right = false;
        break;
      case'B':
        bottom = false;
        break;
    }
  }
}
// set the laser status
void setlaserstatus() {
  if (digitalRead(laser_din) == LOW)
  {
    laserstatus = true;
    // Serial.println("Obstacles!");
  }
  else
  {
    laserstatus = false;
    //Serial.println("No Obstacles!");
  }

}
// chages the status of the ultrasound sensor
String getSensorStatusString() {
  if (sensor == true) {
    return ("true");
  }
  else {
    return ("false");
  }
}
String getlaserStatusString() {
  if (laserstatus == true) {
    return ("true");
  }
  else {
    return ("false");
  }
}

void ServoShiftLeft() {

  if (leftshiftcheck <= 180) {
    int leftPos;
    // Serial.println("servoshiftleft works!!");
    for (leftPos = currentHorizontalServoPos; leftPos <= currentHorizontalServoPos + 10; leftPos += 10) {
      horizontalservo.write(leftPos);
    }
    currentHorizontalServoPos = leftPos;
  }
 
}

void ServoShiftRight() {
  if (rightshiftcheck >= 0) {
    //Serial.println("servoshiftright works!!");
    int rightPos;
    for (rightPos = currentHorizontalServoPos; rightPos >= currentHorizontalServoPos - 10; rightPos -= 10) {
      horizontalservo.write(rightPos);
    }
    currentHorizontalServoPos = rightPos;
  }
  
}

void ServoShiftUp() {
  if (upshiftcheck <= 180) {
    int upPos;
    for (upPos = currentVerticalServoPos; upPos <= currentVerticalServoPos + 10; upPos += 10) {
      verticalservo.write(upPos);
    }
  }
}
void ServoShiftDown() {
  if (downshiftcheck >= 100) {
    int downPos;
    for (downPos = currentVerticalServoPos; downPos >= currentVerticalServoPos - 10; downPos -= 10) {
      verticalservo.write(downPos);
    }
  }

}


