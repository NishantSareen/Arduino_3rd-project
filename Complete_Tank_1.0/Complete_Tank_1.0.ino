
#include <Servo.h> 
Servo horizontalservo;
Servo verticalservo;

long duration; // stores timing used for ultrasound distance measurement
long distance; // stores distance measures from the ultrasound in inches.

int currentHorizontalServoPos; // Because the servo does not give a feedback on which angle it's pointing to this int is used to keep track of the servos relative horizontal position.
int currentVerticalServoPos;
int leftshiftcheck;
int rightshiftcheck;
int upshiftcheck;
int downshiftcheck;

int echoPinL = 49;   // Echo and trigger pin outs are used in distance measurement by the ultrasound.
int trigPinL = 47;
int echoPinR = 33;
int trigPinR = 31;
int trigPinB = 37;
int echoPinB = 39;
int laser_din = 7;   // pin out int for the laser feedback
int firepin = 4;     // pin out for triggering the relay switch used to fire the rail gun


boolean left;       // used to keep track of an object coming within an ultrasounds predefined range
boolean right;
boolean laserstatus;
boolean sensor;    //  also used to keep track of an object coming within an ultrasounds predefined range
boolean setInitialServoPos;
boolean automaticTrace;  // used to trigger the automatic track feature in the main loop
boolean manualControl = true; // used to trigger the manual control takeover in the main loop.
boolean trace;

byte lastRead;     // last read and current read bytes are used in bluetooth communication.
byte currentRead;
byte StatusLED = 0;  // used in bluetooth communication as a means to ensure that the module is alive and running.

void setup() {

  Serial.begin(74880); // Serial is used for communication with the arduino via the usb port
  Serial1.begin(9600); // Serial 1 is used for communication with the bluetooth module and app.
  pinMode(laser_din, INPUT);
  pinMode(echoPinL, INPUT);
  pinMode(trigPinL, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(echoPinB, INPUT);
  pinMode(trigPinB, OUTPUT);
  pinMode(firepin, OUTPUT);
  horizontalservo.attach(11); // attaches to pin 12 of MEGA
  verticalservo.attach(12); // attaches to pin 13 of MEGA
  pinMode(A0, OUTPUT); // A0 and A1 connects to bottom of H-bridge.
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT); // A2 and A3 connects to top of H-bridge.
  pinMode(A3, OUTPUT);
}

void loop() {

  // The Mode at which the code runs in initially is in manual mode.

  setlaserstatus();  // calls the set laser function to detect if an object is in the lasers range
  if (setInitialServoPos == false) {  // The content of this if-statement functionality is to initialize the position of the Horizontal servo to 100 when this loop() first runs and
    horizontalservo.write(100);       // not to access it again until a reset is pressed on the board.

    setInitialServoPos = true;
    currentHorizontalServoPos = 100;

  }
  if (trace == true) {               // follows the trace pattern in the enableTraceMode() function
    Serial.println("tracing works!");
    enableTraceMode();
    trace = false;
  }

  if (automaticTrace == true) {         // This if statement block is designed such that it allows the user to step out of the automatic Trace mode within the main loop()
    while (Serial1.available() > 0) {
      currentRead = Serial1.read();

      // Read all bytes until a zero is received, store the bytes received before the zero.
      if (currentRead != NULL) {
        lastRead = currentRead;

      }

      if (currentRead != 8) {
        Serial1.write(StatusLED);
      }
      if (currentRead == 8) { // if the byte recieved from the bluetooth module is 8 it enables manual control
        automaticTrace = false;
        manualControl = true;
        break;
      }


      // Horizontal shift
      if (laserstatus == false) { // when still in automatic mode it locks onto the object and moves along with it
        if (left == true) { // If the left ultrasound detects and object within its range it moves to the left until the object is no longer within its range and is
          ServoShiftLeft(); // centered between the two ultrasounds and is detected by the laser which would no longer move the servo.

        }
        if (right == true) {
          ServoShiftRight();
        }
      }
    }
  }
  if (manualControl == true) {
    bluetooth();
  }
  leftshiftcheck = currentHorizontalServoPos + 5; // Used to give the position of 5 degrees ahead of the current position so that we know when to stop after it's limit is reached,
  rightshiftcheck = currentHorizontalServoPos - 5; // and protects the servo from damage.
  upshiftcheck = currentVerticalServoPos + 10;
  downshiftcheck = currentVerticalServoPos - 10;
  setandPrintAllSensorStatus(); //

}

