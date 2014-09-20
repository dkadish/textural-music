// Code for Photosynthetics - Riot on the Roof edition
// David Kadish

#include <SerialCommand.h>
//#include <Cycles.h>

const int pins[] = {3, 5, 6, 9};
const int N = 4;

SerialCommand sCmd;     // The demo SerialCommand object
unsigned long time;

boolean systemOn = true;

void setup() {
  // Setup output pins
  for( int i = 0; i < N; i++ ){
    pinMode(pins[i], OUTPUT);
    analogWrite(pins[i],0);
  }
  
  // Setup Serial
  Serial.begin(9600);
  delay(10);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",    system_on);          // Turns LED on
  sCmd.addCommand("OFF",   system_off);         // Turns LED off
  sCmd.addCommand("PWM",     processPulseWidthManagement);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  //Serial.println("Ready");
}

void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands
  delay(10);
}

void system_on() {
  ///Serial.println("System on");
  /*for( int i = 0; i < N; i++ ){
    analogWrite(pins[i], 128);
  }*/
  systemOn = true;
}

void system_off() {
  //Serial.println("System off");
  for( int i = 0; i < N; i++ ){
    analogWrite(pins[i], 0);
  }
  systemOn = false;
}

void processPulseWidthManagement() {
  int aNumber;
  char *arg;

  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    analogWrite(13, aNumber);
  }
  else {
    //Serial.println("No arguments");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  //Serial.println("What?");
}
