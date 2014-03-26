// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

const int led_pin = 5;
const int status_pin = 13;
const int transmit_pin = 12;
const int receive_pin = 11;
const int transmit_en_pin = 3;

int brightness = 0;

void setup()
{
    delay(1000);
    Serial.begin(9600);	// Debugging only
    //delay(5000);
    Serial.println("setup");

    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_set_ptt_pin(transmit_en_pin);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running

    pinMode(led_pin, OUTPUT);
    pinMode(status_pin, OUTPUT);
    digitalWrite(status_pin, HIGH);
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
       	int i;

	// Message with a good checksum received, dump it.
	Serial.print("Got: ");
        Serial.print(buflen, DEC);
        Serial.print(": ");
	
	for (i = 0; i < buflen; i++)
	{
	    Serial.print(buf[i], DEC);
	    Serial.print('.');
	}
	Serial.println();
        
        if( buflen == 2 && buf[0]==0 ){
          brightness = (int) buf[1];
        }
    }
      
    analogWrite(led_pin, brightness);
}
