/* Knock Sensor
  
   This sketch reads a piezo element to detect a knocking sound. 
   It reads an analog pin and compares the result to a set threshold. 
   If the result is greater than the threshold, it writes
   "knock" to the serial port, and toggles the LED on pin 13.
  
   The circuit:
	* + connection of the piezo attached to analog in 0
	* - connection of the piezo attached to ground
	* 1-megohm resistor attached from analog in 0 to ground

   http://www.arduino.cc/en/Tutorial/Knock
   
   created 25 Mar 2007
   by David Cuartielles <http://www.0j0.org>
   modified 30 Aug 2011
   by Tom Igoe
   
   This example code is in the public domain.

 */
#define N_NOTES 12

char elements[N_NOTES] = {60,61,62,63,64,65,66,67,68,69,70,71};
 
// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int knockSensor = A0; // the piezo is connected to analog pin 0
const int threshold = 8;  // threshold value to decide when the detected sound is a knock or not


// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int ledState = LOW;         // variable used to store the last LED status, to toggle the light


#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

byte note = random(87); //The MIDI note value to be played
byte resetMIDI = 4; //Tied to VS1053 Reset line
int  instrument = 0;

void setup() {
 pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
 Serial.begin(9600);       // use the serial port
 delay(2000);
 Serial.println("Startup!");

  //Setup soft serial for MIDI control
  mySerial.begin(31250);
  
  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)
  
  talkMIDI(0xB0, 0, 0x00); //Default bank GM1
  talkMIDI(0xC0, 8, 0); //Set instrument number. 0xC0 is a 1 data byte command
}

void loop() {  
  // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(knockSensor);    
  
  // if the sensor reading is greater than the threshold:
  /*if (sensorReading >= threshold) {
    // toggle the status of the ledPin:
    ledState = !ledState;   
    // update the LED pin itself:        
    digitalWrite(ledPin, ledState);
    // send the string "Knock!" back to the computer, followed by newline
    
  Serial.print("Sensor reading: ");
  Serial.println(sensorReading);     
  
  //note = ( sensorReading ) + 30;
  */
  
  note = getNextNote( note );
  //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
  noteOn(0, elements[note], 60);
  delay(50);
  
   Serial.println(elements[note], DEC);

  //Turn off the note with a given off/release velocity
  noteOff(0, elements[note], 60);
  
  delay(100);  // delay to avoid overloading the serial port buffer
  //}
}


//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    mySerial.write(data2);

  digitalWrite(ledPin, LOW);
}

