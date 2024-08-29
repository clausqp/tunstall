

void setup() 
{
    Serial.begin(2400);
    delay(200);
    Serial.println();
    Serial.println("Start UPS test" );

}

long lastTick = 0;



void loop() 
{
    if (lastTick+1000 < millis())  {
        //Serial.print(".");
        lastTick = millis();
    }

    if (Serial.available()) {
        char ch = Serial.read();
        switch (ch)
        {
            case 'Y': // enter smart mode
                Serial.println("SM");
                break;

            case '9': // send line quality
                //Serial.println("FF"); // power ok
                Serial.println("NA"); // no power 
                break;

            case '\n': case '\r':
                break;

            case 'Q':
                Serial.println("10"); // capacity ok
                break;

            case 'f': // capacity
                Serial.println("84.42");
                break;

            case 'P': // load
                Serial.println("4.2");
                break;

            case 'C': // temperature
                Serial.println("42.7");
                break;

            case 'W': // do self test
                Serial.println("");
                break;

            case 'X': // get self test result
                Serial.println("OK"); // BT, NG, NO
                break;

            default:
                Serial.println("garbage!");
                break;

        }

    }
}


