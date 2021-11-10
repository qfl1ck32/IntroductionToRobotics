const int buzzerPin = 2;
const int buttonPin = 3;

const int antennaPin = A0;

const int redLedPin = A4;
const int greenLedPin = A5;


const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;

bool isAppRunning = true;

const int buttonDebounceDelay = 50;
unsigned long lastDebounceTime = 0;

unsigned long buzzerStartTime;
unsigned long buzzerEndTime;
unsigned long isBuzzerRunning;

const int buzzerPauseDurationBetweenBeepsBaseValue = 800;
const int buzzerPauseDurationBetweenBeepsReducingFactor = 2;

const int buzzerFrequency = 1000;

const int antennaValuesThresholdsCount = 10;
const int antennaValuesThresholds[] = { 50, 150, 250, 350, 450, 550, 650, 750, 850, 950 };

const int antennaMinConstraintValue = 1;
const int antennaMaxConstraintValue = 1023;

const int antennaMinMapValue = 9;
const int antennaMaxMapValue = 14;

const int antennaSamplesCount = 256;

int antennaSamples[antennaSamplesCount];
int antennaSamplesTotal;

int currentAntennaSampleCount;

const int segmentSize = 7;
const int sevenSegmentDisplayPins[segmentSize] = { pinA, pinB, pinC, pinD, pinE, pinF, pinG };

const int sevenSegmentDisplayNumbers[][segmentSize] = {
  { 1, 1, 1, 1, 1, 1, 0 },
  { 0, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 0, 1, 1, 0, 1 },
  { 1, 1, 1, 1, 0, 0, 1 },
  { 0, 1, 1, 0, 0, 1, 1 },
  { 1, 0, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 0, 0, 0, 0 },
  { 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 1, 1 }
};

const int displayDigit(int digit) {
  const int *values = sevenSegmentDisplayNumbers[digit];

  for (short int i = 0; i < segmentSize; ++i) {
    digitalWrite(sevenSegmentDisplayPins[i], values[i]);
  }
}

const int clearDisplay() {
  for (short int i = 0; i < segmentSize; ++i) {
    digitalWrite(sevenSegmentDisplayPins[i], LOW);
  }
}

void debounceButtonPush(void (*function)(), unsigned long delayMs) {
  static int currentButtonState = HIGH;
  static int previousButtonState = HIGH;

  int buttonState = digitalRead(buttonPin);

  if (buttonState != previousButtonState) {
    lastDebounceTime = millis();
  }

  previousButtonState = buttonState;

  if (millis() - lastDebounceTime < delayMs) return;
  
  if (buttonState != currentButtonState) {
    currentButtonState = buttonState;

    if (buttonState == HIGH) {
      function();
    }
  }
}

bool hasTimePassed(unsigned long startTime, unsigned long duration) {
  return millis() - startTime > duration;
}

void buzz(unsigned long freq) {
  if (!isBuzzerRunning && buzzerStartTime == 0) {
    tone(buzzerPin, freq); 

    buzzerStartTime = millis();
    isBuzzerRunning = true;
  }
}

void handleBuzzStop(unsigned long ms) {
  if (buzzerStartTime && hasTimePassed(buzzerStartTime, ms)) {
    noTone(buzzerPin);
    isBuzzerRunning = false;
    buzzerEndTime = millis();
    buzzerStartTime = 0;
  }
}

void switchAppRunningState() {
  if (isAppRunning) {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);

    noTone(buzzerPin);

    clearDisplay();
  }
  
  else {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }
  
  isAppRunning = !isAppRunning;
}

int upper_bound(const int *arr, int arrayLength, int value) {
  int mid, low = 0, high = arrayLength - 1;

  while (low < high) {
    mid = low + (high - low) / 2;

    if (value > arr[mid]) {
      low = mid + 1;
    }

    else {
      high = mid;
    }
  }

  return low;
}

bool handleAntennaSampleGathering() {
  if (currentAntennaSampleCount >= antennaSamplesCount) {
    currentAntennaSampleCount = 0;
  }

  float antennaValue = analogRead(antennaPin);

  antennaSamplesTotal = max(antennaSamplesTotal - antennaSamples[currentAntennaSampleCount], 0);
  
  antennaSamples[currentAntennaSampleCount] = antennaValue;
  
  antennaSamplesTotal += antennaValue;
  
  ++currentAntennaSampleCount;
}

void handleAntenna() {
  handleAntennaSampleGathering();

  int averageFromAntennaSamples = antennaSamplesTotal / antennaSamplesCount;

  int constrainedAntennaValue = constrain(averageFromAntennaSamples, antennaMinMapValue, antennaMaxMapValue);

  int mappedAntennaValue = map(constrainedAntennaValue, antennaMinMapValue, antennaMaxMapValue, antennaMinConstraintValue, antennaMaxConstraintValue);
  
  int antennaThresholdIndex = upper_bound(antennaValuesThresholds, antennaValuesThresholdsCount, mappedAntennaValue);

  int buzzerPauseBetweenBeepsDuration = antennaThresholdIndex == 0 ? 0 : buzzerPauseDurationBetweenBeepsBaseValue / (buzzerPauseDurationBetweenBeepsReducingFactor * antennaThresholdIndex);

  handleBuzzStop(buzzerPauseBetweenBeepsDuration);

  displayDigit(antennaThresholdIndex);

  if (buzzerPauseBetweenBeepsDuration && hasTimePassed(buzzerEndTime, buzzerPauseBetweenBeepsDuration)) {
    buzz(buzzerFrequency); 
  }
}

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(antennaPin, INPUT);

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  for (int i = 0; i < segmentSize; ++i) {
    pinMode(sevenSegmentDisplayPins[i], OUTPUT);
  }

  for (int i = 0; i < antennaSamplesCount; ++i) {
    float antennaValue = analogRead(antennaPin);
  
    antennaSamples[i] = antennaValue;
    antennaSamplesTotal += antennaValue;
  }
  
  Serial.begin(9600);

  switchAppRunningState();
}

void loop() {
  debounceButtonPush(switchAppRunningState, buttonDebounceDelay);

  if (isAppRunning) {
    handleAntenna();
  }
} 