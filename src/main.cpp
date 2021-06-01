#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Programado desde Mac
//Probando Otra Modificacion

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

int inputPin1 = 22;
int inputPin2 = 26;
int Izq;
int Der;
int Menu = 1;
int Cleardisplaycount = 0;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(20,20);
  display.println("LOGO");
  display.display();
  pinMode(inputPin1, INPUT);
  pinMode(inputPin2, INPUT);
  delay(5000);
  display.clearDisplay();

}
void SensorGas(){
  Serial.println("Pantalla 1");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(26,5);
  display.println("MQ-6 & MQ-135");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void SensorAudio(){
  Serial.println("Pantalla 2");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(50,5);
  display.println("HC-20");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void SensorUltrasonic(){
  Serial.println("Pantalla 3");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("HC-SR04");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void SensorAcelGyro(){
 Serial.println("Pantalla 4");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("MPU6050");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
}
void SensorLuz(){
  Serial.println("Pantalla 5");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(44,5);
  display.println("TEMP6000");
  display.setCursor(0,12);
  display.println("_____________________");
  display.drawBitmap((display.width()  - LOGO_WIDTH ) / 128  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.drawBitmap((display.width()  - LOGO_WIDTH )  , (display.height() - LOGO_HEIGHT) / 64 , logo_bmp2, LOGO_WIDTH, LOGO_HEIGHT, 2);
  display.display();
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
  if(Der == HIGH){
     Menu = Menu + 1;
     if(Menu == 15){
       Menu = Menu - 14;
     }
     if(Cleardisplaycount == 1){
        Cleardisplaycount = Cleardisplaycount - 1;
     }
  }
  if(Izq == HIGH){
     Menu = Menu - 1;
     if(Menu == 0){
       Menu = Menu + 14;
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
    SensorAudio();
    delay(100);
  }
  if(Menu == 3){
    if(Cleardisplaycount == 0){
      display.clearDisplay();
      display.display();
      Cleardisplaycount = Cleardisplaycount + 1;
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
