#include <EEPROM.h>

char UNIQUE_ID = 3;

void setup() {
  // put your setup code here, to run once:
  EEPROM.write(0, UNIQUE_ID);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
