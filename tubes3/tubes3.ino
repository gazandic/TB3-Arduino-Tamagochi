#include "SPI.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define counterTreshold 5
#define energycycletreshold 200
#define LED_PIN 10

boolean isEat=false;
boolean isSleep = false;
int lamp=0;
int sleepy;
const int sleepyTeshold = 50;
const int sensorTreshold = 25;
int energy;
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int8_t threshold, count; 
float temp;
bool zero_detect; 
bool TurnOnZI = false;
uint8_t level;
bool XnegMD, XposMD, YnegMD, YposMD, ZnegMD, ZposMD;
bool blinkState = false;
int8_t value;
int8_t a =0;
int8_t counter =0, energycycle=0;
uint8_t experience = 0;
String inputString = ""; 

byte dino1[8]={B00000,B00000,B00000,B00000,B10000,B11000,B11100,B11111};
byte dino2[8]={B00000,B00001,B00001,B00001,B00011,B00111,B01111,B11111};
byte dino3[8]={B11110,B10111,B11111,B11000,B11110,B10000,B10000,B00000};
byte dino4[8]={B11111,B01111,B00111,B00001,B00001,B00000,B00000,B00000};
byte dino5[8]={B11111,B11111,B11111,B11111,B11111,B10010,B10010,B11011};
byte dino6[8]={B11111,B10011,B00000,B00000,B00000,B00000,B00000,B00000};
byte babydino1[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte babydino2[8]={B00000,B00000,B00000,B00011,B00111,B00111,B00111,B00111};
byte babydino3[8]={B00000,B00000,B00000,B11110,B10011,B11111,B11110,B10000};
byte babydino4[8]={B11000,B01100,B00111,B00011,B00011,B00000,B00000,B00000};
byte babydino5[8]={B00111,B01111,B11111,B11111,B11111,B10010,B10010,B11011};
byte babydino6[8]={B11110,B10000,B00000,B11100,B10100,B00000,B00000,B00000};
byte telor1[8]={B00000,B00000,B00011,B00010,B00100,B00110,B01111,B01010};
byte telor2[8]={B00000,B00000,B11000,B01000,B10100,B00100,B01110,B00110};
byte telor3[8]={B10000,B10110,B10110,B10000,B10000,B10011,B01111,B00111};
byte telor4[8]={B00001,B00001,B00101,B01111,B00101,B00001,B00010,B11100}; 


byte altdino1[8]={B00000,B00000,B00000,B00000,B10000,B11000,B10100,B10001};
byte altdino2[8]={B00000,B00001,B00001,B00001,B00010,B00100,B01000,B10001};
byte altdino3[8]={B11110,B10111,B11111,B11000,B11110,B10000,B10000,B00000};
byte altdino4[8]={B10000,B01000,B00110,B00001,B00001,B00000,B00000,B00000};
byte altdino5[8]={B00001,B00001,B00001,B00001,B11111,B10010,B10010,B11011};
byte altdino6[8]={B11111,B10011,B00000,B00000,B00000,B00000,B00000,B00000};
byte altbabydino1[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte altbabydino2[8]={B00000,B00000,B00000,B00011,B00111,B00111,B00111,B00100};
byte altbabydino3[8]={B00000,B00000,B00000,B11110,B10011,B11111,B11110,B10000};
byte altbabydino4[8]={B11000,B01100,B00111,B00010,B00011,B00000,B00000,B00000};
byte altbabydino5[8]={B00101,B01001,B10001,B00001,B11111,B10010,B10010,B11011};
byte altbabydino6[8]={B11110,B10000,B00000,B11100,B10100,B00000,B00000,B00000};
byte alttelor1[8]={B00000,B00000,B00011,B00011,B00111,B00111,B01111,B01111};
byte alttelor2[8]={B00000,B00000,B11000,B11000,B11100,B11100,B11110,B11110};
byte alttelor3[8]={B11111,B11111,B11111,B11111,B11111,B11111,B01111,B00111};
byte alttelor4[8]={B11111,B11111,B11111,B11111,B11111,B11111,B11110,B11100};

byte testUR[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte testUM[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte testUL[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte testBR[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte testBM[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};
byte testBL[8]={B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00000};

byte kasurUR[8]={B11111,B10101,B01011,B11111,B11111,B11111,B11101,B11001};
byte kasurUM[8]={B00011,B00010,B00011,B00011,B01111,B11111,B11111,B11111};
byte kasurUL[8]={B00000,B00000,B00000,B00000,B00000,B00001,B00111,B11111};
byte kasurBR[8]={B00001,B00001,B00000,B00000,B00000,B00000,B00000,B00000};
byte kasurBM[8]={B11111,B11111,B00011,B00011,B00011,B00011,B00000,B00000};
byte kasurBL[8]={B11111,B11111,B11000,B11000,B11000,B11000,B00000,B00000};

int growthStatus =0;
boolean stringComplete = false;
uint8_t isqrt(uint8_t num) {
    uint8_t res = 0;
    uint8_t bit = 1 << 6; // The second-to-top bit is set: 1 << 30 for 32 bits
 
    // "bit" starts at the highest power of four <= the argument.
    while (bit > num)
        bit >>= 2;
        
    while (bit != 0) {
        if (num >= res + bit) {
            num -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

void readData(){
    int iterasi=3;
    growthStatus = EEPROM.read(2);
    switch(growthStatus){
      case 0:
      iterasi = 3;
      break;
      case 1:
      iterasi = 35;
      break;
      case 2:
      iterasi = 83;
      break;
    }
    for(int i =0; i<8;i++){
      testUL[i] = EEPROM.read(iterasi+i);
    }
    iterasi+=8;
    for(int i =0; i<8;i++){
      testUM[i] = EEPROM.read(iterasi+i);
    }
    iterasi+=8;
    if(growthStatus>0){
      for(int i =0; i<8;i++){
        testUR[i] = EEPROM.read(iterasi+i);
      }
      iterasi+=8;
    }
    else{
      for(int i =0; i<8;i++){
        testUR[i] = B00000;
      }
    }
    for(int i =0; i<8;i++){
      testBL[i] = EEPROM.read(iterasi+i);
    }
    iterasi+=8;
    for(int i =0; i<8;i++){
      testBM[i] = EEPROM.read(iterasi+i);
    }
    iterasi+=8;
    if(growthStatus>0){
      for(int i =0; i<8;i++){
        testBR[i] = EEPROM.read(iterasi+i);
      }
    }
    else{
      for(int i =0; i<8;i++){
        testBR[i] = B00000;
      }
    }
   loadfrombyte();
}

void reset(){
    EEPROM.write(0,0);
    EEPROM.write(2,0);
    readData();
    experience = 0;
    level = 0;
}

void changeChar(){
    int tipe = EEPROM.read(1);
    
    if(tipe==0){
    // make some custom characters:
      
      EEPROM.write(1,1);
      for(int i =0; i<8;i++){
        EEPROM.write(3+i, telor1[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(11+i, telor2[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(19+i, telor3[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(27+i, telor4[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(35+i, babydino1[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(43+i, babydino2[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(51+i, babydino3[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(59+i, babydino4[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(67+i, babydino5[i]);
      }
      
      for(int i =0; i<8;i++){
        EEPROM.write(75+i, babydino6[i]);
      }
      
      for(int i =0; i<8;i++){
        EEPROM.write(83+i, dino1[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(91+i, dino2[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(99+i, dino3[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(107+i, dino4[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(115+i, dino5[i]);
      }
      
      for(int i =0; i<8;i++){
        EEPROM.write(123+i, dino6[i]);
      }
    }
    else{
      EEPROM.write(1,0);
      for(int i =0; i<8;i++){
        EEPROM.write(3+i, alttelor1[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(11+i, alttelor2[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(19+i, alttelor3[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(27+i, alttelor4[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(35+i, altbabydino1[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(43+i, altbabydino2[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(51+i, altbabydino3[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(59+i, altbabydino4[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(67+i, altbabydino5[i]);
      }
      
      for(int i =0; i<8;i++){
        EEPROM.write(75+i, altbabydino6[i]);
      }
      
      for(int i =0; i<8;i++){
        EEPROM.write(83+i, altdino1[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(91+i, altdino2[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(99+i, altdino3[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(107+i, altdino4[i]);
      }
      for(int i =0; i<8;i++){
        EEPROM.write(115+i, altdino5[i]);
      }
      
      for(int i =0; i<8;i++){
        EEPROM.write(123+i, altdino6[i]);
      }
    }
    readData();
}

void loadfrombyte(){
   lcd.createChar(0, testUL);
   lcd.createChar(1, testUM);
   lcd.createChar(2, testBL);
   lcd.createChar(3, testBM);
   lcd.createChar(4, testUR);
   lcd.createChar(5, testBR);
}

void loadkasur(){
   lcd.createChar(0, kasurUL);
   lcd.createChar(1, kasurUM);
   lcd.createChar(2, kasurBL);
   lcd.createChar(3, kasurBM);
   lcd.createChar(4, kasurUR);
   lcd.createChar(5, kasurBR);
}

void printEnergy(){
  if(energy==0){
    lcd.setCursor(12,0);
    lcd.print("z");
    lcd.setCursor(0,0);
    lcd.print("Need sleep");
  }
  else {lcd.setCursor(0,0);lcd.print("Energy: "+String(energy));}
}
void printGambar(){
  lcd.setCursor(13,0);
  lcd.write(byte(0));
  lcd.setCursor(14,0);
  lcd.write(byte(1));
  
  lcd.setCursor(13,1);
  lcd.write(byte(2));
  
  lcd.setCursor(14,1);
  lcd.write(byte(3));
  
  lcd.setCursor(15,0);
  lcd.write(byte(4));
  
  lcd.setCursor(15,1);
  lcd.write(byte(5));
}
void setup() {
    lcd.begin(16,2);
    energy = 50;
    // game parameter init
    sleepy = 0;

    // join I2C bus (I2Cdev library doesn't do this automatically)
    lcd.backlight();
    Wire.begin();
    lcd.home();
    readData();
    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);
    experience = EEPROM.read(0);
    level = isqrt(experience);
    if(experience==255) experience=0;
    // initialize device
    accelgyro.initialize();
  
    accelgyro.setAccelerometerPowerOnDelay(3);
    accelgyro.setIntZeroMotionEnabled(TurnOnZI);
    //DEBUG_PRINTLN("Setting DHPF bandwidth to 5Hz...");
    accelgyro.setDHPFMode(1);
        
    //Serial.println("Setting motion detection threshold to 2...");
    accelgyro.setMotionDetectionThreshold(2);
    //Serial.println("Setting zero-motion detection threshold to 156...");
    accelgyro.setZeroMotionDetectionThreshold(2);
    //Serial.println("Setting motion detection duration to 80...");
    accelgyro.setMotionDetectionDuration(40);
  
    //Serial.println("Setting zero-motion detection duration to 0...");
    accelgyro.setZeroMotionDetectionDuration(1);  
    
    attachInterrupt(digitalPinToInterrupt(3), interruptHandler, CHANGE);

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
    
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    inputString.reserve(200);
    printGambar();
    printEnergy();

    writeLevel();
}
void loop() {
    
      writeLevel();  
    if(isEat){
      lcd.clear();
      lcd.print("yummy");
      delay(3000);
      if(energy<80)energy+=20;
      lcd.clear();
      printEnergy();
      printGambar();
      isEat = false;
    }
    // read raw accel/gyro measurements from device
    //Serial.println("Getting raw accwl/gyro measurements");
    //
  
    int sensorValue = analogRead(A0);
  
    if (sensorValue < sensorTreshold) sleepy>=sleepyTeshold || sleepy++;
    else sleepy = 0;
    
    if (sleepy >= sleepyTeshold) {
      if(!isSleep){
        loadkasur();
        lcd.clear(); 
        lcd.setCursor(0,0);
        lcd.print("Zzzzz..");
        printGambar();
      }
      isSleep = true;
      energycycle++;
      if(energy<100&&energycycle>=energycycletreshold/5){
        energycycle=0;
        energy++;
      }
    } else{
      if(isSleep){
        lcd.clear(); 
        printEnergy();
        printGambar();
        loadfrombyte();
      }
      isSleep = false; 
      energycycle++;
      if(energy>0&&energycycle>=energycycletreshold/2){
        energycycle=0;
        energy--; 
        printEnergy();
      }
      
    }
    

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    XnegMD = accelgyro.getXNegMotionDetected();
    XposMD = accelgyro.getXPosMotionDetected();
    YnegMD = accelgyro.getYNegMotionDetected();
    YposMD = accelgyro.getYPosMotionDetected();
    ZnegMD = accelgyro.getZNegMotionDetected();
    ZposMD = accelgyro.getZPosMotionDetected();
  
    zero_detect = accelgyro.getIntMotionStatus();
    threshold = accelgyro.getZeroMotionDetectionThreshold();
    
    //Serial.println("Getting Die Temperature");  
    temp=(accelgyro.getTemperature()/340.)+36.53;
    value = EEPROM.read(a);
     a = a + 1;

    if (a == 512)
      a = 0;
    if(zero_detect==0){
      if (lamp>0){
        lamp -= 8;
        analogWrite(LED_PIN, lamp);
        delay(30);
      }
    }
    else {
      counter++;    
      blinkState = true;
      if(energy>0&&counter>=counterTreshold&&!isSleep){
        counter = 0;
        experience++;
        if(experience>=225){
          experience=225;
          lcd.setCursor(0,0);
          lcd.clear();
          lcd.print("You already rea-");
          lcd.setCursor(0,1);
          lcd.print("ched max level");
          
          delay(2000);
          
          lcd.clear();
          printEnergy();
          printGambar();
          
        }
        else {
          energy--;
          EEPROM.put(0, experience);
          if(level<isqrt(experience)){
            lcd.clear();
            lcd.setCursor(0,0);
            
            lcd.print("level up");
            delay(1000);
     
            if(level<5)growthStatus=0;
            else if(level<10&&level>=5)growthStatus=1;
            else if(level>=10&&level<15)growthStatus=2;
            level = isqrt(experience);
            if(growthStatus>2)growthStatus=2;
            else EEPROM.put(2,growthStatus); 
            readData(); 
          }
          lcd.clear(); 
          printGambar();
          printEnergy(); 
        }
        
      }
      
      if (lamp<255) {
        lamp += 8;
        analogWrite(LED_PIN, lamp);
        delay(30);
      }

    }
    //digitalWrite(LED_PIN, blinkState);
    delay(60);
    if (stringComplete) {
      if(inputString=="reset\n"){
          reset();
          energy = 50;
          Serial.println("reset done");
      }
      else if(inputString=="sleep\n"){
          energy = 0;
          Serial.println("sleep");
      }
      
      else if(inputString=="maxlevel\n"){
          experience = 225;
          growthStatus=2;
            EEPROM.put(2,growthStatus); 
          EEPROM.put(0, experience);
          level =15;
          Serial.println("maxlevel cheat activated");
          readData();
      }
      else if(inputString=="read\n"){
          Serial.println("read data");
          
          Serial.print("level : ");
          Serial.println(level);
          
          Serial.print("exp : ");
          Serial.println(experience);
          
          Serial.print("energy : ");
          Serial.println(energy);
      }
      else if(inputString=="write\n"){
          changeChar();
          Serial.println("character changed");
      }  
      lcd.clear();
      printEnergy();
      printGambar();
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void  interruptHandler(){
  isEat = true;
}

void writeLevel() {
  int p = level/10;
  int s = level%10;

  int a[8];
  for (int i=0;i<4; i++){
    a[i]=p&1;
    p>>=1;
  }
  for (int i=4;i<8; i++){
    a[i]=s&1;
    s>>=1;
  }

  int x = 0;
  for (int i=7; i>=0; i--) {
    digitalWrite(9, LOW);
    digitalWrite(8, a[i]);
    digitalWrite(9, HIGH);
  }
}

/* ( THE END ) */
