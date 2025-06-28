/*
  Arduino Christmas Songs
  Precision version using tone()
  Improved clarity, timing, and LED patterns
*/

#define melodyPin 9 // Buzzer/Speaker connected to digital pin 9

// ───────────────────────────────
// NOTE FREQUENCY CONSTANTS
// These define the standard frequencies for musical notes across different octaves.
// Source: https://www.arduino.cc/en/Tutorial/ToneMelody
// ───────────────────────────────

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// ───────────────────────────────
// SONG ARRAYS
// Each song has a melody array (note frequencies) and a tempo array (note durations).
// Note durations are represented as fractions of a whole note:
// 1 = whole note, 2 = half note, 4 = quarter note, 8 = eighth note, 16 = sixteenth note.
// A '0' in the melody array represents a rest.
// ───────────────────────────────

// Jingle Bells melody and tempo
int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5, // Jingle bells, jingle bells
  NOTE_E5, NOTE_E5, NOTE_E5, // jingle all the way
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5, // Oh what fun it is to ride
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, // In a one-horse open sleigh, hey!
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, // Jingle bells, jingle bells
  NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_G5 // Jingle all the way
};

int tempo[] = {
  8, 8, 4,    // 8th, 8th, quarter
  8, 8, 4,    // 8th, 8th, quarter
  8, 8, 8, 8, 2, // 8th, 8th, 8th, 8th, half
  8, 8, 8, 8, 8, // 8th, 8th, 8th, 8th, 8th
  8, 8, 16, 16, // 8th, 8th, 16th, 16th (for the staccato "ding dong" feel)
  8, 8, 8, 8, 4   // 8th, 8th, 8th, 8th, quarter
};

// We Wish You A Merry Christmas melody and tempo
int wish_melody[] = {
  NOTE_B3,                               // We
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, // wish you a merry christmas
  NOTE_D4, NOTE_D4, NOTE_D4,              // We wish you a merry
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, // christmas, we wish you a merry
  NOTE_E4, NOTE_E4, NOTE_E4,              // christmas, and a happy
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4, // new year.
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,    // Good tidings we bring
  NOTE_D4, NOTE_G4, NOTE_E4,              // to you and your kin;
  NOTE_F4                                // Good tidings for christmas and a happy new year.
};

int wish_tempo[] = {
  4,    // quarter
  4, 8, 8, 8, 8, // quarter, 8th, 8th, 8th, 8th
  4, 4, 4,    // quarter, quarter, quarter
  4, 8, 8, 8, 8, // quarter, 8th, 8th, 8th, 8th
  4, 4, 4,    // quarter, quarter, quarter
  4, 8, 8, 8, 8, // quarter, 8th, 8th, 8th, 8th
  4, 4, 8, 8,    // quarter, quarter, 8th, 8th
  4, 4, 4,    // quarter, quarter, quarter
  2     // half
};

// Santa Claus Is Comin' to Town melody and tempo
int santa_melody[] = {
  NOTE_G4, // You
  NOTE_E4, // better
  NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, // watch out, you better not cry,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5, // Better not pout, I'm telling you why
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, // Santa Claus is coming to town.
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,         // He's making a list
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,         // And checking it twice;
  NOTE_D4, NOTE_F4, NOTE_B3,                  // Gonna find out who's naughty and nice.
  NOTE_C4                                     // Santa Claus is coming to town.
};

int santa_tempo[] = {
  8,    // 8th
  8,    // 8th
  8, 4, 4, 4, // 8th, quarter, quarter, quarter
  8, 8, 4, 4, 4, // 8th, 8th, quarter, quarter, quarter
  8, 8, 4,    // 8th, 8th, quarter
  4, 4,    // quarter, quarter
  8, 8, 4, 2, // 8th, 8th, quarter, half
  4, 4, 4, 4, // quarter, quarter, quarter, quarter
  4, 2, 4,    // quarter, half, quarter
  1     // whole
};

// ───────────────────────────────
// LED pins
// Using pins 5, 6, 9, 10 which are PWM capable on Arduino Uno,
// and avoiding conflict with button pins 2, 3, 4.
// ───────────────────────────────
const int ledPins[] = {5, 6, 9, 10};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// ───────────────────────────────
// LED Pattern Types (Enums for readability)
// ───────────────────────────────
enum LedPattern {
  LED_PATTERN_NONE = 0,      // No LED activity
  LED_PATTERN_SEQUENTIAL,    // LEDs light up one after another
  LED_PATTERN_RANDOM_FLASH,  // Random LEDs turn on/off
  LED_PATTERN_PULSE          // All LEDs turn on briefly
};

// Global variable to keep track of the currently selected LED pattern
int currentLedPattern = LED_PATTERN_SEQUENTIAL; // Default pattern

// Global variable for sequential LED pattern indexing
int globalLedIndex = 0;

// ───────────────────────────────
// SETUP FUNCTION
// Initializes pins and serial communication.
// ───────────────────────────────
void setup() {
  // Set LED pins as OUTPUT
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs are off initially
  }

  // Set button pins as INPUT_PULLUP (internal pull-up resistors, button to GND)
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);

  // Initialize Serial communication for debugging output
  Serial.begin(9600);
  Serial.println("Arduino Christmas Songs Ready!");

  // Seed the random number generator using an unconnected analog pin
  // (A0 usually picks up random noise). This ensures different random patterns each time.
  randomSeed(analogRead(A0));
}

