// Librerias Bases
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// Libreria y Configuracion Oled 0.96 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Libreria y Configuracion MPU6050
#include <Adafruit_MPU6050.h>
Adafruit_MPU6050 mpu;

// Configuracion Sensor de Luz
#define TEMT6000_PIN A0 

// Libreria y Configuracion Sensores MQ
#include <MQUnifiedsensor.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A6 //Entrada Analoga
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // Para Arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

// Libreria y Configuracion Ultrasonico
#include <NewPing.h>
const int UltrasonicPin = 4;
const int MaxDistance = 200;
NewPing sonar(UltrasonicPin, UltrasonicPin, MaxDistance);

//Dibujo de Flechas Laterales
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B00000000, 
  B00000001, B00000000, 
  B00000011, B00000000,  
  B00000111, B00000000, 
  B00001111, B00000000, 
  B00011111, B00000000, 
  B00111111, B00000000,  
  B01111111, B00000000,  
  B11111111, B00000000,  
  B01111111, B00000000,  
  B00111111, B00000000,  
  B00011111, B00000000,  
  B00001111, B00000000, 
  B00000111, B00000000,  
  B00000011, B00000000, 
  B00000001, B00000000, 
  };

static const unsigned char PROGMEM logo_bmp2[] =
{ B00000000, B00000000, 
  B00000000, B10000000, 
  B00000000, B11000000,  
  B00000000, B11100000, 
  B00000000, B11110000, 
  B00000000, B11111000, 
  B00000000, B11111100,  
  B00000000, B11111110,  
  B00000000, B11111111,  
  B00000000, B11111110,  
  B00000000, B11111100,  
  B00000000, B11111000,  
  B00000000, B11110000, 
  B00000000, B11100000,  
  B00000000, B11000000, 
  B00000000, B10000000, 
  };
// Declaracion de Variables

int inputPin1 = 22;
int inputPin2 = 26;
int inputPin3 = 24;
int inputPin4 = 18;
int Izq;
int Der;
int Enter;
int Back;
int Menu = 1;
int Menu2 = 1;
int Cleardisplaycount = 0;
const int encoderPin = 39;

