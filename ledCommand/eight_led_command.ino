// ----------------------------
// ADVANCED INTERRUPTIBLE LED CONTROLLER
// ----------------------------

const int ledPins[] = {3, 5, 6, 9, 10, 11, 13, 12}; // PWM + digital pins for 8 LEDs
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

enum Pattern {
  NONE,
  KNIGHT,
  CHASE,
  WAVE,
  RANDOM,
  BINARY,
  BLINK,
  POLICE,
  THEATER,
  BREATHING,
  SPARKLE,
  PINGPONG_FADE,
  RUNNING_PULSE
};

// Forward declarations
void startPattern(Pattern pattern);
void turnAllOn();
void turnAllOff();
void fadeAllTo(int targetBrightness, int stepDelay);
bool isPWMPin(int pin);
void analogWriteSafe(int pin, int value);

void runKnightRider(unsigned long interval);
void runChase(unsigned long interval);
void runWave(unsigned long interval);
void runRandomFlicker(unsigned long interval);
void runBinaryCounter(unsigned long interval);
void runBlinkAll(unsigned long interval);
void runPoliceLights(unsigned long interval);
void runTheaterChase(unsigned long interval);
void runBreathingEffect(unsigned long interval);
void runSparkle(unsigned long interval);
void runPingPongFade(unsigned long interval);
void runRunningPulse(unsigned long interval);
void printHelp();

Pattern currentPattern = NONE;
Pattern nextPattern = NONE;

unsigned long previousMillis = 0;
int patternStep = 0;
bool patternForward = true;
unsigned long userInterval = 100;

char incomingChar;

// For PWM fading transition
bool isTransitioning = false;
int currentBrightness = 255; // full brightness

// For ping pong fade
int pingPongBrightness = 0;
int pingPongFadeAmount = 15;
int pingPongIndex = 0;
bool pingPongForward = true;

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    analogWriteSafe(ledPins[i], 0); // start off
  }
  Serial.begin(9600);
  Serial.println(F("Advanced LED Controller Ready!"));
  Serial.println(F("Type 'H' for help."));
  randomSeed(analogRead(A0));
}

void loop() {
  readSerial();

  if (isTransitioning) {
    // Fade all LEDs out before switching pattern
    if (currentBrightness > 0) {
      currentBrightness -= 5;
      if (currentBrightness < 0) currentBrightness = 0;
      for (int i = 0; i < numLeds; i++) {
        analogWriteSafe(ledPins[i], currentBrightness);
      }
      delay(10);
    } else {
      // Transition done - switch pattern
      currentPattern = nextPattern;
      patternStep = 0;
      previousMillis = millis();
      patternForward = true;
      currentBrightness = 255;
      isTransitioning = false;
      // Clear LEDs for fresh start
      turnAllOff();
    }
    return;
  }

  switch (currentPattern) {
    case KNIGHT:
      runKnightRider(userInterval);
      break;
    case CHASE:
      runChase(userInterval);
      break;
    case WAVE:
      runWave(userInterval * 3);
      break;
    case RANDOM:
      runRandomFlicker(userInterval);
      break;
    case BINARY:
      runBinaryCounter(userInterval * 3);
      break;
    case BLINK:
      runBlinkAll(userInterval * 3);
      break;
    case POLICE:
      runPoliceLights(userInterval);
      break;
    case THEATER:
      runTheaterChase(userInterval);
      break;
    case BREATHING:
      runBreathingEffect(userInterval);
      break;
    case SPARKLE:
      runSparkle(userInterval);
      break;
    case PINGPONG_FADE:
      runPingPongFade(userInterval);
      break;
    case RUNNING_PULSE:
      runRunningPulse(userInterval);
      break;
    case NONE:
    default:
      break;
  }
}

// -----------------------------
// SERIAL COMMANDS
// -----------------------------

