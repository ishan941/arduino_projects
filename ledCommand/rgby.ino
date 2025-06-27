const int redPin = 3;
const int greenPin = 4;
const int bluePin = 5;
const int yellowPin = 6;

const int ledPins[] = {redPin, greenPin, bluePin, yellowPin};
const int numLeds = 4;

enum Pattern {
  NONE,
  ALL_ON,
  ALL_OFF,
  BLINK_ALL,
  CHASE,
  WAVE,
  KNIGHT,
  RANDOM_FLICKER,
  BREATHING,
  BINARY_COUNTER,
  POLICE_LIGHTS
};

Pattern currentPattern = NONE;

char incomingChar;

unsigned long previousMillis = 0;
unsigned long interval = 200;   // default speed

int patternStep = 0;
bool patternForward = true;
int breathingBrightness = 0;
int breathingFadeAmount = 5;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println("READY!");
  Serial.println("Commands: R, r, G, g, B, b, Y, y, A, a, L, C, W, K, F, D, N, P");
}

void loop() {
  readSerial();

  unsigned long currentMillis = millis();

  switch (currentPattern) {
    case ALL_ON:
      turnAllOn();
      break;
    case ALL_OFF:
      turnAllOff();
      break;
    case BLINK_ALL:
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        bool on = (patternStep % 2 == 0);
        setAll(on);
        patternStep++;
      }
      break;
    case CHASE:
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        turnAllOff();
        digitalWrite(ledPins[patternStep % numLeds], HIGH);
        patternStep++;
      }
      break;
    case WAVE:
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        for (int i = 0; i < numLeds; i++) {
          bool on = ((patternStep % 2 == 0) && (i % 2 == 0)) ||
                    ((patternStep % 2 == 1) && (i % 2 == 1));
          digitalWrite(ledPins[i], on ? HIGH : LOW);
        }
        patternStep++;
      }
      break;
    case KNIGHT:
      if (currentMillis - previousMillis >= interval/2) {
        previousMillis = currentMillis;
        turnAllOff();
        digitalWrite(ledPins[patternStep], HIGH);
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
      break;
    case RANDOM_FLICKER:
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        turnAllOff();
        int randPin = ledPins[random(numLeds)];
        digitalWrite(randPin, HIGH);
      }
      break;
    case BREATHING:
      if (currentMillis - previousMillis >= 30) {
        previousMillis = currentMillis;
        breathingBrightness += breathingFadeAmount;
        if (breathingBrightness <= 0 || breathingBrightness >= 255) {
          breathingFadeAmount = -breathingFadeAmount;
        }
        analogWriteSafe(redPin, breathingBrightness);
        analogWriteSafe(greenPin, breathingBrightness);
        analogWriteSafe(bluePin, breathingBrightness);
        analogWriteSafe(yellowPin, breathingBrightness);
      }
      break;
    case BINARY_COUNTER:
      if (currentMillis - previousMillis >= interval * 2) {
        previousMillis = currentMillis;
        for (int i = 0; i < numLeds; i++) {
          digitalWrite(ledPins[i], (patternStep & (1 << i)) ? HIGH : LOW);
        }
        patternStep++;
        if (patternStep >= (1 << numLeds)) patternStep = 0;
      }
      break;
    case POLICE_LIGHTS:
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (patternStep % 2 == 0) {
          digitalWrite(redPin, HIGH);
          digitalWrite(bluePin, LOW);
        } else {
          digitalWrite(redPin, LOW);
          digitalWrite(bluePin, HIGH);
        }
        digitalWrite(greenPin, LOW);
        digitalWrite(yellowPin, LOW);
        patternStep++;
      }
      break;
    case NONE:
    default:
      // do nothing
      break;
  }
}

void readSerial() {
  if (Serial.available()) {
    incomingChar = Serial.read();
    Serial.print("Command: ");
    Serial.println(incomingChar);

    switch (incomingChar) {
      case 'R': digitalWrite(redPin, HIGH); currentPattern = NONE; break;
      case 'r': digitalWrite(redPin, LOW); currentPattern = NONE; break;
      case 'G': digitalWrite(greenPin, HIGH); currentPattern = NONE; break;
      case 'g': digitalWrite(greenPin, LOW); currentPattern = NONE; break;
      case 'B': digitalWrite(bluePin, HIGH); currentPattern = NONE; break;
      case 'b': digitalWrite(bluePin, LOW); currentPattern = NONE; break;
      case 'Y': digitalWrite(yellowPin, HIGH); currentPattern = NONE; break;
      case 'y': digitalWrite(yellowPin, LOW); currentPattern = NONE; break;
      case 'A': currentPattern = ALL_ON; break;
      case 'a': currentPattern = ALL_OFF; break;
      case 'L': currentPattern = BLINK_ALL; patternStep = 0; break;
      case 'C': currentPattern = CHASE; patternStep = 0; break;
      case 'W': currentPattern = WAVE; patternStep = 0; break;
      case 'K': currentPattern = KNIGHT; patternStep = 0; patternForward = true; break;
      case 'F': currentPattern = RANDOM_FLICKER; patternStep = 0; break;
      case 'D': currentPattern = BREATHING; patternStep = 0; break;
      case 'N': currentPattern = BINARY_COUNTER; patternStep = 0; break;
      case 'P': currentPattern = POLICE_LIGHTS; patternStep = 0; break;
      case '0'...'9':
        interval = map(incomingChar - '0', 0, 9, 50, 1000);
        Serial.print("Interval set to: ");
        Serial.println(interval);
        break;
      default:
        currentPattern = NONE;
        turnAllOff();
        break;
    }
  }
}

void turnAllOn() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
}

void turnAllOff() {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void setAll(bool on) {
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], on ? HIGH : LOW);
  }
}

void analogWriteSafe(int pin, int value) {
  if (isPWMPin(pin)) {
    analogWrite(pin, value);
  } else {
    digitalWrite(pin, value > 127 ? HIGH : LOW);
  }
}

bool isPWMPin(int pin) {
  return (pin == 3 || pin == 5 || pin == 6);
}
