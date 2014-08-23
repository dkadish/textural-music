/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */

// Pin Definitions
int lightPin = 5;    // select the output pin for the lights
int leafPin = 3;    // select the output pin for the lights

int delayLength = 100;

// Values
float lightValue = 0.0;  // variable to store the value coming from the sensor
float leafValue = 0.0;  // variable to store the value coming from the sensor

// Amplitudes, 0 to 100
int lightAmp = 100;
int leafAmp = 100;

// Means, difference from middle between -100,100
int lightMean = 0;
int leafMean = 0;

// Periods, in minutes
float lightPeriod = 0.25;
float leafPeriod = 1;

/* CALCULATED VARIABLES */
float lightIncrement = null;
float leafIncrement = null;

void setup() {
  Serial.begin(9600);
  delay(10);
  
  pinMode(lightPin, OUTPUT);  
  pinMode(leafPin, OUTPUT);
  
  lightIncrement = lightPeriod / (60.0 * delayLength);  
  leafIncrement = leafPeriod / (60.0 * delayLength);
}

void loop() {
  // read the value from the sensor:
  lightValue = lightValue + lightIncrement; //analogRead(sensorPin);
  analogWrite(lightPin, map((int)(sin(lightValue) * 100.0), -100, 100, 0, 255));
  
  leafValue = leafValue + leafIncrement; //analogRead(sensorPin);
  analogWrite(leafPin, map((int)(sin(leafValue) * 255.0), -100, 100, 0, 255));
  
  delay(delayLength);
}
