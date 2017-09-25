void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

void motorControl() {
  // if there's any serial bytes available, read them:
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
        case 10:            // front
          analogWrite(A0, 225);
          digitalWrite(A1, LOW);
          digitalWrite(A2, LOW);
          analogWrite(A3, 215);
          Serial1.write(StatusLED);
          break;
        case 110:
          motorstop();
          Serial1.write(StatusLED);
          break;
        case 20:  // back
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
        case 30:   // left
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
        case 40: // right
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
          //default:
          //break;
      }
    }
  }
}
