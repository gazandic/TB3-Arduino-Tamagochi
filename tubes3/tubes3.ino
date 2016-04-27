#include "SPI.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define counterTreshold 5
#define energycycletreshold 200
#define LED_PIN 9

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
int8_t level;
bool XnegMD, XposMD, YnegMD, YposMD, ZnegMD, ZposMD;
bool blinkState = false;
int8_t value;
int8_t a =0;
int8_t counter =0, energycycle=0;
int8_t experience = 0;
byte testUR[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte testUM[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte testUL[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte testBR[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte testBM[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte testBL[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte kasurUR[8]={
  B11111,
  B10101,
  B01011,
  B11111,
  B11111,
  B11111,
  B11101,
  B11001
};
byte kasurUM[8]={
  B00011,
  B00010,
  B00011,
  B00011,
  B01111,
  B11111,
  B11111,
  B11111
};
byte kasurUL[8]={
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00001,
  B00111,
  B11111
};
byte kasurBR[8]={
  
  B00001,
  B00001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte kasurBM[8]={
  B11111,
  B11111,
  B00011,
  B00011,
  B00011,
  B00011,
  B00000,
  B00000
};
byte kasurBL[8]={
  B11111,
  B11111,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B00000
};
int growthStatus =0;

int8_t isqrt(int8_t num) {
    int8_t res = 0;
    int8_t bit = 1 << 6; // The second-to-top bit is set: 1 << 30 for 32 bits
 
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
   loadfrombyte();
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
  

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
    
    Serial.print("level : ");
    Serial.println(level);
    
    Serial.print("exp : ");
    Serial.println(experience);
}

void loop() {
    // read raw accel/gyro measurements from device
    //Serial.println("Getting raw accwl/gyro measurements");
    lcd.clear();
  
    int sensorValue = analogRead(A0);
  
    if (sensorValue < sensorTreshold) sleepy>=sleepyTeshold || sleepy++;
    else sleepy = 0;
    
    if (sleepy >= sleepyTeshold) {
      if(!isSleep)loadkasur();
      isSleep = true; 
      lcd.setCursor(0,0);
      lcd.print("Zzzzz..");
      energycycle++;
      if(energy<100&&energycycle>=energycycletreshold/5){
        energycycle=0;
        energy++;
        if(energy>100)energy=100;
      }
    } else {
      if(isSleep)loadfrombyte();
      isSleep = false; 
      lcd.setCursor(0,0);
      if(energy==0)lcd.print("Need sleep");
      else lcd.print("Energy: "+String(energy));
      energycycle++;
      if(energy>0&&energycycle>=energycycletreshold/2){
        Serial.print("energy now: ");
        Serial.println(energy);
        energycycle=0;
        energy--;
        if(energy<0)energy=0;
      }
      
    }
    
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
      if(energy>0&&counter==counterTreshold){
        counter = 0;
        experience++;
        if(experience>=225){
          experience=225;
          lcd.print("You reached max level");
        }
        energy--;
        EEPROM.put(0, experience);
        int8_t templevel = level;
        if(level<isqrt(experience)){
          if(level/5<isqrt(experience)/5)growthStatus++;
          level = isqrt(experience);
          if(growthStatus>2)growthStatus=2;
          else EEPROM.put(2,growthStatus); 
          readData(); 
        }
        Serial.print("level : ");
        Serial.println(level);
        
        Serial.print("exp : ");
        Serial.println(experience);
      }
      
      if (lamp<255) {
        lamp += 8;
        analogWrite(LED_PIN, lamp);
        delay(30);
      }

    }
    //digitalWrite(LED_PIN, blinkState);
    delay(60);
}


/* ( THE END ) */
