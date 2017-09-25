/*const int analogPin = 22;
  int analogValue = 0;
  void setup() {
  Serial.begin(9600);
  }
  void loop() {
  analogValue = analogRead(analogPin);
  //analogValue = map(analogValue, 0, 1023, 0, 20);
  Serial.print("analog value = " );
  Serial.println(analogValue, DEC);
  delay(400);
  }*/
//This laser sensor detects weather an obstacle is within its specfied range of approximately 80 cm.
int laser_din = 7; // pin connected to the arduino board

void setup()
{
  pinMode(laser_din, INPUT); // only communication pin to the laser in an input signal wire

  Serial.begin(9600);
}
void loop()
{
  if (digitalRead(laser_din) == LOW)
  {
    Serial.println("Obstacles!");

  }
  else
  {
    Serial.println("No Obstacles!");

  }
  delay(500);
}
