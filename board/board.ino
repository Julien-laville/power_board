/*
 * 
 * radio and board receiver for power board
 * radio talk first
 * 
 */


#include <Servo.h> 
#include <SPI.h>
#include "RF24.h"

#define BOARD_TYPE 0
#define RADIO_TYPE 1
#define ROLE BOARD_TYPE // 0 board | 1 tx
#define DEBUG 1

#define RF_TIMEOUT 1000 * 30
#define LOOP_TIME = 1000

#define RTADIO_IN 7
#define RADIO_OUT 8
#define SPEED_MASK 0x0000000f
#define BATTERY_MASK 0x0000000f
#define BATTERY_LED_0 4 
#define BATTERY_LED_1 5
#define BATTERY_LED_2 6
#define BATTERY_LED_3 7
#define ESC_IN 9

const unsigned int HAND_SHAKE = 101;


#if ROLE == BOARD_TYPE
  
  Servo esc;

#else

#endif



/* board & radio data as global */
int trainData = 0;
bool isTransmitting = BOARD_TYPE == ROLE;

RF24 radio(RTADIO_IN,RADIO_OUT);

void setup() {
  radio.begin();
  #ifdef DEBUG 
    Serial.begin(115200);
    Serial.println("Serial ready");
    Serial.println(" ready");
  #endif
  
  #if ROLE == BOARD_TYPE 
    esc.attach(ESC_IN); 
  #else 
  
  #endif
  
  //board case 
  #if BOARD_TYPE == ROLE
    radio.startListening();    
  #endif
  debugln("radio ready");
}

void loop() {
  bool isTimeOut = false;
  unsigned long startAwait = micros();
  // initial handshake
  if(isTransmitting) {
    while(!radio.write(&HAND_SHAKE, sizeof(HAND_SHAKE))) {
      if(micros() - startAwait > RF_TIMEOUT) {
        debugln("Timeout sending");
      }
    }
  } else {
    radio.startListening(); 
    while (!radio.available()){
      if (micros() - startAwait > RF_TIMEOUT ){            
        isTimeOut = true;
        debugln("Timeout receiving");
        break;
      }      
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
      speed = trainData; //&& SPEED_MASK << 28;
      esc.write(speed);
    } else {
      int battery = 0;
      battery = trainData;// && BATTERY_MASK << 28;
      showPower(battery);
    }

  }
  
  delay(LOOP_TIME);
  
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

void debugln(String msg) {
  #ifdef DEBUG
    Serial.println(msg);
  #endif
}