// ───────────────────────────────
// LOOP FUNCTION
// Continuously checks for button presses to play different songs and select LED patterns.
// ───────────────────────────────
void loop() {
  // If button on pin 2 is pressed (LOW), play Jingle Bells with Sequential LEDs
  if (digitalRead(2) == LOW) {
    currentLedPattern = LED_PATTERN_SEQUENTIAL;
    playSong(melody, tempo, sizeof(melody) / sizeof(int), "Jingle Bells");
  }
  // If button on pin 3 is pressed (LOW), play Santa Claus with Random Flash LEDs
  else if (digitalRead(3) == LOW) {
    currentLedPattern = LED_PATTERN_RANDOM_FLASH;
    playSong(santa_melody, santa_tempo, sizeof(santa_melody) / sizeof(int), "Santa Claus Is Comin' to Town");
  }
  // If button on pin 4 is pressed (LOW), play We Wish You with Pulse LEDs
  else if (digitalRead(4) == LOW) {
    currentLedPattern = LED_PATTERN_PULSE;
    playSong(wish_melody, wish_tempo, sizeof(wish_melody) / sizeof(int), "We Wish You A Merry Christmas");
  } else {
    // Default action: if no button is pressed, play Jingle Bells with Sequential LEDs
    currentLedPattern = LED_PATTERN_SEQUENTIAL;
    playSong(melody, tempo, sizeof(melody) / sizeof(int), "Jingle Bells (Default)");
  }
  delay(100); // Small delay to debounce buttons and prevent rapid song repeats
}

// ───────────────────────────────
// PLAY SONG FUNCTION
// Plays a given musical song array with specified tempo and synchronizes LED patterns.
// ───────────────────────────────
void playSong(int *song, int *tempo, int length, const char *songName) {
  Serial.print("Playing: ");
  Serial.println(songName);

  // Define the base tempo for the song in Beats Per Minute (BPM)
  // Adjusted to 300 BPM for clearer note distinction and gaps.
  const int BPM = 00;
  // Calculate the duration of a quarter note in milliseconds based on the BPM.
  // (60 seconds/minute * 1000 ms/second) / BPM = ms per beat (quarter note)
  const long quarterNoteDurationMs = (60L * 1000L) / BPM;

  // Reset the global LED index for sequential patterns at the start of each song
  globalLedIndex = 0;

  // Iterate through each note in the song
  for (int i = 0; i < length; i++) {
    // Calculate the fractional value of the current note
    // For tempo[i] = 4 (quarter note), noteValue = 4.0 / 4 = 1.0
    // For tempo[i] = 8 (eighth note), noteValue = 4.0 / 8 = 0.5
    float noteValue = 4.0 / tempo[i];

    // Calculate the total duration for the current note (or rest) in milliseconds
    long totalNoteDuration = (long)(quarterNoteDurationMs * noteValue);

    // Allocate 80% of the total duration for the tone sound (increased silence for distinctness)
    long playTime = totalNoteDuration * 0.8;
    // Allocate 20% of the total duration for silence between notes, for clarity
    long silenceTime = totalNoteDuration * 0.2;

    // Play the note using tone() if the note frequency is not 0 (which indicates a rest)
    if (song[i] != 0) {
      tone(melodyPin, song[i], playTime); // Play the note for 'playTime' duration
    } else {
      noTone(melodyPin); // Ensure no sound is played for rests
    }

    // --- LED Pattern Control for the current note ---
    // First, turn off all LEDs to ensure a clean slate before applying the new pattern
    for (int k = 0; k < numLeds; k++) {
      digitalWrite(ledPins[k], LOW);
    }

    // Apply the selected LED pattern based on the 'currentLedPattern' variable
    switch (currentLedPattern) {
      case LED_PATTERN_NONE:
        // No LEDs are turned on for this pattern
        break;

      case LED_PATTERN_SEQUENTIAL:
        // Lights up LEDs one by one in a circular sequence
        digitalWrite(ledPins[globalLedIndex], HIGH);
        // Move to the next LED for the next note, wrapping around if needed
        globalLedIndex = (globalLedIndex + 1) % numLeds;
        break;

      case LED_PATTERN_RANDOM_FLASH:
        // Randomly turn on or off a subset of the LEDs
        for (int k = 0; k < numLeds; k++) {
          digitalWrite(ledPins[k], random(0, 2)); // random(0, 2) generates either 0 (LOW) or 1 (HIGH)
        }
        break;

      case LED_PATTERN_PULSE:
        // A simple "all on" effect for digital pins, simulating a pulse
        // (Could be enhanced with analogWrite for fading if more complex PWM is desired)
        for (int k = 0; k < numLeds; k++) {
          digitalWrite(ledPins[k], HIGH);
        }
        break;

      default:
        // Fallback to sequential pattern if an invalid pattern is somehow set
        digitalWrite(ledPins[globalLedIndex], HIGH);
        globalLedIndex = (globalLedIndex + 1) % numLeds;
        break;
    }

    // Wait for the duration of the tone playing
    delay(playTime);

    // Stop the current tone and turn off all LEDs after the note has finished playing
    noTone(melodyPin);
    for (int k = 0; k < numLeds; k++) {
      digitalWrite(ledPins[k], LOW);
    }

    // Wait for the silence period between notes
    delay(silenceTime);
  }

  // After the song finishes, ensure no tone is playing and all LEDs are off
  noTone(melodyPin);
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}
