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

        // Police siren effect: frequency sweep up and down
        for (int i = 500; i <= 1000; i += 10)
        {
            tone(bp, i);
            delay(5);
        }

        for (int i = 1000; i >= 500; i -= 10)
        {
            tone(bp, i);
            delay(5);
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
