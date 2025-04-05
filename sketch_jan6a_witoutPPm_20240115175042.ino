//helmet module 

#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>
#include <binary.h>
#include <Wire.h>
//

RF24 radio(9,8);

const byte add_for_send[5]="ADDR01";


byte state[]={0,0,0,1};//state[0] --> acident(not using), state[1] --> drunk case, state[2] --> wear ditection, state[4] --> connection bit;

bool acidentDitect=false;
bool drunkDitect=false;
bool wearDitect=false;



void setup() {
  Serial.begin(9600);
  radio.begin();
   radio.openWritingPipe(add_for_send);
  radio.stopListening();
  radio.setPALevel(RF24_PA_MAX);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  acident_ditect();
  drunk_ditect();
  wear_ditect();
  serial_print(); 
  data_transmit();
  Serial.println("");
}

void acident_ditect() //HAVE TO CALL
 {
  if(acidentDitect){
    state[0]=1;
  }else{state[0]=0;}
}
//
void drunk_ditect()// HAVE TO CALL
 {
  if(digitalRead(5)==LOW){
    drunkDitect=true;
  }else{drunkDitect=false;}
  if(digitalRead(5)==LOW){
    state[1]=1;
  }else{state[1]=0;}
}

void wear_ditect(){
  if((digitalRead(3)==LOW)&&(digitalRead(4)==LOW)){
    wearDitect=true;
  }else{wearDitect=false;}
  if((digitalRead(3)==LOW)&&(digitalRead(4)==LOW)){
    state[2]=1;
  }else{state[2]=0;}
}

void data_transmit(){
  bool condi;
  condi = radio.write(state,sizeof(state));
  delay(50);
  Serial.print(" DATA : ");
  if(condi){
    Serial.println("SEND");
  }else{Serial.println("NOT SEND");}
}

void serial_print(){
    Serial.print("state : ");
  for(int i=0;i<=2;i++){
    Serial.print(state[i]);
    Serial.print(" : ");
  }
  Serial.println(" ");
}