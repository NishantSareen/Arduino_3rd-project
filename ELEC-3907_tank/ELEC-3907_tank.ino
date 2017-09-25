


int enablePin1=2;
int enablePin2=3;


void setup() {
  Serial.begin(9600);
  pinMode(A0,OUTPUT); // A0 and A1 connects to bottom of H-bridge. 
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT); // A2 and A3 connects to top of H-bridge.
  pinMode(A3,OUTPUT);
  // set enable pin high so the motor can turn on
  digitalWrite(enablePin1,HIGH);
  digitalWrite(enablePin2,HIGH);

}

void loop() {
  if (Serial.available() > 0)
  {
    int ByteReceived = Serial.read();
    Serial.print(ByteReceived);   
    
    
    switch(ByteReceived){
      case 48:                     // type 0 , stop
      digitalWrite(A0,LOW);
      digitalWrite(A1,LOW);
      digitalWrite(A2,LOW);
      digitalWrite(A3,LOW);
      Serial.print(" stop ");
      break;
      
      case 49:                     // type 1 , front
      analogWrite(A0,215);
      digitalWrite(A1,LOW);
      digitalWrite(A2,LOW);
      analogWrite(A3,215);
      Serial.print(" front ");
      break;
      
      case 50:                     // type 2 , back
      analogWrite(A1,215);
      digitalWrite(A0,LOW);
      digitalWrite(A3,LOW);
      analogWrite(A2,215);
      Serial.print(" back ");
      break;
      
      case 51:                     // type 3 , left
      analogWrite(A1,215);
      digitalWrite(A0,LOW);
      digitalWrite(A2,LOW);
      analogWrite(A3,215);
      Serial.print(" left ");
      break;
      
      case 52:                     // type 4 , right
      analogWrite(A0,215);
      digitalWrite(A1,LOW);
      digitalWrite(A3,LOW);
      analogWrite(A2,215);
      break;
      
      }
      delay(500);
    
    }
  }