// Functions
void setandPrintAllSensorStatus() {
  int dleft = finddistance(trigPinL, echoPinL); // stores the distance found by left sensor
  int dright = finddistance(trigPinR, echoPinR); // stores the distance found by right sensor
  //int dbottom = finddistance(trigPinB, echoPinB);

  Serial.print("Left distance:");
  Serial.print(dleft, DEC);
  setultrasoundstatus(dleft, 'L'); // refreshes the pevious distance in the left ultrasound
  Serial.print(" " + getSensorStatusString());
  Serial.print("  Right distance:");
  Serial.print(dright, DEC);
  setultrasoundstatus(dright, 'R'); // refreshes the previous distance in the right ultrasound
  Serial.print(" " + getSensorStatusString());
  Serial.print("  Horizontal Servo Pos:");
  Serial.print(currentHorizontalServoPos, DEC);
  Serial.print("  laser Status:");
  Serial.println(getlaserStatusString());

}
// this method finds the distance measured from the ultrasound sensor
long finddistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  return distance = duration / 58; // inches
}

// set the ultrasound status- if the ultrasound sensor detects an object less than 20 inches it turns the left and right booleans true
void setultrasoundstatus(int sensorFeed, char individualsensor) {
  if (sensorFeed < 20) {
    sensor = true;
    switch (individualsensor) {
      case'L':
        left = true;
        break;
      case'R':
        right = true;
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
    }
  }
}
// sets the boolean pertaining to the detection of an object by the laser
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
String getSensorStatusString() { // returns a string value of the ultrasound status
  if (sensor == true) {
    return ("true");
  }
  else {
    return ("false");
  }
}
String getlaserStatusString() { // returns a string value of the laserstatus
  if (laserstatus == true) {
    return ("true");
  }
  else {
    return ("false");
  }
}
// ServoShiftLeft and ServoShiftRight functions are responsible for moving the horizontal servo left and right.
// The servo is controlled by using the servo library included above. It checks if the automatic trace boolean is true and the left or right shift check int that
// shows the next available position of the servo is also true. This function is designed to work such that when the servo reaches its max position the motor turns inorder to continue tracking.
void ServoShiftLeft() {
  if (automaticTrace == true) {
    if (leftshiftcheck >= 180 ) {
      //motorShiftLeft();
      motorShiftRight();
    }
  }
  if (leftshiftcheck <= 180) {
    int leftPos;
    // Serial.println("servoshiftleft works!!");
    for (leftPos = currentHorizontalServoPos; leftPos <= currentHorizontalServoPos + 5; leftPos += 5) {
      horizontalservo.write(leftPos);
    }
    currentHorizontalServoPos = leftPos; // updates the position of horizontal servo
  }

}

void ServoShiftRight() {
  follows a similar working as ServoShiftRight above.
  if (automaticTrace == true) {
    if (rightshiftcheck <= 0) {
      // motorShiftRight();
      motorShiftLeft();
    }
  }
  if (rightshiftcheck >= 0) {
    //Serial.println("servoshiftright works!!");
    int rightPos;
    for (rightPos = currentHorizontalServoPos; rightPos >= currentHorizontalServoPos - 5; rightPos -= 5) {
      horizontalservo.write(rightPos);
    }
    currentHorizontalServoPos = rightPos;
  }

}
// servoshiftup and servoshiftdown functions were supposed to be used for the servo responsible for vertical tracking but this function was not used due to a broken servo.
//void ServoShiftUp() {
//  Serial.println("servo up works 2");
//  if (upshiftcheck <= 180) {
//    int upPos;
//    for (upPos = currentVerticalServoPos; upPos <= currentVerticalServoPos + 10; upPos += 10) {
//      verticalservo.write(upPos);
//    }
//  }
//}
//void ServoShiftDown() {
//  Serial.print("servo down works");
//  if (downshiftcheck >= 20) {
//    int downPos;
//    for (downPos = currentVerticalServoPos; downPos >= currentVerticalServoPos - 10; downPos -= 10) {
//      verticalservo.write(downPos);
//    }
//  }
//}
// The enableTraceMode function fllows a trace pattern and when an object is found it resumes automatic tracking via the ultrasound sensors.
void enableTraceMode() {
  int HTraceServoPos = 0;
  //verticalservo.write(100);
  horizontalservo.write(0);
  currentHorizontalServoPos = HTraceServoPos;

  while (laserstatus == false) {
    Serial.print("fpllowtracepatternworks");
    setlaserstatus();
    if (laserstatus == true) {
      break;
    }
    followTracePattern();
  }
}
// followTracePattern function is used to control the servo such that it moves from left to right continously until the laser detects an object within its range.
void followTracePattern() {
  int pos;
  Serial.println("fpllowtracepatternworks");
  for ( pos = 0; pos <= 180; pos += 10) { // horizontal tracing only.
    horizontalservo.write(pos);
    currentHorizontalServoPos = pos;
    setlaserstatus();
    if (laserstatus == true) {
      break;
    }
  }

  // moves servo from left to right in steps of 10 degrees.
  for (pos = 180; pos >= 0; pos -= 10) {
    horizontalservo.write(pos);
    currentHorizontalServoPos = pos;
    if (laserstatus == true) {
      break;
    }
    setlaserstatus();
  }
}

