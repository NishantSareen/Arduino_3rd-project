/*
=========== Comm Legend ===========
 10 - LED Off
 20 - LED On
255 - Request from app for local variable status
===================================
*/



byte lastRead;
byte currentRead;
byte StatusLED = 0;  // initialize LED in OFF state

void setup() {
  pinMode(A0,OUTPUT); // A0 and A1 connects to bottom of H-bridge. 
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT); // A2 and A3 connects to top of H-bridge.
  pinMode(A3,OUTPUT);
  
  Serial1.begin(9600);
}

void loop() {
  // if there's any serial bytes available, read them:
  while (Serial1.available() > 0) {
    currentRead = Serial1.read();
    
    // Read all bytes until a zero is received, store the bytes received before the zero.
    if (currentRead != NULL) {
            lastRead = currentRead;
    }
    
    if (currentRead == NULL) {
      switch(lastRead) {
        case 255:
          Serial1.write(StatusLED);
          break;
        case 10:            // front
         analogWrite(A0,225);
         digitalWrite(A1,LOW);
         digitalWrite(A2,LOW);
         analogWrite(A3,215);
         Serial1.write(StatusLED);
          break;
        case 110: 
          stop();
          Serial1.write(StatusLED);
          break;
        case 20:  // back
          analogWrite(A1,225);
          digitalWrite(A0,LOW);
          digitalWrite(A3,LOW);
          analogWrite(A2,215);
          Serial1.write(StatusLED);
          break;
        case 120: 
          stop();
          Serial1.write(StatusLED);
          break;
        case 30:   // left
          analogWrite(A0,225);
          digitalWrite(A1,LOW);
          digitalWrite(A3,LOW);
          analogWrite(A2,215);
          Serial1.write(StatusLED);
          break;
        case 130: 
          stop();
          Serial1.write(StatusLED);
          break;
        case 40: // right
          analogWrite(A1,225);
          digitalWrite(A0,LOW);
          digitalWrite(A2,LOW);
          analogWrite(A3,215);
          Serial1.write(StatusLED);
          break;
        case 140: 
          stop();
          Serial1.write(StatusLED);
          break;
        //default:
        //break;
      }
    }
  }
}

void stop(){
      digitalWrite(A0,LOW);
      digitalWrite(A1,LOW);
      digitalWrite(A2,LOW);
      digitalWrite(A3,LOW);
  }
