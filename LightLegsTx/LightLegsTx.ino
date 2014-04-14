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
#define ID_MIN 1
#define ID_MAX 8

const int led_pin = 11;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;

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

    //firmataSetup();
    
    Serial.begin(9600);
}

char buff[] = {0,0,0,0,0,0,0};

void loop()
{ 
  if (Serial.available() > 0){
       Serial.readBytesUntil('\n', buff, 7);
       if(buff[0] >= ID_MIN && buff[0] <= ID_MAX){
         // Set the permanent level
         level[buff[0]-ID_MIN] = buff[1];
         
         // Send the message
         char msg[2] = {buff[0], buff[1]};
         digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
         vw_send((uint8_t *)msg, 2);
         vw_wait_tx(); // Wait until the whole message is gone
         digitalWrite(led_pin, LOW);
       }
    } else {
      for( int i = ID_MIN; i <= ID_MAX; i++ ){
        // If there is a message, send that one, otherwise, send them all
        char msg[2] = {i, level[i-ID_MIN]};
        
        digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
        vw_send((uint8_t *)msg, 2);
        vw_wait_tx(); // Wait until the whole message is gone
        digitalWrite(led_pin, LOW);
        
        Serial.print(i, DEC);
        Serial.print(", ");
        Serial.print(level[i-ID_MIN], DEC);
        Serial.print('\n');
        delay(100);
      }
    }
    delay(100);
  //firmataLoop();
}
