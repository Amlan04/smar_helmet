#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>

//vehicle module

#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>

byte state[]={0,0,0,1};//state[0] --> acident, state[1] --> drunk case, state[2] --> helmet detact(ideal[1]) {ideal[0.0.1.1]==default[0.0.0.0]}

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool acidentDitect=false;
bool drunkDitect=false;
bool helmetwearDitect=true;
bool nothing;

RF24 radio(9,8);

const byte add_for_recive[5]="ADDR01";

void collectData(){
  if(radio.available()){
    radio.read(&state, sizeof(state));
  }
}

void updateSituation(){
  switch (state[3]){
    case 1:
    //lcd.clear();
    //lcd.println("conention stabilished");
    delay(1000);
    switch(state[2]){
      case 0:
        //display: wear helmate
        helmetwearDitect=false;
        break;
      case 1:
        helmetwearDitect=true;
        switch(state[0]){
        case 0: 
          acidentDitect=false;
          break;
        case 1:
          acidentDitect=true;
          break;
        }
        switch(state[1]){
        case 0: 
          drunkDitect=false;
          //engine onn
          break;
        case 1:
          drunkDitect=true;
          //engine off
          break;
        }
        
    }break;
    case 0:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("connection not ");
    lcd.setCursor(0, 1);
    lcd.print("stablish  :-(");
    delay(500);
    break;
  }
}

void lcdDisplay(){
  if(!(helmetwearDitect)){
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Please");
    lcd.setCursor(2,1);
    lcd.print("Wear Helmate..");
    Serial.println("Please Wear Helmate..");
    delay(500);
  }
  else if(drunkDitect){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("You are Drunk...");
    lcd.setCursor(0,1);
    lcd.print("Eingin kill : on");
    Serial.println("You are Drunk...");
  }else{
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Eingin kill :off");}
  if(acidentDitect){
    // not using
  }
}  
  
void kill_switch(){
  if((!(helmetwearDitect))||(drunkDitect)){
    digitalWrite(5,LOW);
  }else{digitalWrite(5,HIGH);}
}

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0,add_for_recive);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  lcd.begin();
  lcd.backlight();
  pinMode(5,OUTPUT);//relay
  digitalWrite(5,HIGH);

}

void loop() {
  //collectData();
  updateSituation();
  lcdDisplay();
  kill_switch();
}
