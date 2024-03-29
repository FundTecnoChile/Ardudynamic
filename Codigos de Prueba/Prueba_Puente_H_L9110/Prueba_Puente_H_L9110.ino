/*
 L9110 motor driver controlando 2 motores CC
 COPIAR Y PEGAR SOBRE EL SKETCH DE ARDUINO
*/

const int AIA = 47;  // (pwm) pin 6 conectado a pin A-IA 
const int AIB = 45;  // (pwm) pin 5 conectado a pin A-IB 
const int BIA = 43; // (pwm) pin 10 conectado a pin B-IA  
const int BIB = 41;  // (pwm) pin 9 conectado a pin B-IB 

byte velocidad = 160;  
// cambie este valor (100 a 255) para controlar 
// la velocidad de los motores 

void setup() {
  pinMode(AIA, OUTPUT); // fijar los pines como salidas
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);
}

void loop() {
  izquierda();
  delay(2000);
  derecha();
  delay(2000);
  avanzar();
  delay(500);
  retroceder();
  delay(1000);
  avanzar();
  delay(500);
}

void avanzar()
{
  analogWrite(AIA, 0);
  analogWrite(AIB, velocidad);
  analogWrite(BIA, 0);
  analogWrite(BIB, velocidad);
}

void retroceder()
{
  analogWrite(AIA, velocidad);
  analogWrite(AIB, 0);
  analogWrite(BIA, velocidad);
  analogWrite(BIB, 0);
}

void izquierda()
{
  analogWrite(AIA, velocidad);
  analogWrite(AIB, 0);
  analogWrite(BIA, 0);
  analogWrite(BIB, velocidad);
}

void derecha()
{
  analogWrite(AIA, 0);
  analogWrite(AIB, velocidad);
  analogWrite(BIA, velocidad);
  analogWrite(BIB, 0);
}
