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

// Libreria y Configuracion Sensor MAX30102

#include "MAX30105.h"
#include "spo2_algorithm.h"

MAX30105 particleSensor;

#define MAX_BRIGHTNESS 255

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//Arduino Uno doesn't have enough SRAM to store 100 samples of IR led data and red led data in 32-bit format
//To solve this problem, 16-bit MSB of the sampled data will be truncated. Samples become 16-bit data.
uint16_t irBuffer[100]; //infrared LED sensor data
uint16_t redBuffer[100];  //red LED sensor data
#else
uint32_t irBuffer[100]; 
uint32_t redBuffer[100];  
#endif

int32_t bufferLength; 
int32_t spo2; 
int8_t validSPO2; 
int32_t heartRate; 
int8_t validHeartRate; 

byte pulseLED = 11; 
byte readLED = 13; 

int Pulso;
int oxigeno; 

// Libreria y Configuracion Sensores MQ
#include <MQUnifiedsensor.h>
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A6 //Entrada Analoga
#define typemq "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // Para Arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, typemq);

// Libreria y Configuracion Ultrasonico
#include <NewPing.h>
const int UltrasonicPin = 4;
const int MaxDistance = 200;
NewPing sonar(UltrasonicPin, UltrasonicPin, MaxDistance);

// LED WS2812b

