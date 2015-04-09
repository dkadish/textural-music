// Demo Code for SerialCommand Library
// Steven Cogswell
// May 2011

#define CONN Serial1
#define POST_INTERVAL 60000L

#include <SPI.h>
#include <WiFi.h>
#include "Statistic.h"
#include <SerialCommand.h>

char ssid[] = "RCA-Client4";     //  your network SSID (name)
char pass[] = "sugarskull111";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(54,86,132,254);  // numeric IP for data.sparkfun.com
//IPAddress server(192,168,0,159);
char server[] = "data.sparkfun.com";    // name address for data.sparkFun (using DNS)
int port = 80;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

SerialCommand sCmd(&CONN);     // The demo SerialCommand object

/////////////////
// Phant Stuff //
/////////////////
const String publicKey = "n1zorjW0RpUl0o6KoMg2"; //"Le2QYxyBk7tDVnZB5aZ0";
const String privateKey = "MoejgZAEaBsyADjMDq0z"; //"30L6Emk5vMtrkgje45jq";
const byte NUM_FIELDS = 3;
const String fieldNames[NUM_FIELDS] = {"humidity", "temperature", "humidifier"};
String fieldData[NUM_FIELDS];

long lastPost = 0;

Statistic temperature, light; 

void setup() {

  CONN.begin(9600);
  delay(100);
  
    // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    //Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  /*String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");*/

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    //Serial.print("Attempting to connect to WPA SSID: ");
    //Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  //Serial.print("You're connected to the network");
  //printCurrentNet();
  //printWifiData();

  // Setup callbacks for SerialCommand commands
  sCmd.addCommand("TEMP",    processTemp);          // Turns LED on
  sCmd.addCommand("LIGHT",   processLight);         // Turns LED off
  sCmd.setDefaultHandler(unrecognized);      // Handler for command that isn't matched  (says "What?")
  //CONN.println("Ready");
  
  
  temperature.clear(); //explicitly start clean
  light.clear();
  
}

void loop() {
  sCmd.readSerial();     // We don't do much, just process serial commands
  
  // Post Again?
  if( millis() > lastPost + POST_INTERVAL ){
    // Post
    float t = temperature.average();
    float l = light.average();
    
    if (isnan(t) != 1 && isnan(l) != 1){
      
      String lightStr = floatToString(l);
      String tempStr = floatToString(t);
      
      fieldData[0] = lightStr;
      fieldData[1] = tempStr;
      fieldData[2] = "0.0";
      
      postData();
      
      // Reset last post
      lastPost = millis();
      
      // Reset data
      temperature.clear();
      light.clear();
    
    }
  }
}


void processTemp() {
  float temp;
  char *arg;

  arg = sCmd.next();
  if (arg != NULL) {
    temp = atof(arg);    // Converts a char string to an integer\
  }
  
  temperature.add(temp);
}

void processLight() {
  int light_reading;
  char *arg;

  arg = sCmd.next();
  if (arg != NULL) {
    light_reading = atoi(arg);    // Converts a char string to an integer\
  }
  
  light.add(light_reading);
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
}

void postData()
{
  // Make a TCP connection to remote host
  if (client.connect(server, port))
  {
    // Post the data! Request should look a little something like:
    // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&name=Jim HTTP/1.1\n
    // Host: data.sparkfun.com\n
    // Connection: close\n
    // \n
    client.print("GET /input/");
    client.print(publicKey);
    client.print("?private_key=");
    client.print(privateKey);
    for (int i=0; i<NUM_FIELDS; i++)
    {
      client.print("&");
      client.print(fieldNames[i]);
      client.print("=");
      client.print(fieldData[i]);
    }
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    
/*    Serial.print("GET /input/");
    Serial.print(publicKey);
    Serial.print("?private_key=");
    Serial.print(privateKey);
    for (int i=0; i<NUM_FIELDS; i++)
    {
      Serial.print("&");
      Serial.print(fieldNames[i]);
      Serial.print("=");
      Serial.print(fieldData[i]);
    }
    Serial.println(" HTTP/1.1");
    Serial.print("Host: ");
    Serial.println(server);
    Serial.println("Connection: close");
    Serial.println();
  }
  else
  {
    Serial.println("Connection failed");*/
  } 
  
  // Check for a response from the server, and route it
  // out the serial port.
  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      //Serial.print(c);
    }
  }
  //Serial.println("Connection Finished...");
  
  client.stop();
}

/*void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}*/

String floatToString(float f){
  char c[15];
  dtostrf(f, 7, 3, c);
  String s = String(c);
  s.trim();
  
  return s;
}
