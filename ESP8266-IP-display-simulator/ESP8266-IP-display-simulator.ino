/* LED display simulator - Used to simulate the necessary behaviour for DisplayLedServer to believe it is talking to a real Tunstall display
 * 
 * Most packages must be answered by 0x04, 0x01, only exception is command RS, which requests display information.
 * the RS-command must be answered with some text in order to be accepted as a working display.
 * 
 * Based on TCP Socket Server Send Receive Demo sample.
 * Use Arduino IDE to compile and write the program to chip flash memory.
 * This program was tested using Board type "LOLIN (WEMOS) D1 R2 & Mini" which is an ESP8266 type of board.
*/

#include <ESP8266WiFi.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 10001;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "asus_F10";  //Enter your wifi SSID
const char *password = "xxxxxxxxx";  //Enter your wifi Password

char macAddr[50];


int count=0;

void setup() 
{
  Serial.begin(115200);
  pinMode(BUILTIN_LED,OUTPUT);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);

  String x = WiFi.macAddress();
  x.toCharArray( macAddr, 50 ) ;

  digitalWrite( BUILTIN_LED, HIGH );
}


void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
    }

    int cmd = 0;
    byte cmd1, cmd2;
    int state = 0;
    digitalWrite( BUILTIN_LED, LOW );
    while(client.connected()){      
      if (client.available()>0) {
        while(client.available()>0){
          // read data from the connected client
          byte b = client.read(); 
          Serial.write(b);

          switch (b) {
            case 0x00: break;
            
            case 0x01: // start of packet header
            case 0x02: // start of packet data
            case 0x03: // end of packet data
            case 0x04: // end of packet
              state = b; 
              break;

            default:
              switch (state) {
                case 0x02:
                  if (cmd == 0) {
                    cmd1 = b;
                    cmd++;
                  } else if (cmd == 1) {
                    cmd2 = b;
                    cmd++;
                  } // ignore rest!
                  break;
              }
              break;
          }
        }

        if (state == 0x04) { 
            // Package done:
            client.write( 0x04 );
            client.write ( 0x01 );
            if (cmd > 1 && cmd1 == 'R' && cmd2 == 'S') {
              //client.write( "Firmware" );
              // LED Information:[0D][0A]-----------------------------------------[0D][0A]LED Height: 7 Pixels[0D][0A]LED Width: 80 Pixels[0D][0A]LED Number: 01[0D][0A]LED Color: TriColor[0D][0A]LED Mode: Expand Mode[0D][0A]LED Play Mode: Play all text[0D][0A]Text Storage Location: RAM[0D][0A]Remote Control: ON[0D][0A]Firmware Version: V16_W78_ON_OFF_Buzzer[0D][0A][0D][0A]CPU Type: EUTC21[0D][0A]FLASH Write Cycle: 217[0D][0A]Build Time: 18:02:34 Dec 02 2018[0D][0A]Firmware Update Time: 2021-09-22 09:40[0D][0A]Serial number: DC68-1006-DFA8-3137[0D][0A][0D][0A]-----------------------------------------[0D][0A]User Information:[0D][0A]-----------------------------------------[0D][0A][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF][FF]ZA22[7F][14][12][04][0C][14][13][04][0C][03]990F[04]
              client.write( "LED Height: 1 Pixels\r\nLED Width: 1 Pixels\r\nLED Number: 1\r\nLED Color: Blue\r\nLED Mode: HACK\r\nFirmware Version: V01_COS\r\nCPU Type: ESP8266\r\nBuild Time: 13:30:00 Jan 14 2022\r\nSerial number: " );
              client.write( macAddr );
              client.write( "\r\n" );

              Serial.print(" *RS*");
            } else {
              Serial.print(".");
            }
        }
      }
      
    }
    client.stop();
    Serial.println();
    Serial.println("Client disconnected");   
    digitalWrite( BUILTIN_LED, HIGH );
  }
}
