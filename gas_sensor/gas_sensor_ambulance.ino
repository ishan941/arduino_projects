#define bp 3  // Buzzer pin
#define led 4 // LED pin

void setup()
{
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    delay(20000); // MQ gas sensor warm-up time
}

void loop()
{
    int sv = analogRead(A0);
    Serial.println(sv);

    if (sv > 400)
    {
        digitalWrite(led, HIGH);

        // Simulate ambulance siren: alternate between 700 Hz and 1000 Hz
        for (int i = 0; i < 5; i++)
        {
            tone(bp, 1000);
            delay(500);
            tone(bp, 700);
            delay(500);
        }

        noTone(bp);
        delay(200); // short pause before next cycle
    }
    else
    {
        digitalWrite(led, LOW);
        noTone(bp);
        delay(1000); // check again in 1 second
    }
}
