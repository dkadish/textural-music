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

#define LEAF 0
#define LIGHT 1

const int leaves[] = {LEAF1, LEAF2, LEAF3};
const int lights[] = {LIGHT1, LIGHT2, LIGHT3};

SquareWave leafCycles[] = {SquareWave(30000,255), SquareWave(30000,255), SquareWave(30000,255)};
Heartbeat lightCycles[] = {Heartbeat(5000,255), Heartbeat(5000,255), Heartbeat(5000,255)};

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
  sCmd.addCommand("PWM",     processPulseWidthManagement);  // Converts two arguments to integers and echos them back
  sCmd.addCommand("AMP",     processAmplitudeChange);  // Changes the amplitude of a signal
  sCmd.addCommand("PER",     processPeriodChange);  // Changes the period of a signal
  sCmd.addCommand("AVG",     processAveragingChange);  // Changes the period of a signal
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  //Serial.println("Ready");
}

void loop() {
  if( systemOn ){
    time = millis();
    for( int i = 0; i < N; i++ ){
      analogWrite(lights[i], lightCycles[i].value(time)*200/255 + 50);
      analogWrite(leaves[i], leafCycles[i].value(time));
    }
  }
  
  //Serial.println(leafCycles[0].value(time));
  
  sCmd.readSerial();     // We don't do much, just process serial commands
  delay(10);
}

void system_on() {
  ///Serial.println("System on");
  for( int i = 0; i < N; i++ ){
    analogWrite(lights[i], 255);
    analogWrite(leaves[i], 255);
  }
  systemOn = true;
}

void system_off() {
  //Serial.println("System off");
  for( int i = 0; i < N; i++ ){
    analogWrite(lights[i], 0);
    analogWrite(leaves[i], 0);
  }
  systemOn = false;
}

void processAmplitudeChange(){
  // Commands come in the form of: AMP TYPE ID AMPLITUDE FADE
  int object_type;
  int amplitude;
  int id;
  int fade;
  char *arg;
  
  arg = sCmd.next();
  if( arg != NULL ){
    object_type = atoi(arg);
  } else {
    object_type = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    id = atoi(arg);
  } else {
    id = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    amplitude = atoi(arg);
  } else {
    amplitude = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    fade = atoi(arg);
  } else {
    fade = -1;
  }
  
  // Execute command
  if( object_type != -1 && amplitude != -1 && id != -1 ){
    if( object_type == LEAF ){
      if( fade == -1 ){
        leafCycles[id].setAmplitude(amplitude);
      } else {
        leafCycles[id].setAmplitude(time, amplitude, fade);
      }
    } else if ( object_type == LIGHT ){
      if( fade == -1 ){
        lightCycles[id].setAmplitude(amplitude);
      } else {
        lightCycles[id].setAmplitude(time, amplitude, fade);
      }
    }
  } else {
    //Serial.println("Invalid Command");
  }
}

void processPeriodChange(){
  // Commands come in the form of: PER TYPE ID PERIOD
  int object_type;
  int period;
  int id;
  char *arg;
  
  arg = sCmd.next();
  if( arg != NULL ){
    object_type = atoi(arg);
  } else {
    object_type = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    id = atoi(arg);
  } else {
    id = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    period = atoi(arg);
  } else {
    period = -1;
  }
  
  // Execute command
  if( object_type != -1 && period != -1 && id != -1 ){
    if( object_type == LEAF ){
        leafCycles[id].setPeriod(time, period);
        /*Serial.print("Leaf ");
        Serial.print(id);
        Serial.print(" period set to ");
        Serial.println( period );*/
    } else if ( object_type == LIGHT ){
        lightCycles[id].setPeriod(time, period);
        /*Serial.print("Light ");
        Serial.print(id);
        Serial.print(" period set to ");
        Serial.println( period );*/
    }
  } else {
    //Serial.println("Invalid Command");
  }
}

void processAveragingChange(){
  // Commands come in the form of: AVG TYPE ID AVERAGE
  int object_type;
  int average;
  int id;
  char *arg;
  
  arg = sCmd.next();
  if( arg != NULL ){
    object_type = atoi(arg);
  } else {
    object_type = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    id = atoi(arg);
  } else {
    id = -1;
  }
  
  arg = sCmd.next();
  if( arg != NULL ){
    average = atoi(arg);
  } else {
    average = -1;
  }
  
  // Execute command
  if( object_type != -1 && average != -1 && id != -1 ){
    if ( object_type == LIGHT ){
        lightCycles[id].setAveraging(average);
    }
  } else {
    //Serial.println("Invalid Command");
  }
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