void readSerial() {
  if (Serial.available()) {
    incomingChar = Serial.read();
    Serial.print(F("Command: "));
    Serial.println(incomingChar);

    Pattern requestedPattern = NONE;

    switch (incomingChar) {
      case 'K': requestedPattern = KNIGHT; break;
      case 'C': requestedPattern = CHASE; break;
      case 'W': requestedPattern = WAVE; break;
      case 'F': requestedPattern = RANDOM; break;
      case 'N': requestedPattern = BINARY; break;
      case 'L': requestedPattern = BLINK; break;
      case 'P': requestedPattern = POLICE; break;
      case 'T': requestedPattern = THEATER; break;
      case 'B': requestedPattern = BREATHING; break;
      case 'S': requestedPattern = SPARKLE; break;
      case 'G': requestedPattern = PINGPONG_FADE; break; // G for "Glow PingPong"
      case 'R': requestedPattern = RUNNING_PULSE; break;
      case 'A': turnAllOn(); requestedPattern = NONE; break;
      case 'a': turnAllOff(); requestedPattern = NONE; break;
      case 'H': printHelp(); requestedPattern = NONE; break;
      default:
        if (incomingChar >= '0' && incomingChar <= '9') {
          userInterval = (incomingChar - '0') * 50 + 50;
          Serial.print(F("Speed interval set to: "));
          Serial.println(userInterval);
          return; // no pattern change
        } else {
          Serial.println(F("Unknown command."));
          turnAllOff();
          requestedPattern = NONE;
        }
        break;
    }

    if (requestedPattern != currentPattern) {
      // Start fade out transition before switching
      nextPattern = requestedPattern;
      isTransitioning = true;
      currentBrightness = 255;
    }
  }
}

void printHelp() {
  Serial.println(F("Commands:"));
  Serial.println(F("K = Knight Rider"));
  Serial.println(F("C = Chase"));
  Serial.println(F("W = Wave"));
  Serial.println(F("F = Random Flicker"));
  Serial.println(F("N = Binary Counter"));
  Serial.println(F("L = Blink All"));
  Serial.println(F("P = Police Lights"));
  Serial.println(F("T = Theater Chase"));
  Serial.println(F("B = Breathing Effect"));
  Serial.println(F("S = Sparkle"));
  Serial.println(F("G = PingPong Fade"));
  Serial.println(F("R = Running Pulse"));
  Serial.println(F("A = All ON"));
  Serial.println(F("a = All OFF"));
  Serial.println(F("0-9 = Adjust speed"));
}

// -----------------------------
// PATTERNS
// -----------------------------

void runKnightRider(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    turnAllOff();

    if (patternStep >= 0 && patternStep < numLeds) {
      analogWriteSafe(ledPins[patternStep], 255);
    }

    if (patternForward) {
      patternStep++;
      if (patternStep >= numLeds) {
        patternStep = numLeds - 2;
        patternForward = false;
      }
    } else {
      patternStep--;
      if (patternStep < 0) {
        patternStep = 1;
        patternForward = true;
      }
    }
  }
}

void runChase(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    turnAllOff();
    digitalWrite(ledPins[patternStep], HIGH);

    patternStep++;
    if (patternStep >= numLeds) {
      patternStep = 0;
    }
  }
}

void runWave(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    for (int i = 0; i < numLeds; i++) {
      if ((patternStep % 2 == 0 && i % 2 == 0) ||
          (patternStep % 2 == 1 && i % 2 == 1)) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
    patternStep++;
  }
}

void runRandomFlicker(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    turnAllOff();
    analogWriteSafe(ledPins[random(numLeds)], 255);
  }
}

void runBinaryCounter(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    for (int b = 0; b < numLeds; b++) {
      digitalWrite(ledPins[b], (patternStep & (1 << b)) ? HIGH : LOW);
    }
    patternStep++;
    if (patternStep >= (1 << numLeds)) {
      patternStep = 0;
    }
  }
}

void runBlinkAll(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    bool isOn = patternStep % 2 == 0;
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], isOn ? HIGH : LOW);
    }
    patternStep++;
  }
}

void runPoliceLights(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    if (patternStep % 2 == 0) {
      for (int i = 0; i < numLeds / 2; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
      for (int i = numLeds / 2; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);
      }
    } else {
      for (int i = 0; i < numLeds / 2; i++) {
        digitalWrite(ledPins[i], LOW);
      }
      for (int i = numLeds / 2; i < numLeds; i++) {
        digitalWrite(ledPins[i], HIGH);
      }
    }
    patternStep++;
  }
}