void motorstop() {  // stops all the motors
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void ServoStop() {
  horizontalservo.write(currentHorizontalServoPos);
  verticalservo.write(currentVerticalServoPos);
}
void motorShiftLeft() {
  analogWrite(A0, 225);
  digitalWrite(A1, LOW);
  digitalWrite(A3, LOW);
  analogWrite(A2, 215);

}
void motorShiftRight() {
  analogWrite(A1, 225);
  digitalWrite(A0, LOW);
  digitalWrite(A2, LOW);
  analogWrite(A3, 215);
}

void bluetooth() {
  while (Serial1.available() > 0) {
    currentRead = Serial1.read();

    // Read all bytes until a zero is received, store the bytes received before the zero.
    if (currentRead != NULL) {
      lastRead = currentRead;
    }

    if (currentRead == NULL) {
      switch (lastRead) {
        case 255:
          Serial1.write(StatusLED);
          break;
        case 8:            // manual mode, allows for manual control to take over and disabble the automatic tracking mode.
          automaticTrace = false;
          manualControl = true;
          Serial1.write(StatusLED);
          break;
        case 18:         //automatic mode, allows for tracing to begin and the manual ocntrol to be permanently be disabled
          automaticTrace = true;
          manualControl = false;
          Serial1.write(StatusLED);
          break;
        case 35:                    // servoLeft
          ServoShiftLeft();
          Serial1.write(StatusLED);
          break;
        case 135:
          ServoStop();  // stops all servo movement an resists movement.
          Serial1.write(StatusLED);
          break;
        case 45:                    // servoRight
          ServoShiftRight();
          Serial1.write(StatusLED);
          break;
        case 145:
          ServoStop();
          Serial1.write(StatusLED);
          break;
        case 10:  // moves the tank motor front
          analogWrite(A0, 225);
          digitalWrite(A1, LOW);
          digitalWrite(A2, LOW);
          analogWrite(A3, 215);
          Serial1.write(StatusLED);
          break;
        case 110:
          motorstop(); // shuts off sending signals to the motors in the tank
          Serial1.write(StatusLED);
          break;
        case 20:  // moves the tank motor back
          analogWrite(A1, 225);
          digitalWrite(A0, LOW);
          digitalWrite(A3, LOW);
          analogWrite(A2, 215);
          Serial1.write(StatusLED);
          break;
        case 120:
          motorstop();
          Serial1.write(StatusLED);
          break;
        case 30:   // moves the tank motor left
          analogWrite(A0, 225);
          digitalWrite(A1, LOW);
          digitalWrite(A3, LOW);
          analogWrite(A2, 215);
          Serial1.write(StatusLED);
          break;
        case 130:
          motorstop();
          Serial1.write(StatusLED);
          break;
        case 40:   // moves the tank motor right
          analogWrite(A1, 225);
          digitalWrite(A0, LOW);
          digitalWrite(A2, LOW);
          analogWrite(A3, 215);
          Serial1.write(StatusLED);
          break;
        case 140:
          motorstop();
          Serial1.write(StatusLED);
          break;
        case 69:
          fire();
          Serial1.write(StatusLED);
        case 79:
          releaseFire(); 
          Serial1.write(StatusLED);
        case 28:
          Serial.println("trace==true works");
          trace = true;

          //enableTraceMode();
          Serial1.write(StatusLED);
      }
    }
  }
}
void fire() {                     // This function sends a high signal to the firepin in the arduino that triggers the relay which inturn allows current to flow through the metal rails.
  digitalWrite(firepin, HIGH);
}
void releaseFire() {              // Sends a low signal to the firepin to turn off the relay switch.
  digitalWrite(firepin, LOW);
}
