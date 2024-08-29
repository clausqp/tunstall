/* ESPA 4.4.4 MASTER test program
 * uses USB to simulate serial comms to a slave.
 * 
 * Pull digital pin #2 to ground to halt this master from pollin.
 */

 
const int LedPin = 13;

void setup() 
{
    Serial.begin(9600);
    pinMode( LedPin, OUTPUT );
    digitalWrite( LedPin, HIGH );
    delay( 500 );
    digitalWrite( LedPin, LOW );
    pinMode( 2, INPUT_PULLUP );
}


int state = 0; // 0=wait for EOT

char buf[40];
int prevByte = 0;

void loop() 
{
    delay( 26500 ); // 20+ sekunder
    digitalWrite( LedPin, !digitalRead( LedPin )); // Toggle
                    
    Serial.print( "2\005" ); // poll slave

    // avait slave response
    long waitUntil = millis() + 10000;
    while (!Serial.available() && waitUntil > millis()) {
        delay(100);
    }
    digitalWrite( LedPin, !digitalRead( LedPin )); // Toggle
    
    while (Serial.available()) {
        // digitalWrite( LedPin, !digitalRead( LedPin )); // Toggle
        int byteIn;
        byteIn = Serial.read();
        switch (byteIn) {
            case 1: // SOH
                break;
            case 2: // STX
                break;
            case 3: // ETX
                break;
            case 4: // EOT
                break;
            case 5: // ENQ
                if (prevByte == '2') {
                    digitalWrite( LedPin, !digitalRead( LedPin )); // Toggle
                    Serial.print( "\004" ); // we're done, send EOT
                }
                break;
            default:
                if (' ' <= byteIn && byteIn <= '~') {
                    // buf[idx++] = byteIn;
                }
                break;

        }
        prevByte = byteIn;
    }
    Serial.print( "\004" ); // end of poll, send EOT

    while (digitalRead( 2 ) == 0) {
       delay( 1000 );
    }

}
