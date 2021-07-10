#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>  
#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include <IRLib_P03_RC5.h>   
#include <IRLibCombo.h>   
#include <Arduino.h>


#define GUN_INPUT_NEGATIVE_PIN 4
#define GUN_INPUT_POSITIVE_PIN 5
#define LED_1 A2
#define LED_2 A0
#define LED_3 14
#define LED_4 10

#define BOOT_DELAY 500


#define DELAY_BETWEEN_SHOTS 200 // in ms
#define DELAY_BETWEEN_LIGHTS 50

int pins[] = {LED_1, LED_2, LED_3, LED_4};

boolean isReadyToShoot = true;
unsigned long timestamp = 0;

boolean isBatch2 = false;

IRsendRC5 mySender;
 

 void resetLEDs() {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    digitalWrite(LED_4, LOW);
}

void bootUpSoundBatch1(){
  pinMode(GUN_INPUT_NEGATIVE_PIN, OUTPUT);
  for (int pin : pins) { 
    digitalWrite(pin, HIGH);
    digitalWrite(GUN_INPUT_NEGATIVE_PIN, LOW);
    delay(100);
    digitalWrite(GUN_INPUT_NEGATIVE_PIN, INPUT_PULLUP);
    delay(BOOT_DELAY);
  }
  resetLEDs();
  pinMode(GUN_INPUT_NEGATIVE_PIN, INPUT_PULLUP);
}

void bootUpSoundBatch2(){
    pinMode(GUN_INPUT_POSITIVE_PIN, OUTPUT);
  for (byte i = 0; i < 5; i++){
    for (int pin : pins) { 
      boolean state = digitalRead(pin);
      digitalWrite(pin, !state);
    }
      digitalWrite(GUN_INPUT_POSITIVE_PIN, HIGH);
      delay(50);
      digitalWrite(GUN_INPUT_POSITIVE_PIN, LOW);
      delay(BOOT_DELAY);
  }
  resetLEDs();
  pinMode(GUN_INPUT_POSITIVE_PIN, INPUT);
}

void setup() {
  pinMode(GUN_INPUT_NEGATIVE_PIN, INPUT_PULLUP);
  pinMode(GUN_INPUT_POSITIVE_PIN, INPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

  isBatch2 = !digitalRead(GUN_INPUT_POSITIVE_PIN);

  delay(1000);
  if(isBatch2) bootUpSoundBatch2();
  else bootUpSoundBatch1();
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

boolean button_down(){
  if(isBatch2) {
    boolean button_state = digitalRead(GUN_INPUT_POSITIVE_PIN); // if high = button down
    return button_state;

  } else { // must be first batch
    boolean button_state = digitalRead(GUN_INPUT_NEGATIVE_PIN); // if low = button down
    return !button_state; //flip because button up = down
  }
}

void loop() {
  boolean button_state = button_down();
  
    if(button_state && isReadyToShoot) {
      mySender.send(0x7f9d); //Send IR command through RC5
      isReadyToShoot = false;
      timestamp = millis();
     } 
     
     if ((millis() - timestamp) > DELAY_BETWEEN_SHOTS && button_state) {
       isReadyToShoot = true;
     }

     updateLights();    
}