void runTheaterChase(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    for (int i = 0; i < numLeds; i++) {
      int val = ((i + patternStep) % 3 == 0) ? 255 : 0;
      analogWriteSafe(ledPins[i], val);
    }
    patternStep++;
  }
}

void runBreathingEffect(unsigned long interval) {
  static int brightness = 0;
  static int fadeAmount = 5;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    for (int i = 0; i < numLeds; i++) {
      analogWriteSafe(ledPins[i], brightness);
    }

    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
  }
}

// Sparkle pattern - random LEDs fade in/out smoothly
void runSparkle(unsigned long interval) {
  static int sparkleLed = 0;
  static int sparkleBrightness = 0;
  static int sparkleFadeAmount = 10;
  static bool sparkleFadingOut = false;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    if (!sparkleFadingOut) {
      sparkleLed = random(numLeds);
      sparkleBrightness = 0;
      sparkleFadingOut = true;
    }

    turnAllOff();

    sparkleBrightness += sparkleFadeAmount;
    if (sparkleBrightness >= 255) {
      sparkleBrightness = 255;
      sparkleFadingOut = false;
    }
    analogWriteSafe(ledPins[sparkleLed], sparkleBrightness);
  }
}

// PingPong fade - smooth fade left to right then right to left
void runPingPongFade(unsigned long interval) {
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    for (int i = 0; i < numLeds; i++) {
      analogWriteSafe(ledPins[i], 0);
    }

    analogWriteSafe(ledPins[pingPongIndex], pingPongBrightness);

    pingPongBrightness += pingPongFadeAmount;
    if (pingPongBrightness >= 255) {
      pingPongBrightness = 255;
      pingPongFadeAmount = -pingPongFadeAmount;
    }
    if (pingPongBrightness <= 0) {
      pingPongBrightness = 0;
      pingPongFadeAmount = -pingPongFadeAmount;
      if (pingPongForward) {
        pingPongIndex++;
        if (pingPongIndex >= numLeds) {
          pingPongIndex = numLeds - 2;
          pingPongForward = false;
        }
      } else {
        pingPongIndex--;
        if (pingPongIndex < 0) {
          pingPongIndex = 1;
          pingPongForward = true;
        }
      }
    }
  }
}

// Running pulse - LEDs pulse one by one in sequence
void runRunningPulse(unsigned long interval) {
  static int pulseBrightness = 0;
  static int pulseFadeAmount = 15;

  if (millis() - previousMillis >= interval) {
    previousMillis = millis();

    for (int i = 0; i < numLeds; i++) {
      if (i == patternStep) {
        analogWriteSafe(ledPins[i], pulseBrightness);
      } else {
        analogWriteSafe(ledPins[i], 0);
      }
    }

    pulseBrightness += pulseFadeAmount;
    if (pulseBrightness >= 255 || pulseBrightness <= 0) {
      pulseFadeAmount = -pulseFadeAmount;
      if (pulseBrightness <= 0) {
        patternStep++;
        if (patternStep >= numLeds) {
          patternStep = 0;
        }
      }
    }
  }
}

// -----------------------------
// UTILITY
// -----------------------------

void turnAllOn() {
  for (int i = 0; i < numLeds; i++) {
    analogWriteSafe(ledPins[i], 255);
  }
}

void turnAllOff() {
  for (int i = 0; i < numLeds; i++) {
    analogWriteSafe(ledPins[i], 0);
  }
}

// Check if pin supports PWM on Arduino Uno (3,5,6,9,10,11)
bool isPWMPin(int pin) {
  return (pin == 3 || pin == 5 || pin == 6 || pin == 9 || pin == 10 || pin == 11);
}

// Safe analogWrite - if pin supports PWM, do analogWrite; else digitalWrite HIGH/LOW based on value threshold
void analogWriteSafe(int pin, int value) {
  if (isPWMPin(pin)) {
    analogWrite(pin, value);
  } else {
    // For pins without PWM, treat value > 127 as ON, else OFF
    digitalWrite(pin, value > 127 ? HIGH : LOW);
  }
}
