const int sensorPin = 39;

void setup() {
  Serial.begin(9600);   //iniciar puerto serie
  pinMode(sensorPin , INPUT);  //definir pin como entrada
}
 
void loop(){
  int value = 0;
  value = digitalRead(sensorPin );  //lectura digital de pin
 
  if (value == LOW) {
      Serial.println("Optointerruptor activado");
  }
  else {
      Serial.println("Optointerruptor Desactivado");
  }
  delay(1000);
}
