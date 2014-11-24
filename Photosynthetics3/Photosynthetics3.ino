// Code for Photosynthetics - Riot on the Roof edition
// David Kadish

#define S Serial

// Depends on the kroimon version of SerialCommand
// Available at https://github.com/kroimon/Arduino-SerialCommand
#include <SerialCommand.h>
#include <EEPROM.h>

//#include <Cycles.h>

const int pins[] = {3, 5, 6, 9, 10, 11, 13};
const int N = 7;
int board_id;

SerialCommand sCmd(&S);     // The demo SerialCommand object
//unsigned long time;

boolean systemOn = true;
int ledMode = HIGH;

void setup() {
  // Setup output pins
  for( int i = 0; i < N; i++ ){
    pinMode(pins[i], OUTPUT);
    analogWrite(pins[i],0);
  }
  
  pinMode(13, OUTPUT);
  
  // Setup Serial
  S.begin(9600);
  delay(10);

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("ON",    system_on);          // Turns LED on
  sCmd.addCommand("OFF",   system_off);         // Turns LED off
  sCmd.addCommand("ALL",   all);         // Turns LED off
  sCmd.addCommand("PWM",     processPulseWidthManagement);  // Converts two arguments to integers and echos them back
  sCmd.addCommand("LED",     processLED);  // Converts two arguments to integers and echos them back
  sCmd.addCommand("ID",     processID);  // Converts two arguments to integers and echos them back
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  //S.println("Ready");
  
  board_id = EEPROM.read(board_id);
}

void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands
  
  delay(10);
}

void system_on() {
  ///S.println("System on");
  /*for( int i = 0; i < N; i++ ){
    analogWrite(pins[i], 128);
  }*/
  systemOn = true;
}

void system_off() {
  //S.println("System off");
  for( int i = 0; i < N; i++ ){
    analogWrite(pins[i], 0);
  }
  systemOn = false;
}

void all() {
  //S.println("System off");
  for( int i = 0; i < N; i++ ){
    analogWrite(pins[i], 255);
  }
  systemOn = false;
}

void processPulseWidthManagement() {
  //S.println("Got PWM Command");
  int pin, val;
  char *arg;
  
  arg = sCmd.next();
  if( arg != NULL ){
    pin = atoi(arg);
  } else {
    pin = -1;
  }

  arg = sCmd.next();
  if (arg != NULL) {
    val = atoi(arg);    // Converts a char string to an integer
    analogWrite(pin, val);
  }
  else {
    //S.println("No arguments");
  }
}

void processLED(){
  ledMode = !ledMode;
  digitalWrite(13, ledMode);
}

void processID(){
  S.println(board_id);
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  //S.println("What?");
}
