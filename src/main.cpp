#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>  
#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include <IRLib_P03_RC5.h>   
#include <IRLibCombo.h>   
#include <Arduino.h>

#define GUN_INPUT 4

#define DELAY_BETWEEN_SHOTS 200 // in ms

boolean isReadyToShoot = true;
unsigned long timestamp = 0;

IRsendRC5 mySender;
 
void setup() {
  pinMode(GUN_INPUT, INPUT_PULLUP);
}
 
void loop() {
    //send a code every time a character is received from the serial port
    //Sony DVD power A8BCA
    boolean button_state = digitalRead(GUN_INPUT);
  
    if(!button_state && isReadyToShoot) {
      mySender.send(0x7f9d);
      isReadyToShoot = false;
      timestamp = millis();

     } 
     
     if ((millis() - timestamp) > DELAY_BETWEEN_SHOTS && button_state) {
       isReadyToShoot = true;
     }
     


} 