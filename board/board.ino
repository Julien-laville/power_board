#include <Servo.h> 
#include <SPI.h>
#include "RF24.h"

#define BOARD_TYPE 0
#define RADIO_TYPE 1
#define ROLE BOARD_TYPE // 0 board | 1 tx
#define DEBUG 1

#define RF_TIMEOUT 1000 * 1000 * 30

#define RTADIO_IN 7
#define RADIO_OUT 8

#if ROLE == BOARD_TYPE
  #define ESC_IN 9
  #define SPEED_MASK 0x0000000f
  
  Servo esc;

#else
  #define BATTERY_MASK 0x0000000f
  #define BATTERY_LED_0 4
  #define BATTERY_LED_1 5
  #define BATTERY_LED_2 6
  #define BATTERY_LED_3 7
#endif



/* board & radio data as global */
int trainData = 0;
bool isTransmitting = true;

RF24 radio(RTADIO_IN,RADIO_OUT);

void setup() {
  radio.begin();
  #if ROLE == BOARD_TYPE 
    esc.attach(ESC_IN); 
    
  #else 
  
  #endif
}

void loop() {
  // read rf
  radio.startListening();    
  bool isTimeOut = false;
  unsigned long startAwait = micros();
  // check for timeout
  while (!radio.available()){
    if (micros() - startAwait > RF_TIMEOUT ){            
      isTimeOut = true;
      break;
    }      
  }
  // 
  if(!isTimeOut) {
    if(isTransmitting) {
      radio.write(&trainData, sizeof(int));
    } else {  
      radio.read(&trainData, sizeof(int) );  
    }

    
    if(ROLE == BOARD_TYPE) {
      int speed = 0;
      speed = &trainData
      esc.write(speed);
    } else {
      showPower(battery);
    }

  }
  
}


void showPower(int battery) {
  if(battery <= 1) // < 25%
    digitalWrite(BATTERY_LED_0, HIGH);
  if(battery <= 2) // 25% -> 50%
    digitalWrite(BATTERY_LED_1, HIGH);
  if(battery <= 3) // 50% -> 75%
    digitalWrite(BATTERY_LED_2, HIGH);
  if(battery <= 4) // 75% -> 100%
    digitalWrite(BATTERY_LED_3, HIGH);
  
}

