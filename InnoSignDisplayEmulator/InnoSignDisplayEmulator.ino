/*
  Pager Server

  The ESP8266WiFi library's WiFiServer and WiFiServerSecure
  work differently than WiFiServer and EthernetSever
  in Arduino networking libraries.
  This example demonstrates the ArduinoWiFiServer,
  which enhances the WiFiServer.
  ArduinoWiFiServer has available() behaving as documented
  and supports send-to-all-clients functionality, which
  is not implemented in ESP8266WiFi library's WiFiServer
  and WiFiServerSecure.

  The example is a simple server that echoes any incoming
  messages to all connected clients. Connect two or more
  telnet sessions to see how server.available() and
  server.print() work.

  created in September 2020 for ESP8266WiFi library
  by Juraj Andrassy https://github.com/jandrassy
*/

#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>

#ifndef STASSID
#define STASSID "egeCL_2G"
#define STAPSK "egeCL150abcd!"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ArduinoWiFiServer server(2323);

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();

  IPAddress ip = WiFi.localIP();
  Serial.println();
  Serial.println("Connected to WiFi network.");
  Serial.print("To access the server, connect with Telnet client to ");
  Serial.print(ip);
  Serial.println(" 2323");
}


byte buffer[128];
int idx = 0;


void loop() 
{
  WiFiClient client = server.available();     // returns first client which has data to read or a 'false' client
  if (client) {  
    
     if (client.available()) {             // if there's bytes to read from the client, add to buffer
        byte c = client.read();
        Serial.write(c);                    // print it out the serial monitor

        buffer[idx++] = c;
        if (c == 3) // ETX
        {
          HandleBuffer( client );
          idx = 0;
          Serial.println();
        } else 
        if (c == 2) // STX
        {
          // reset and start over!        
          idx = 0;
          buffer[idx++] = c;
          //Serial.println();
        } 
        if (idx> 120) {
          idx = 0; // reset!
          Serial.println("!");
        }
     }
  }
}


void HandleBuffer( WiFiClient client )
{
    if (idx > 2) {
        switch (buffer[1]) {
            case 'x': Serial.print( " position"); break;
            case 'I': Serial.print( " image"); break;
            case 't': Serial.print( " text"); break;
            case 'a': Serial.print( " clear"); break;
            case 'c': Serial.print( " color"); break;
            case 'f': Serial.print( " font"); break;
        }
    }

    client.write(2);
    client.write('s');
    client.write('0');
    client.write(3);
    client.flush();
}
