
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
                    Serial.print( "\004" );
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

    while (digitalRead( 2 ) == 0) {
       delay( 1000 );
    }

}
