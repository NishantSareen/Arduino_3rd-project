
//int trigReading;
long duration;
long distance;

int echoPinL = 49;
int trigPinL = 47;
int echoPinR = 33;
int trigPinR = 31;
int trigPinB = 37;
int echoPinB = 39;

int laser_din=7;
boolean left;
boolean right;
boolean bottom;
boolean laserstatus;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(38400);
  pinMode(laser_din,INPUT);
  pinMode(echoPinL,INPUT);
  pinMode(trigPinL,OUTPUT);
  pinMode(echoPinR,INPUT);
  pinMode(trigPinR,OUTPUT);
  pinMode(echoPinB,INPUT);
  pinMode(trigPinB,OUTPUT);
}

void loop() {
 int dleft= finddistance(trigPinL,echoPinL);
 int dright=finddistance(trigPinR,echoPinR);
 int dbottom= finddistance(trigPinB,echoPinB);
 Serial.print("Left distance:");
 Serial.print(dleft,DEC);
 Serial.print("  Right distance:");
 Serial.print(dright,DEC);
 Serial.print("  Bottom distance:");
 Serial.println(dbottom,DEC);
setultrasoundstatus(left,dleft);
setultrasoundstatus(right,dright);
setultrasoundstatus(bottom,dbottom);
// add bottom sensor as well
//setlaserstatus();
  
  delay(250);
}


// Functions

long finddistance(int trigPin, int echoPin){ 
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  return distance = duration/58; // inches  
  }

void setultrasoundstatus(boolean sensor,int sensorFeed){ 
  if(sensorFeed<210){
  sensor=true;
    }
  else{
    sensor = false;};
   }
void setlaserstatus(){
    if(digitalRead(laser_din)==LOW)
  {
    laserstatus=true;
   // Serial.println("Obstacles!");
    
  }
  else
  {
    laserstatus=false;
    //Serial.println("No Obstacles!");    
  }
    
    } 
 
    
