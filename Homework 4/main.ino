#include <EEPROM.h>

#define EEPROM_IS_DATA_STORED_INDEX 0
#define EEPROM_DISPLAY_DIGITS_START_INDEX 1


const int clockPin = 10;
const int latchPin = 11;
const int dataPin = 12;

const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

bool swState = false;
bool lastSwState = false;

int xValue = 0;
int yValue = 0;

bool joyMovedOnXAxis = false;
bool joyMovedOnYAxis = false;

bool displayDecimalPointState;

int minThreshold = 400;
int maxThreshold = 600;

unsigned int displayDecimalPointLastBlink = 0;
const unsigned int displayDecimalPointDelayBetweenBlinks = 500;

int displayDigits[] = { 0, 0, 0, 0 };

int digitArray[16] = {
    B11111100,
    B01100000,
    B11011010,
    B11110010,
    B01100110,
    B10110110,
    B10111110,
    B11100000,
    B11111110,
    B11110110,
    B11101110,
    B00111110,
    B10011100,
    B01111010,
    B10011110,
    B10001110 
};

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

int segmentDigits[] = {
  segD1, segD2, segD3, segD4
};

const int segmentsCount = 4;

volatile bool isSegmentSelected = false;

int currentSegment = 0;

bool areDisplayDigitsStoredInEEPROM() {
  return EEPROM.read(EEPROM_IS_DATA_STORED_INDEX);
}

void storeInitialDisplayDigitsInEEPROM() {
  for (int i = 0; i < segmentsCount; ++i) {
    EEPROM.write(EEPROM_DISPLAY_DIGITS_START_INDEX + i, displayDigits[i]);
  }

  EEPROM.write(EEPROM_IS_DATA_STORED_INDEX, 1);
}

void restoreDisplayDigitsFromEEPROM() {
  for (int i = 0; i < segmentsCount; ++i) {
    int digit = EEPROM.read(EEPROM_DISPLAY_DIGITS_START_INDEX + i);

    displayDigits[i] = digit;
  }
}

void updateDisplayDigitInEEPROM(int index, int digit) {
  EEPROM.write(EEPROM_DISPLAY_DIGITS_START_INDEX + index, digit);
}


void handleSw() {
  bool swState = digitalRead(pinSW);

  if (swState != lastSwState) {
    if (swState == LOW) {
      displayDecimalPointLastBlink = 0;
      isSegmentSelected = !isSegmentSelected;
    }
  }

  lastSwState = swState;
}

void handleJoystickMovementOnAxisY() {
  if (isSegmentSelected) return;
  
  yValue = analogRead(pinY);

  if (yValue < minThreshold && !joyMovedOnYAxis) {
    joyMovedOnYAxis = true;

    if (currentSegment > 0) {
      --currentSegment;
    }

    else {
      currentSegment = segmentsCount - 1;
    }
  }

  if (yValue > maxThreshold && !joyMovedOnYAxis) {
    joyMovedOnYAxis = true;

    if (currentSegment < segmentsCount - 1) {
      ++currentSegment;
    }

    else {
      currentSegment = 0;
    }
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMovedOnYAxis = false;
  }
}

void handleJoystickMovementOnAxisX() {
  if (!isSegmentSelected) return;
  
  xValue = analogRead(pinX);

  int digit = displayDigits[currentSegment];

  if (xValue < minThreshold && !joyMovedOnXAxis) {
    joyMovedOnXAxis = true;

    if (digit > 0) {
      --digit;
    }

    else {
      digit = 9;
    }

    updateDisplayDigitInEEPROM(currentSegment, digit);
  }

  if (xValue > maxThreshold && !joyMovedOnXAxis) {
    joyMovedOnXAxis = true;

    if (digit < 9) {
      ++digit;
    }

    else {
      digit = 0;
    }

    updateDisplayDigitInEEPROM(currentSegment, digit);
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMovedOnXAxis = false;
  }

  displayDigits[currentSegment] = digit;
}

void handleJoystick() {
  handleJoystickMovementOnAxisX();
  handleJoystickMovementOnAxisY();
}

void handleDisplayDecimalPoint() {
  if (hasTimePassed(displayDecimalPointLastBlink, displayDecimalPointDelayBetweenBlinks)) {
    displayDecimalPointLastBlink = millis();
    displayDecimalPointState = !displayDecimalPointState;
  }
}


bool hasTimePassed(unsigned long startTime, unsigned long duration) {
  return millis() - startTime > duration;
}


void showOnlyNthSegment(int n) {
  for (int i = 0; i < segmentsCount; ++i) {
    digitalWrite(segmentDigits[i], HIGH);
  }

  digitalWrite(segmentDigits[segmentsCount - n - 1], LOW);
}

void writeRegister(int digit) {
  digitalWrite(latchPin, LOW);

  shiftOut(dataPin, clockPin, MSBFIRST, digit);

  digitalWrite(latchPin, HIGH);
}

void showNumber(int *number) {
  static short int delayMs = 5;

  int segmentIndex = 0;

  do {
    int digit = number[segmentIndex];

    showOnlyNthSegment(segmentIndex);

    writeRegister(digit);

    ++segmentIndex;

    delay(delayMs);
  } while (segmentIndex < segmentsCount && number);
}


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(pinSW, INPUT_PULLUP);

  for (int i = 0; i < segmentsCount; i++) {
    pinMode(segmentDigits[i], OUTPUT);
    digitalWrite(segmentDigits[i], HIGH);
  }

  attachInterrupt(digitalPinToInterrupt(pinSW), handleSw, RISING);

  if (areDisplayDigitsStoredInEEPROM()) {
      restoreDisplayDigitsFromEEPROM();
  }

  else {
    storeInitialDisplayDigitsInEEPROM();
  }

  Serial.begin(9600);
}

void loop() {
  int digits[segmentsCount];

  handleJoystick();
  
  if (!isSegmentSelected) {
    handleDisplayDecimalPoint();
  }

  for (int i = 0; i < segmentsCount; ++i) {
    digits[i] = digitArray[displayDigits[i]];

    if (!isSegmentSelected && i == currentSegment) {
      digits[i] |= displayDecimalPointState;
    }
  }

  showNumber(digits);
}