void setup() {
  Serial.begin(9600);
  if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
  }
  Serial.println("Found a MPU-6050 sensor");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(30,20);
  display.println("FTC");
  display.display();
  delay(5000);
  MQ135.setRegressionMethod(1); 
  MQ135.init(); 
  Serial.print("Calibrating please wait.");
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(0,23);
  display.println("Calibrando Sensores");
  display.display();
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); 
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
  display.clearDisplay();
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(0,23);
  display.println("         LISTO!");
  display.display();
  delay(1000);
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  Serial.println("** Lectures from MQ-135 ****");
  Serial.println("|    CO   |  Alcohol |   CO2  |  Tolueno  |  NH4  |  Acteona  |");  

  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);
  pinMode(inputPin3, INPUT);
  pinMode(inputPin4, INPUT);
  pinMode(TEMT6000_PIN,  INPUT); 
  pinMode(encoderPin , INPUT); 
  display.clearDisplay();

}
void SensorGas(){
  if (Menu2 == 1){
     Serial.println("Pantalla 1");

    //Nombre Pantalla

     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(42,7);
     display.println("MQ-6");
     display.setCursor(60,28);
     display.println("&");
     display.setCursor(34,47);
     display.println("MQ-135");

    //Flechas Laterales

     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2  , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.display();
  }
  if (Menu2 == 2){
     MQ135.update(); 

     MQ135.setA(605.18); MQ135.setB(-3.937); 
     float CO = MQ135.readSensor(); 

     MQ135.setA(77.255); MQ135.setB(-3.18);
     float Alcohol = MQ135.readSensor();

     MQ135.setA(110.47); MQ135.setB(-2.862); 
     float CO2 = MQ135.readSensor(); 

     MQ135.setA(44.947); MQ135.setB(-3.445); 
     float Tolueno = MQ135.readSensor(); 

     MQ135.setA(102.2 ); MQ135.setB(-2.473); 
     float NH4 = MQ135.readSensor(); 

     MQ135.setA(34.668); MQ135.setB(-3.369);
     float Acetona = MQ135.readSensor(); 

     Serial.print("|   "); Serial.print(CO); 
     Serial.print("   |   "); Serial.print(Alcohol); 
     Serial.print("   |   "); Serial.print(CO2); 
     Serial.print("   |   "); Serial.print(Tolueno); 
     Serial.print("   |   "); Serial.print(NH4); 
     Serial.print("   |   "); Serial.print(Acetona);
     Serial.println("   |"); 

     display.setTextSize(0);
     display.setTextColor(WHITE);

     //Titulo Pantalla Concetracion PPM

     display.setCursor(5,0);
     display.println("Concentracion - PPM");

     //Barras Horizontales de Borde a Borde

     display.setCursor(0,8);
     display.println("---------------------");
     display.setCursor(0,20);
     display.println("---------------------");

     display.setCursor(50,14);
     display.println("MQ-135");
     display.setCursor(69,34);
     display.println("-Tol:");
     display.setCursor(0,25);
     display.println("-CO:");
     display.setCursor(69,25);
     display.println("-Ace:");
     display.setCursor(0,34);
     display.println("-CO2:");
     display.setCursor(0,43);
     display.println("-Eta:");
     display.setCursor(69,43);
     display.println("-NH4:");

     //Menu Inferior

     display.setCursor(90,50);
     display.println("------");
     display.setCursor(0,50);
     display.println("------");
     display.setCursor(32,55);
     display.println("|");
     display.setCursor(88,55);
     display.println("|");
     display.setCursor(1,55);
     display.println(" Back");
     display.setCursor(95,55);
     display.println("Enter");
     display.display();
     
     //Mostramos en Display Los Valores del Sensor


     display.setCursor(100,34);
     display.println(Tolueno);
     display.setCursor(23,25);
     display.println(CO);
     display.setCursor(100,25);
     display.println(Acetona);
     display.setCursor(29,34);
     display.println(CO2);
     display.setCursor(30,43);
     display.println(Alcohol);
     display.setCursor(100,43);
     display.println(NH4);
     display.display();
     display.clearDisplay();
     delay(50);
  }
  if (Menu2 == 3){
     display.setTextSize(0);
     display.setTextColor(WHITE);

     // Titulo Pantalla Concentracion PPM

     display.setCursor(5,0);
     display.println("Concentracion - PPM");

    // Barras Horizontales De Borde a Borde

     display.setCursor(0,8);
     display.println("---------------------");
     display.setCursor(0,20);
     display.println("---------------------");

     display.setCursor(50,14);
     display.println("MQ-6");

     //Menu Inferior

    // display.setCursor(90,50);
    // display.println("------");
     display.setCursor(0,50);
     display.println("------");
     display.setCursor(32,55);
    // display.println("|");
    // display.setCursor(88,55);
     display.println("|");
     display.setCursor(1,55);
     display.println(" Back");
     // display.setCursor(95,55);
     // display.println("Enter");
     display.display();

  }
}
void SensorAudio(){
  if (Menu2 == 1){
    Serial.println("Pantalla 2");

    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(35,28);
    display.println("HC-20");

    //Flechas Laterales

    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();
  }
  if (Menu2 == 2){
    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    //display.setCursor(90,50);
    //display.println("------");
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    //display.setCursor(88,55);
    //display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    //display.setCursor(95,55);
    //display.println("Enter");
    display.display();
  }
}
void SensorUltrasonic(){
  if (Menu2 == 1){
    Serial.println("Pantalla 3");

    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(25,28);
    display.println("HC-SR04");

    //Flechas Laterales

    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();
  }
  if (Menu2 == 2){
    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    //display.setCursor(90,50);
    //display.println("------");
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    //display.setCursor(88,55);
    //display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    //display.setCursor(95,55);
    //display.println("Enter");

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(12,5);
    display.println("Distancia");
    display.setCursor(70,26);
    display.println("Cm");
    display.display();

     //Mostramos en Display Los Valores del Sensor

    display.setCursor(25,26);
    display.println(sonar.ping_cm());
    display.display();
    display.clearDisplay();
    delay(50);

  }
}
void SensorAcelGyro(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (Menu2 == 1){
    Serial.println("Pantalla 4");

    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(23,28);
    display.println("MPU6050");
    display.setCursor(0,12);
    
    //Flechas Laterales

    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();
  }
  if (Menu2 == 2){
    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    display.setCursor(90,50);
    display.println("------");
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(88,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.setCursor(95,55);
    display.println("Enter");

    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("     Aceleracion     ");
    display.setCursor(0, 4);
    display.println("_____________________");

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 15);
    display.println(" X   Y   Z");
    display.setCursor(45, 47);
    display.println("m/s");
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(80, 47);
    display.println("2");
    display.display();
    
    // Mostramos en Display Valores del Sensor

    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(8, 34);
    display.print(a.acceleration.x, 1);
    
    display.setCursor(56, 34);
    display.print(a.acceleration.y, 1);
    
    display.setCursor(100, 34);
    display.print(a.acceleration.z, 1);
    
    display.display();
    display.clearDisplay();
    delay(100);
  }
  if (Menu2 == 3){
    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    //display.setCursor(90,50);
    //display.println("------");
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    //display.setCursor(88,55);
    //display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    //display.setCursor(95,55);
    //display.println("Enter");

    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("         Gyro       ");
    display.setCursor(0, 4);
    display.println("_____________________");

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 15);
    display.println(" X   Y   Z");
    display.display();
    
    // Mostramos en Display Valores del Sensor

    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(8, 34);
    display.print(g.gyro.x, 1);
    
    display.setCursor(56, 34);
    display.print(g.gyro.y, 1);
    
    display.setCursor(100, 34);
    display.print(g.gyro.z, 1);

    display.display();
    display.clearDisplay();
    delay(100);
  }
}
void SensorLuz(){
  float lux2 = analogRead(TEMT6000_PIN) * 0.9765625;
  if (Menu2 == 1){
    Serial.println("Pantalla 5");

    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(15,28);
    display.println("TEMP6000");

    //Flechas Laterales
    
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) /1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();
  }
  if (Menu2 == 2){
    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    //display.setCursor(90,50);
    //display.println("------");
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    //display.setCursor(88,55);
    //display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    //display.setCursor(95,55);
    //display.println("Enter");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,24);
    display.println("LUX:");
    display.display();

    //Mostramos en Display Los Valores del Sensor

    display.setCursor(50,24);
    display.println(lux2);
    display.display();
    display.clearDisplay();
    delay(50);
  }
}
void SensorEncoder(){
  Serial.println("Pantalla 6");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30,5);
  display.println("Encoder/Motor");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void SensorPulsoSpO2(){
  Serial.println("Pantalla 7");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("MAX30102");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void LedNeoPixel(){
  Serial.println("Pantalla 8");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("WS2812b");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void NormalLeds(){
  Serial.println("Pantalla 9");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(50,5);
  display.println("LEDs");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void ServoMoved(){
  Serial.println("Pantalla 10");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(35,5);
  display.println("Servo Motor");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void PuenteHmotor(){
  Serial.println("Pantalla 11");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(30,5);
  display.println("PuenteH/Motor");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void LiquidCrystalFunc(){
  Serial.println("Pantalla 12");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(55,5);
  display.println("LCD");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void BuzzerMelody(){
  Serial.println("Pantalla 13");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("Buzzer");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void Created_Engine(){
  Serial.println("Pantalla 14");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("Creadores");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
}
void loop() {
  Izq=digitalRead(inputPin1);
  Der=digitalRead(inputPin2);
  Enter=digitalRead(inputPin3);
  Back=digitalRead(inputPin4);

  if((Der == HIGH) && (Menu2 == 1)){
     Menu = Menu + 1;
     if(Menu == 15){
       Menu = Menu - 14;
     }
     if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if((Izq == HIGH) && (Menu2 == 1)){
     Menu = Menu - 1;
     if(Menu == 0){
       Menu = Menu + 14;
     }
     if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if((Enter == HIGH) && (Menu2 != 3)){
    Menu2 = Menu2 + 1;
    if (Menu2  == 4){
      Menu2 = Menu2 - 1;
    }
    if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if((Back == HIGH) && (Menu2 != 1)){
    Menu2 = Menu2 - 1;
    if (Menu2  == 0){
      Menu2 = Menu2 + 1;
    }
    if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if(Menu == 1){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    SensorGas();
    delay(100);
  }
  if(Menu == 2){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
    }
    SensorAudio();
    delay(100);
  }
  if(Menu == 3){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
    }
    SensorUltrasonic();
    delay(100);
  }
  if(Menu == 4){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    SensorAcelGyro();
    delay(100);
  }
  if(Menu == 5){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
    }
    SensorLuz();
    delay(100);
  }
  if(Menu == 6){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    SensorEncoder();
    delay(100);
  }
  if(Menu == 7){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    SensorPulsoSpO2();
    delay(100);
  }
  if(Menu == 8){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    LedNeoPixel();
    delay(100);
  }
  if(Menu == 9){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    NormalLeds();
    delay(100);
  }
  if(Menu == 10){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    ServoMoved();
    delay(100);
  }
  if(Menu == 11){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    PuenteHmotor();
    delay(100);
  }
  if(Menu == 12){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    LiquidCrystalFunc();
    delay(100);
  }
  if(Menu == 13){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    BuzzerMelody();
    delay(100);
  }
  if(Menu == 14){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
    }
    Created_Engine();
    delay(100);
  }
}
