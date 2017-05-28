#include <Servo.h> 
#include <SPI.h>
#include "RF24.h"
#define RF_TIMEOUT 1000 * 1000 * 30
#define RTADIO_IN 7
#define RADIO_OUT 8
#define ESC_IN 9


Servo esc;
struct boardInfo {
  int battery;
};

struct CommandPacket {
  int speed;
  int signalPower; 
};

int power = 0;
int voltage = 0;

RF24 radio(RTADIO_IN,RTADIO_OUT);

void setup() {
  radio.begin();
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
  if(!isTimeOut) {
    CommandPacket commandPacket;
    radio.read( &commandPacket, sizeof(commandPacket) );
  }
}
