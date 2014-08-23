// Code for Photosynthetics - Riot on the Roof edition
// David Kadish

#include <SerialCommand.h>
#include <Cycles.h>

#define LIGHT1 5   // Light1
#define LEAF1 3 // The White Leaf
#define LIGHT2 6   // Light2
#define LEAF2 9 // The ?? Leaf
#define LIGHT3 10   // Light3
#define LEAF3 11 // The ?? Leaf
#define N 3

const int leaves[] = {LEAF1, LEAF2, LEAF3};
const int lights[] = {LIGHT1, LIGHT2, LIGHT3};

SineWave leafCycles[] = {SineWave(1000,255), SineWave(1000,255), SineWave(1000,255)};
Heartbeat lightCycles[] = {Heartbeat(1000,255), Heartbeat(1000,255), Heartbeat(1000,255)};

SerialCommand sCmd;     // The demo SerialCommand object
unsigned long time;

boolean systemOn = true;

void setup() {
  // Setup output pins
  for( int i = 0; i < N; i++ ){
    pinMode(leaves[i], OUTPUT);
    analogWrite(leaves[i],0);
    
    pinMode(lights[i], OUTPUT);
    analogWrite(lights[i],0);
  }
  
  // Setup Serial
  Serial.begin(9600);
  delay(10);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",    system_on);          // Turns LED on
  sCmd.addCommand("OFF",   system_off);         // Turns LED off
  sCmd.addCommand("P",     processCommand);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  Serial.println("Ready");
}

void loop() {
  if( systemOn ){
    time = millis();
    for( int i = 0; i < N; i++ ){
      analogWrite(lights[i], lightCycles[i].value(time));
      analogWrite(leaves[i], leafCycles[i].value(time));
    }
  }
  
  sCmd.readSerial();     // We don't do much, just process serial commands
}

void system_on() {
  Serial.println("System on");
  for( int i = 0; i < N; i++ ){
    analogWrite(lights[i], 255);
    analogWrite(leaves[i], 255);
  }
  systemOn = true;
}

void system_off() {
  Serial.println("System off");
  for( int i = 0; i < N; i++ ){
    analogWrite(lights[i], 0);
    analogWrite(leaves[i], 0);
  }
  systemOn = false;
}

void processCommand() {
  int aNumber;
  char *arg;

  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);;
    analogWrite(LIGHT1, aNumber);
  }
  else {
    Serial.println("No arguments");
  }

  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atol(arg);
    Serial.print("Second argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No second argument");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("What?");
}
