
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LedControlMS.h"


#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

/*
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 */

LiquidCrystal_I2C lcd(0x20,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display 
LedControl lc=LedControl(12,11,10,1);
int buttonPin = 2; // The button shall be attached to digital pin 2
int buzzerPin = 4; //buzzer attached to digital pin 4
int restartPin = 0; //vibrate to restart

int score = 0; //intial score
int pos = 2; // cursor position on the lcd 
int i; //random number of the shape
unsigned long delaytime=200;


  /* here is the data for the LCD characters */
uint8_t hero[8] = {0xc,0xc,0x0,0xe,0x1c,0xc,0x1a,0x13};
uint8_t beauty[8] = {0xe,0x1b,0xe,0x4,0x1f,0x4,0xa,0xa};
uint8_t lcdHeart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t arrow[8] = {0x00,0x10,0x18,0x1c,0x1e,0x1c,0x18,0x10};
uint8_t lost[8] = {0x1f,0x15,0x1f,0x1f,0x0e,0x0a,0x1b,0x00};

  /* here is the data for the LED shapes */
byte shapes[7][8]={
  {B00000000,B00111110,B01110111,B01101011,B01111111,B01101011,B01111111,B00111110},
  {B00001000,B00011100,B00111110,B01111111,B01111111,B01110111,B00100010,B00000000},
  {B00000000,B00010000,B00111000,B01111100,B11111110,B01111100,B00111000,B00010000},
  {B00011000,B00111100,B01011010,B11100111,B11100111,B01011010,B00111100,B00011000},
  {B00110000,B01001000,B01001000,B00111001,B00000101,B00000011,B00001111,B00000000},
  {B00111000,B01000100,B01000100,B00111000,B00010000,B01111100,B00010000,B00000000},
  {B11000001,B11100001,B01110011,B00111011,B00011111,B00001111,B00111111,B11111111},
  };


void setup()
{
  Serial.begin(9600);
  
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  lcd.createChar(0, hero);
  lcd.createChar(1, beauty);
  lcd.createChar(2, lcdHeart);
  lcd.createChar(3, arrow);
  lcd.createChar(4, lost);
  
  lcd.home();
  
  lcd.print("Catch My Heart...");
  lcd.setCursor(0, 1);
  lcd.printByte(3);
  lcd.printByte(3);
  lcd.printByte(2);
  lcd.printByte(0);
  lcd.setCursor(15, 1);
  lcd.printByte(1);
  
  lc.shutdown(0,false);
  /* Set the brightness to a low values */
  lc.setIntensity(0,2);
  /* and clear the display */
  lc.clearDisplay(0);
  
  pinMode(buttonPin, INPUT);
  pinMode(restartPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  playSound(1, 1);
 playSound(2, 2);
 playSound(3, 1);

}



void loop(){
  if (analogRead( restartPin ) > 70){
      delay(10);
  asm volatile ("  jmp 0");
  }
  
  i = random(7);
  for (int repeat = 0; repeat < delaytime; repeat++){
  for (int row = 0; row<8; row++){
  lc.setRow(0,row,shapes[i][row]);
  };
 while( digitalRead( buttonPin ) == HIGH ){
 delaytime = delaytime * 8 / 9;
 
 judge();
 }
 }
}


void winGame(){
  lcd.clear();

  for (int i=0; i<15; i++){
      lcd.printByte(2);
      delay(100);
  }
    playSound(6,1);
    playSound(7,1);
    playSound(8,1);
    playSound(9,1);
    playSound(10,2);
  lcd.setCursor(0, 1); 
  lcd.print("You won! Start?");
  delay(2000);
  asm volatile ("  jmp 0");
}

void loseGame(){
  playSound(1, 2) ;
  lcd.clear();
  for (int i=0; i<15; i++){
      lcd.printByte(4);
      delay(100);
  }
    playSound(11,2);
    playSound(12,1);
  lcd.setCursor(0, 1); 
  lcd.print("Game over!Start?");
  delay(2000);
  asm volatile ("  jmp 0");
//  if ( digitalRead(buttonPin) == HIGH ){
//    setup(); /// how to return to the beginning???
//  }
}


void judge(){
  if( i == 1 ){ //heart shape
    score++;      
    playSound(4, 1);
    playSound(5, 1);
    
    if(pos < 12){
    pos++;
   // tone(buzzerPin, 1000, 500);
    lcd.setCursor(pos, 1);
    lcd.printByte(2);
    lcd.printByte(0);
    delay(300);
    }else {winGame();}
  
 }else  {loseGame();} 
}


void playSound(int sound, int repeatTimes) {
  for (int i = 1; i <= repeatTimes; i ++) {
   if (sound == 1) {
      tone(buzzerPin,1020 , 250);
       tone(buzzerPin, 1700, 350);
       tone(buzzerPin, 2900, 750);
      delay(300);
    }
    else if (sound == 2) {
      tone(buzzerPin, 261, 250);
      tone(buzzerPin, 1700, 350);
      delay(500);
    }
    else if (sound == 3) {
      tone(buzzerPin, 523, 150);
      tone(buzzerPin, 3000, 850);
      delay(100);
    }
            else if (sound == 4) {
  //    tone(buzzerPin, 523, 150);
      tone(buzzerPin, 1100, 200);
      delay(200);
    }
     else if (sound == 5) {
     // tone(buzzerPin, 2000, 350);
      tone(buzzerPin, 2500, 250);
      delay(250);
     }
     
      else if (sound == 6) {
     tone(buzzerPin, 2500, 350);
      delay(350);
      }
       else if (sound == 7) {
     tone(buzzerPin, 2240, 350);
      delay(350);
       }
        else if (sound == 8) {
     tone(buzzerPin, 2000, 450);
      delay(450);
        }
        else if (sound == 9) {
     tone(buzzerPin, 2230, 450);
      delay(450);
        }
        else if (sound == 10) {
     tone(buzzerPin, 2000, 450);
      delay(450);
        }
        else if (sound == 11) {
     tone(buzzerPin, 1300, 250);
      delay(250);
        }
        else if (sound == 12) {
     tone(buzzerPin, 1100, 450);
      delay(450);
        }
  }
}
