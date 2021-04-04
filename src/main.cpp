#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>  
#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include <IRLib_P03_RC5.h>   
#include <IRLibCombo.h>   
#include <Arduino.h>


#define GUN_INPUT 4
#define LED_1 A2
#define LED_2 A0
#define LED_3 14
#define LED_4 10


#define DELAY_BETWEEN_SHOTS 200 // in ms
#define DELAY_BETWEEN_LIGHTS 50

boolean isReadyToShoot = true;
unsigned long timestamp = 0;

IRsendRC5 mySender;
 
void setup() {
  pinMode(GUN_INPUT, INPUT_PULLUP);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

}
 
void updateLights() {
  
  if (millis() - timestamp < DELAY_BETWEEN_LIGHTS) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else if((millis() - timestamp) < DELAY_BETWEEN_LIGHTS * 2) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  } else if((millis() - timestamp) < DELAY_BETWEEN_LIGHTS * 3) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, LOW);
  } else if((millis() - timestamp) < DELAY_BETWEEN_LIGHTS * 4) {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, HIGH);
  } else {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
  }

}

void loop() {
    boolean button_state = digitalRead(GUN_INPUT);
  
    if(!button_state && isReadyToShoot) {
      mySender.send(0x7f9d); //Send IR command through RC5
      isReadyToShoot = false;
      timestamp = millis();
     } 
     
     if ((millis() - timestamp) > DELAY_BETWEEN_SHOTS && button_state) {
       isReadyToShoot = true;
     }

     updateLights();    
}