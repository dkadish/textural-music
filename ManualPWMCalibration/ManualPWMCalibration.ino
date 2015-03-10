
#define S Serial

// Depends on the kroimon version of SerialCommand
// Available at https://github.com/kroimon/Arduino-SerialCommand
#include <SerialCommand.h>

SerialCommand sCmd(&S);     // The demo SerialCommand object

const int POT_PIN = A0;
const int BUTTON_PIN = 2;
const int LEAF_PIN = 13;

void setup() {
  S.begin(9600);
  delay(10);
  
  // put your setup code here, to run once:
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LEAF_PIN, OUTPUT);
  
  
  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("REC",    record);          // record movemnet
  sCmd.addCommand("PLAY",   play);         // play back movement
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  //S.println("Ready");
}

void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands  
  delay(10);
}

void record(){
  S.println("Starting to Record");
  int buttonPressed, valueRaw, val;
  long time;
  do {
    buttonPressed = digitalRead(BUTTON_PIN);
    valueRaw = analogRead(POT_PIN);
    val = map(valueRaw, 0, 1023, 0, 255);
  
    analogWrite(LEAF_PIN, val);
    
    S.print(millis()-time);
    S.print(", ");
    S.println(val);
  } while ( buttonPressed == LOW );
  
  time = millis();
  S.println("Record");
  
  do {
    buttonPressed = digitalRead(BUTTON_PIN);
    valueRaw = analogRead(POT_PIN);
    val = map(valueRaw, 0, 1023, 0, 255);
  
    analogWrite(LEAF_PIN, val);
    
    S.print(millis()-time);
    S.print(", ");
    S.println(val);
  } while ( buttonPressed == HIGH );
  
  S.println("End");
}

void play(){
    //S.println("Got PWM Command");
  int val;
  char *arg;

  arg = sCmd.next();
  if (arg != NULL) {
    val = atoi(arg);    // Converts a char string to an integer
    analogWrite(LEAF_PIN, val);
  }
  else {
    //S.println("No arguments");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  //S.println("What?");
}
