/* LED display simulator - Used to simulate the necessary behaviour for DisplayLedServer to believe it is talking to a real Tunstall display
 * 
 * Most packages must be answered by 0x04, 0x01, only exception is command RS, which requests display information.
 * the RS-command must be answered with some text in order to be accepted as a working display.
 * 
 * Based on TCP Socket Server Send Receive Demo sample.
 * Use Arduino IDE to compile and write the program to chip flash memory.
 * This program was tested using Board type "LOLIN (WEMOS) D1 R2 & Mini" which is an ESP8266 type of board.
*/


// http://arduino.esp8266.com/stable/package_esp8266com_index.json

//#include <ESP8266WiFi.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int count=0;

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.println();

  digitalWrite( LED_BUILTIN, HIGH );
}


void loop() 
{


    int cmd = 0;
    byte cmd1, cmd2;
    int state = 0;
    
    if (Serial.available()>0) {
        digitalWrite( LED_BUILTIN, LOW );
        
        while(Serial.available()>0){
          // read data from the connected Serial
          byte b = Serial.read(); 
          Serial.print( "'" ); Serial.print( b); Serial.print( "' ");

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
        
        delay(25);
        digitalWrite( LED_BUILTIN, HIGH );
    }
      
}