#include <Adafruit_NeoPixel.h>
#define PINRGB        25 
#define NUMPIXELS 16 
Adafruit_NeoPixel pixels(NUMPIXELS, PINRGB, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 100


//Servo

#include <Servo.h>
Servo myservo;  
int pos = 0;  
int pos0 = 0;  

//LCD 

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//Buzzer

#include "pitches.h"
int melodia[] = {
  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, 
  NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5, 
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5, 
  NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
};
int duraciones[] = {
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8, 
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8
};
int sonidoLength = sizeof(melodia)/sizeof(melodia[0]);

//DHT11

#include <DHT.h>
#define DHTPIN 37
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

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

static const unsigned char PROGMEM Heart_bmp[] =
{ B00000000, B00000000, 
  B00000000, B00000000, 
  B00111000, B00011100,  
  B01111100, B00111110, 
  B11111110, B01111111, 
  B11111110, B11111111, 
  B01111111, B11111110,  
  B00111111, B11111100,  
  B00011111, B11111000,  
  B00001111, B11110000,  
  B00000111, B11100000,  
  B00000011, B11000000,  
  B00000001, B00000000, 
  B00000000, B00000000,  
  B00000000, B00000000, 
  B00000000, B00000000, 
};

// Declaracion de Variables

// Botones
int inputPin1 = 22;
int inputPin2 = 26;
int inputPin3 = 24;
int inputPin4 = 18;
// Uso de Codigo
int Izq;
int Der;
int Enter;
int Back;
int Menu = 1;
int Menu2 = 1;
int Cleardisplaycount = 0;
// Pines
const int encoderPin = 39;
const int AIA = 47;  
const int AIB = 45;  
const int BIA = 43; 
const int BIB = 41;

// cambie este valor (100 a 255) para controlar 
// la velocidad de los motores 
byte velocidad = 100;  

void setup() {
  Serial.begin(115200);
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

  // Sensor MAX30102

  pinMode(pulseLED, OUTPUT);
  pinMode(readLED, OUTPUT);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  byte ledBrightness = 60; //Options: 0=Off to 255=50mA
  byte sampleAverage = 4; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
  //WS2812b

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  

  // Sensores MQ
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  Serial.println("** Lectures from MQ-135 ****");
  Serial.println("|    CO   |  Alcohol |   CO2  |  Tolueno  |  NH4  |  Acteona  |");  
  
  //LEDS RGB

  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(27, OUTPUT);

  //Servo
  
  myservo.attach(34);

  //LCD

  lcd.init();

  //DHT11

  dht.begin();

  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);
  pinMode(inputPin3, INPUT);
  pinMode(inputPin4, INPUT);
  pinMode(TEMT6000_PIN,  INPUT); 
  pinMode(encoderPin , INPUT); 
  pinMode(AIA, OUTPUT); // fijar los pines como salidas
  pinMode(AIB, OUTPUT);
  pinMode(BIA, OUTPUT);
  pinMode(BIB, OUTPUT);

  display.clearDisplay();
  pixels.clear();
  pixels.show();

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

     display.setCursor(0,50);
     display.println("------");
     display.setCursor(32,55);
     display.println("|");
     display.setCursor(1,55);
     display.println(" Back");
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

    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
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

    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    
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

    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");

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

    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");

    display.setCursor(1,55);
    display.println(" Back");

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
  
  if (Menu2 == 1){
    
    Serial.println("Pantalla 6");
    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(27,7);
    display.println("Encoder");
    display.setCursor(60,28);
    display.println("&");
    display.setCursor(34,47);
    display.println("Motor");

    //Flechas Laterales

    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2  , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();
  }
  if (Menu2 == 2){

    //Menu Inferior

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.display();
  }
}
void SensorPulsoSpO2(){
  if (Menu2 == 1){
     Serial.println("Pantalla 7");

     //Nombre Pantalla

     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(17,28);
     display.println("MAX30102");

     //Flechas Laterales
    
     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.drawBitmap((display.width()  - LOGO_WIDTH ) /1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.display();
    }
  if (Menu2 == 2){
  
     display.setTextSize(0);
     display.setTextColor(WHITE);

     //Menu Inferior

 
     display.setCursor(0,50);
     display.println("------");
     display.setCursor(32,55);
     display.println("|");

     display.setCursor(1,55);
     display.println(" Back");


     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(12,5);
     display.println("PPM | H2O");
     display.display();
     
     if(particleSensor.getIR() <= 50000 && Pulso != 0 && oxigeno != 0 && heartRate <= 300 && heartRate >= 0 && spo2 <= 100 && spo2 >= 0){

       display.setCursor(1,26);
       display.println(Pulso, DEC);
       display.setCursor(70,26);
       display.println(oxigeno, DEC);
       display.setCursor(110,26);
       display.println("%");
       display.drawBitmap((display.width()  - LOGO_WIDTH ) /2.8 , (display.height() - LOGO_HEIGHT) / 2 , Heart_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
       display.display();
       
     }

      while (particleSensor.getIR() >= 50000)
    {
    
      bufferLength = 100; //buffer length of 100 stores 4 seconds of samples running at 25sps

      //read the first 100 samples, and determine the signal range
      for (byte i = 0 ; i < bufferLength ; i++)
      {
       while (particleSensor.available() == false) //do we have new data?
      particleSensor.check(); //Check the sensor for new data

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.println(irBuffer[i], DEC);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(12,28);
      display.println("Espere...");
      display.display();
      display.clearDisplay();
      }

      //calculate heart rate and SpO2 after first 100 samples (first 4 seconds of samples)
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

      //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
      while (1)
      {
      //dumping the first 25 sets of samples in the memory and shift the last 75 sets of samples to the top
      for (byte i = 25; i < 100; i++)
      {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
      }

      //take 25 sets of samples before calculating the heart rate.
      for (byte i = 75; i < 100; i++)
      {
      while (particleSensor.available() == false) //do we have new data?
        particleSensor.check(); //Check the sensor for new data

      digitalWrite(readLED, !digitalRead(readLED)); //Blink onboard LED with every data read
      
      display.setTextSize(0);
      display.setTextColor(WHITE);
      display.setCursor(0,50);
      display.println("------");
      display.setCursor(32,55);
      display.println("|");
      display.setCursor(1,55);
      display.println(" Back");
      

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(12,5);
      display.println("PPM | H2O");
      display.display();
     
      display.drawBitmap((display.width()  - LOGO_WIDTH ) /2.8 , (display.height() - LOGO_HEIGHT) / 2 , Heart_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
      display.display();

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //We're finished with this sample so move to next sample

      //send samples and calculation result to terminal program through UART
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.print(irBuffer[i], DEC);

      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);

      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);

      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);
      
      
      Serial.print(heartRate);
      

      if (Pulso != heartRate){
        Pulso = heartRate;
        display.clearDisplay();
      }
      
      if (oxigeno != spo2){
        oxigeno = spo2;
        display.clearDisplay();
      }

      if(heartRate <= 300 && heartRate >= 0 && spo2 <= 100 && spo2 >= 0){
       display.setCursor(1,26);
       display.println(Pulso, DEC);
       display.setCursor(70,26);
       display.println(oxigeno, DEC);
       display.setCursor(110,26);
       display.println("%");
       display.display();
      // display.clearDisplay();
       Serial.print(Pulso);
      }

      }
      if (particleSensor.getIR() <= 50000){
        break;
      }

      //After gathering 25 new samples recalculate HR and SP02
      maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
      }
      
      
    }
  
   
  }

}
void LedNeoPixel(){
    Serial.println("Pantalla 8");
    if (Menu2 == 1){
     //Nombre Pantalla

     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(25,28);
     display.println("WS2812b");

     //Flechas Laterales
    
     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.drawBitmap((display.width()  - LOGO_WIDTH ) /1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.display();
     pixels.clear();
     pixels.show();
    }
    if (Menu2 == 2){
    
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(35,20);
      display.println("Verde");
      display.setTextSize(0);
      display.setTextColor(WHITE);

      //Menu Inferior

      display.setTextSize(0);
      display.setTextColor(WHITE);
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
      display.println("Rojo");
      display.display();

      pixels.clear(); 
      for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(0, 200, 0));
      pixels.show();  
      delay(DELAYVAL); 
     }
    }
    if (Menu2 == 3){
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(40,20);
      display.println("Rojo");
      
      //Menu Inferior
      
      display.setTextSize(0);
      display.setTextColor(WHITE);
      display.setCursor(90,50);
      display.println("------");
      display.setCursor(0,50);
      display.println("------");
      display.setCursor(32,55);
      display.println("|");
      display.setCursor(88,55);
      display.println("|");
      display.setCursor(1,55);
      display.println("Verde");
      display.setCursor(95,55);
      display.println("Azul");
      display.display();

      pixels.clear(); 
      for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(200, 0, 0));
      pixels.show();   
      delay(DELAYVAL); 
     }
    }
    if (Menu2 == 4){

      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(40,20);
      display.println("Azul");
      display.setTextSize(0);
      display.setTextColor(WHITE);

      //Menu Inferior


      display.setCursor(0,50);
      display.println("------");
      display.setCursor(32,55);
      display.println("|");
      display.setCursor(1,55);
      display.println(" Rojo");
      display.display();

      pixels.clear(); 
      for(int i=0; i<NUMPIXELS; i++) { 
      pixels.setPixelColor(i, pixels.Color(0, 0, 200));
      pixels.show();  
      delay(DELAYVAL); 
     }
    }
}
void NormalLeds(){
  if (Menu2 == 1){
     Serial.println("Pantalla 9");

     //Nombre Pantalla

     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(40,28);
     display.println("LEDs");

     //Flechas Laterales
    
     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.drawBitmap((display.width()  - LOGO_WIDTH ) /1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.display();
    }

  if (Menu2 == 2){
    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

 
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.display();
    delay(2000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(30,20);
    display.println("Blanco");
    display.display();
   
    digitalWrite(30, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40,20);
    display.println("Rojo");
    display.display();

    digitalWrite(30, LOW);   // turn the LED on (HIGH is the voltage level)
    digitalWrite(31, HIGH);
    delay(1000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(35,20);
    display.println("Verde");
    display.display();

    digitalWrite(31, LOW);
    digitalWrite(29, HIGH);
    delay(1000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40,20);
    display.println("Azul");
    display.display();

    digitalWrite(29, LOW);
    digitalWrite(27, HIGH);
    delay(1000);

    digitalWrite(27, LOW);
    display.clearDisplay();
    display.display();
  }

}
void ServoMoved(){
  if (Menu2 == 1){
    
    Serial.println("Pantalla 10");
    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(35,17);
    display.println("Servo");
    display.setCursor(34,37);
    display.println("Motor");

    //Flechas Laterales

    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2  , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();

    myservo.write(pos0); 
  }
  if (Menu2 == 2){

    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");

    display.setCursor(1,55);
    display.println(" Back");

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,24);
    display.println("Angle:");
    display.display();

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(80,24);
    display.println(pos);
    display.setCursor(10,24);
    display.println("Angle:");
    display.display();
    display.clearDisplay();
    delay(15);                       // waits 15ms for the servo to reach the position
    Back=digitalRead(inputPin4);
    if(Back == HIGH){
      break;
      Menu2 = Menu2 - 1;
    }
  }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    display.setTextSize(0);
    display.setTextColor(WHITE);
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(80,24);
    display.println(pos);
    display.setCursor(10,24);
    display.println("Angle:");
    display.display();
    display.clearDisplay();
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    Back=digitalRead(inputPin4);
    if(Back == HIGH){
      break;  
      Menu2 = Menu2 - 1;
    }
  }
  
  }
}
void PuenteHmotor(){
  if (Menu2 == 1){
    
    Serial.println("Pantalla 11");
    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(27,7);
    display.println("Puente");
    display.setCursor(60,28);
    display.println("H");
    display.setCursor(34,47);
    display.println("Motor");

    //Flechas Laterales

    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2  , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.drawBitmap((display.width()  - LOGO_WIDTH ) / 1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
    display.display();
  }
  if (Menu2 == 2){

    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

 
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.display();
    
  }
}
void LiquidCrystalFunc(){
  if (Menu2 == 1){
     Serial.println("Pantalla 12");

     //Nombre Pantalla

     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(45,28);
     display.println("LCD");

     //Flechas Laterales
    
     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.drawBitmap((display.width()  - LOGO_WIDTH ) /1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.display();
     
     lcd.backlight();
     lcd.setCursor(0,0);
     lcd.print("                ");
     lcd.setCursor(0,1);
     lcd.print("                ");

    }
  if (Menu2 == 2){

    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

 
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.display();

    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("  Hello,");
    lcd.setCursor(0,1);
    lcd.print("        world!");
  }
}
void BuzzerMelody(){
  if (Menu2 == 1){
    Serial.println("Pantalla 13");

    //Nombre Pantalla

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(30,28);
    display.println("BUZZER");
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

 
    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    display.display();

   /*
    for (int estanota = 0; estanota < sonidoLength; estanota++){
    // determine the duration of the notes that the computer understands
    // divide 1000 by the value, so the first note lasts for 1000/8 milliseconds
    int duracion = 1000/ duraciones[estanota];
    tone(50, melodia[estanota], duracion);
    // pause between notes
    int pausa = duracion * 1.3;
    delay(pausa);
    // stop the tone
    noTone(8);
    }
    */
  }
}
void DHT_11(){
  
    float h = dht.readHumidity();
    float t = dht.readTemperature();

  if (Menu2 == 1){
    
     Serial.println("Pantalla 12");

     //Nombre Pantalla

     display.setTextSize(2);
     display.setTextColor(WHITE);
     display.setCursor(35,28);
     display.println("DHT11");

     //Flechas Laterales
    
     display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
     display.drawBitmap((display.width()  - LOGO_WIDTH ) /1 , (display.height() - LOGO_HEIGHT) / 2 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
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

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40,1);
    display.println("Temp:");
    display.setCursor(100,24);
    display.println("C");
    display.display();


    display.setCursor(20,24);
    display.println(t);
    display.display();
    display.clearDisplay();
    delay(50);

  }
  if (Menu2 == 3){

    display.setTextSize(0);
    display.setTextColor(WHITE);

    //Menu Inferior

    display.setCursor(0,50);
    display.println("------");
    display.setCursor(32,55);
    display.println("|");
    display.setCursor(1,55);
    display.println(" Back");
    

    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(40,1);
    display.println("Hum%:");
    display.setCursor(100,24);
    display.println("%");
    display.display();


    display.setCursor(20,24);
    display.println(h);
    display.display();
    display.clearDisplay();
    delay(50);


  }
}
void loop() {
  Izq=digitalRead(inputPin1);
  Der=digitalRead(inputPin2);
  Enter=digitalRead(inputPin3);
  Back=digitalRead(inputPin4);

  if((Der == HIGH) && (Menu2 == 1)){
     Menu = Menu + 1;
     if(Menu == 15){
       Menu = Menu - 1;
     }
     if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if((Izq == HIGH) && (Menu2 == 1)){
     Menu = Menu - 1;
     if(Menu == 0){
       Menu = Menu + 1;
     }
     if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if((Enter == HIGH) && (Menu2 != 5)){
    Menu2 = Menu2 + 1;
    if (Menu2  == 5){
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
    if (Menu2 == 4){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 4){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 5){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 3){
      Menu2 = Menu2 - 1;
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
    if (Menu2 == 4){
      Menu2 = Menu2 - 1;
    }
    DHT_11();
    delay(100);
  }
}
