// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

const int led_pin = 11;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;

char id = 0;
char level[] = {0,0,0,0,0,0,0,0};

void setup()
{
    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);       // Bits per sec
    pinMode(led_pin, OUTPUT);

    firmataSetup();
    
    Serial.begin(9600);
}

byte count = 1;

void loop()
{ 
  // Message format = {'target_id', 'brightness'}
  for( int i = 0; i <=7; i++){
    char msg[2] = {i, level[i]};
  
    //msg[6] = count;
    digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
    vw_send((uint8_t *)msg, 2);
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(led_pin, LOW);
    delay(100);
    count = count + 1;
    
    level[i] = setLevel();
    
    Serial.print("Looped ");
    Serial.print(i, DEC);
    Serial.print(", ");
    Serial.print(level[i], DEC);
    Serial.print('\n');
  }
  //firmataLoop();
}

int incomingByte = 0;

int setLevel(){
  //level = level + 10;
  /*if( level == 0 ){
    return 255;
  } else {
    return 0;
  }*/
  
  if (Serial.available() > 2) {
      // read the incoming byte:
      incomingByte = Serial.read();

      // say what you got:
      Serial.println(incomingByte, DEC);
      
      return (int) incomingByte;
      // read the incoming byte:
      incomingByte = Serial.read();

      // say what you got:
      Serial.println(incomingByte, DEC);
      
      return (int) incomingByte;
  } else {
    return level[0];
  }
}
