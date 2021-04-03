#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>  
#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>   
#include <IRLibCombo.h>   
#include <Arduino.h>

#define GUN_INPUT 4

#define DELAY_BETWEEN_SHOTS 100 // in ms

boolean isReadyToShoot = true;
unsigned long timestamp = 0;

IRsend mySender;
 
void setup() {
  pinMode(GUN_INPUT, INPUT_PULLUP);
}
 
void loop() {
    //send a code every time a character is received from the serial port
    //Sony DVD power A8BCA
    boolean button_state = digitalRead(GUN_INPUT);
  
    if(!button_state && isReadyToShoot) {
      mySender.send(SONY,0xa8bca, 10);
      isReadyToShoot = false;
      timestamp = millis();

     } 
     
     if ((millis() - timestamp) > DELAY_BETWEEN_SHOTS && button_state) {
       isReadyToShoot = true;
     }
     


} 