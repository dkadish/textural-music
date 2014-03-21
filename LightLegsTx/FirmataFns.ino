/**
* Holds all things firmata related
*/

#include <Firmata.h>

void stringCallback(char *myString)
{
    Firmata.sendString(myString);
}


void firmataSetup()
{
    Firmata.setFirmwareVersion(0, 1);
    Firmata.attach(STRING_DATA, stringCallback);
    Firmata.begin(57600);
}

void firmataLoop()
{
    while(Firmata.available()) {
        Firmata.processInput();
    }
}

