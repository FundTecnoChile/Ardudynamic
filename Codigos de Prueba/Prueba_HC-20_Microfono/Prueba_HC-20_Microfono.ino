const int pinLED = 30; 
const int pinMicrophone = 35;
bool state;

void setup()
{
  pinMode(pinLED, OUTPUT);
  pinMode(pinMicrophone, INPUT_PULLUP);
}

void loop()
{
  bool soundDetected = digitalRead(pinMicrophone); 
  if (soundDetected == true)
  {
    state = ! state;    
    digitalWrite(pinLED , state);
    delay (1000);
  }
  delay(10);
}
