#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

//LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27(Cooperate with 3 short circuit caps) for a 16 chars and 2 line display

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int backlightState = LOW;
long previousMillis = 0;
long interval = 1000;
  
// make some custom characters:
byte dino1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B11000,
  B11100,
  B11111
};
byte dino2[8] = {
  B00000,
  B00001,
  B00001,
  B00001,
  B00011,
  B00111,
  B01111,
  B11111
};

byte dino3[8] = {
  B11110,
  B10111,
  B11111,
  B11000,
  B11110,
  B10000,
  B10000,
  B00000
};
byte dino4[8] = {
  B11111,
  B01111,
  B00111,
  B00001,
  B00001,
  B00000,
  B00000,
  B00000
};
byte dino5[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B10010,
  B10010,
  B11011
};
byte dino6[8] = {
  B11111,
  B10011,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// make some custom characters:
byte babydino1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};
byte babydino2[8] = {
  
  B00000,
  B00000,
  B00000,
  B00011,
  B00111,
  B00111,
  B00111,
  B00111,
};

byte babydino3[8] = {
  
  B00000,
  B00000,
  B00000,
  B11110,
  B10011,
  B11111,
  B11110,
  B10000
};
byte babydino4[8] = {
  
  B11000,
  B01100,
  B00111,
  B00011,
  B00011,
  B00000,
  B00000,
  B00000
};
byte babydino5[8] = {
  
  B00111,
  B01111,
  B11111,
  B11111,
  B11111,
  B10010,
  B10010,
  B11011
};
byte babydino6[8] = {
  B11110,
  B10000,
  B00000,
  B11100,
  B10100,
  B00000,
  B00000,
  B00000
};
byte telor1[8] = {
  B00000,
  B00000,
  B00011,
  B00010,
  B00100,
  B00110,
  B01111,
  B01010  
};
byte telor2[8] = {
  B00000,
  B00000,
  B11000,
  B01000,
  B10100,
  B00100,
  B01110,
  B00110
};

byte telor3[8] = {
  B10000,
  B10110,
  B10110,
  B10000,
  B10000,
  B10011,
  B01111,
  B00111
};

byte telor4[8] = {
  B00001,
  B00001,
  B00101,
  B01111,
  B00101,
  B00001,
  B00010,
  B11100
};

byte pac1Def[8] = {
  B00000,
  B01110,
  B11011,
  B11111,
  B11111,
  B01110,
  B00000,
  B00000
};
byte pac2Def[8] = {
B00000,
B01110,
B10100,
B11000,
B11100,
B01110,
B00000,
B00000
};
byte pillDef[8] = {
B00000,
B00000,
B00000,
B01100,
B01100,
B00000,
B00000,
B00000
};
 
const byte pac1 = 0x0;
const byte pac2 = 0x1;
const byte pill = 0x2;
const byte dino1a = 0x3;
const byte dino2a = 0x4;
const byte dino3a = 0x5;
const byte dino4a = 0x6;
const byte dino5a = 0x7;
const byte dino6a = 0x8;
const int del = 250;
 
int x = 0;
int y = 0;
int px = 0;
int py = 0;
 
void setup() {
EEPROM.write(2,0);
//for(int i =0; i<8;i++){
//  EEPROM.write(3+i, telor1[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(11+i, telor2[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(19+i, telor3[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(27+i, telor4[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(35+i, babydino1[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(43+i, babydino2[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(51+i, babydino3[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(59+i, babydino4[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(67+i, babydino5[i]);
//}
//
//for(int i =0; i<8;i++){
//  EEPROM.write(75+i, babydino6[i]);
//}
//
//for(int i =0; i<8;i++){
//  EEPROM.write(83+i, dino1[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(91+i, dino2[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(99+i, dino3[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(107+i, dino4[i]);
//}
//for(int i =0; i<8;i++){
//  EEPROM.write(115+i, dino5[i]);
//}
//
//for(int i =0; i<8;i++){
//  EEPROM.write(123+i, dino6[i]);
//}
//
//EEPROM.write(2,0);
lcd.begin(16,2);                      // initialize the lcd 
lcd.backlight();
Wire.begin();
lcd.home();

// create a new character

lcd.createChar(3, babydino1);
lcd.createChar(4, babydino2);
lcd.createChar(5, babydino3);

lcd.createChar(6, babydino4);
lcd.createChar(7, babydino5);
lcd.createChar(8, babydino6);
//lcd.createChar(9, telor1);
//lcd.createChar(10, telor2);
//
//lcd.createChar(11, telor3);
//
//lcd.createChar(12, telor4);

// set up the lcd's number of columns and rows:

lcd.begin(16, 2);
// fill the display
//fill();
}
 

void loop() {
//anim();
//x++;
//if(x>15 && y == 0)
//{
//x = 0;
//y = 1;
//}
//else if(x>15 && y == 1)
//{
//x = 0;
//y = 0;
//fill();
//}

lcd.setCursor(13,0);
lcd.write(byte(3));
lcd.setCursor(14,0);
lcd.write(byte(4));
lcd.setCursor(15,0);
lcd.write(byte(5));
lcd.setCursor(13,1);
lcd.write(byte(6));
lcd.setCursor(14,1);
lcd.write(byte(7));
lcd.setCursor(15,1);
lcd.write(byte(8));


}
 
// Initial display fill
void fill()
{
lcd.setCursor(0,0);
lcd.write(byte(0)); //pac1
for(int j=0;j<7;j++)
{
lcd.print(" ");
lcd.write(byte(2)); //pill
}
lcd.setCursor(0,1);
lcd.write(byte(2)); //pill
for(int j=0;j<7;j++)
{
lcd.print(" ");
lcd.write(byte(2)); //pill
}
}
 
// character animation
void anim()
{
lcd.setCursor(px,py);
lcd.print(" ");
lcd.setCursor(x,y);
lcd.write(byte(1));  //pac2
delay(del);
lcd.setCursor(x,y);
lcd.write(byte(0));  //pac1
delay(del);
px = x;
py = y;
}
