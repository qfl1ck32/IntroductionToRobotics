## Introduction to Robotics (2021 - 2022)


##### This repository contains all the homeworks for the _Robotics Course_.

##### Each homework will be detailed below, in the following manner:

---

## Homework #n

##### 1) Description

###### &nbsp;&nbsp; ...


##### 2) Requirements

###### &nbsp;&nbsp; ...

##### 3) Implementation details (with code & images)

###### &nbsp;&nbsp; ...

---

:-)

---

## Homework #1

##### 1) Description

###### &nbsp;&nbsp; We have to modify the intensity of an RGB LED, for each color, using three potentiometers.


##### 2) Requirements

       1. 1x Arduino Uno
       2. 1x RGB Led
       3. 1x 220Ω resistor
       4. 12x Jumper Wires
       5. 3x Rotary knob

##### 3) Implementation details (with code & images)

The actual code can be found [here](https://github.com/qfl1ck32/IntroductionToRobotics/blob/master/Homework%201/main.ino).

Little explanation for each step:

1. Declaring the constants that we'll be using throughout the algorithm:

```C++
const int numberOfColors = 3;

const int maxAnalogWriteValue = 255;

const int maxAnalogReadValue = 1023;
```


2. Declaring the pins that we're going to use. (note: we will usually store data in an `array[3]`, representing `{ R, G, B }` related values, i.e. `potPins[1] = the pin for the potentiometer that will control the intensity for green`).

```C++
const int potPins[numberOfColors] = { A0, A1, A2 };

const int ledPins[numberOfColors] = { 3, 5, 6 };
```


3. Writing the logic for setting `mode` for the pins we'll be using, inside the `setup()` function:

```C++
void setup() {
  for (unsigned int i = 0; i < numberOfColors; ++i) {
    pinMode(potPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }

   Serial.begin(9600);
}
```


4. Creating a function that reads the values from the potentiometers, and returns them as a `float *`:

```C++
float *getPotValues() {
  float *potValues = (float *) malloc(numberOfColors * sizeof(float));
 
  for (unsigned int i = 0; i < numberOfColors; ++i) {
    potValues[i] = analogRead(potPins[i]);
  }

  return potValues;
}
```


5. (Optional) Creating a function for debugging purposes, that, given the values of the potentiometers, it prints them:

```C++
void printPotValues(float *potValues) {
  for (unsigned int i = 0; i < numberOfColors - 1; ++i) {
    Serial.print(potValues[i]);
    Serial.print(", ");
  }

  Serial.println(potValues[numberOfColors - 1]);
}
```


6. Writing the main loop, where we read the values from the potentiometer, print them (for testing), write them to the pins and clear up the allocated memory:

```C++
void loop() {
  float *potValues = getPotValues();

  printPotValues(potValues);

  writePotValuesToLedPins(potValues);

  free(potValues);
}
```


Here are some pictures, showcasing the wire management & functionality:

1. Wire management:
![IMG_20211022_174210](https://user-images.githubusercontent.com/56713436/138485824-ba1d6bc4-8309-49d7-a604-4cc9e25382c0.jpg)

2. Red light:
![IMG_20211022_174340](https://user-images.githubusercontent.com/56713436/138485847-286e650a-20ba-4786-9786-e13efd820bfe.jpg)

3. Green light:
![IMG_20211022_174400](https://user-images.githubusercontent.com/56713436/138485888-ad9ea3b8-33a0-4b1e-a855-bc2eb0b5bd0c.jpg)

4. Blue light:
![IMG_20211022_174412](https://user-images.githubusercontent.com/56713436/138485904-81158d04-7125-448d-8520-9270aa148fc3.jpg)

5. Mix, yay!
![IMG_20211022_174450](https://user-images.githubusercontent.com/56713436/138485931-1a6bca4e-48b7-418a-aca2-c5501a3f5ed7.jpg)


Demo: https://www.youtube.com/watch?v=GwVAiBVyfeE 😸

--

## Homework #2

##### 1) Description

###### We need to simulate a simple, classic semaphore for pedestrians.


##### 2) Requirements

       1. 1x Arduino Uno
       2. 2x Green LEDs
       3. 2x Red LEDs
       4. 1x Yellow LED
       5. 2x 220Ω resistor
       6. 3x 100Ω resistor
       7. 12x Jumper Wires
       8. 1x Push Button
       9. 1x Passive Buzzer

##### 3) Implementation details (with code & images)

The actual code can be found [here](https://github.com/qfl1ck32/IntroductionToRobotics/blob/master/Homework%202/main.ino).

Little explanation for each step:

1. Create constants for the pins we'll be using:

```C++

const int buttonPin = 2;
const int buzzerPin = 3;

const int pedestriansGreenLedPin = A0;
const int pedestriansRedLedPin = A1;

const int carsGreenLedPin = A2;
const int carsYellowLedPin = A3;
const int carsRedLedPin = A4;
```

2. Define constants for states:

```C++

const int state1 = 0;
const int state2 = 1;
const int state3 = 2;
const int state4 = 4;
```

Explanation for each state:
- `State 1`:
  * Green light for cars
  * Red light for pedestrians
- `State 2`:
  * Yellow light for cars
  * Red light for pedestrians
- `State 3`:
  * Red light for cars
  * Green light for pedestrians
  * Buzzer running at a constant interval
- `State 4`:
  * Red light for cars
  * Green light for pedestrians, this time, blinking
  * Buzzer running at a constant interval, but faster than in step 3
       
3. Define buzzer-related constants:

```C++
const int buzzerFrequency = 1000;
const int buzzDuration = 200;

unsigned long buzzerStartTime;
unsigned long buzzerEndTime;
unsigned long isBuzzerRunning;

unsigned long buzzerDurationBetweenBeepsInState3 = 1000;
unsigned long buzzerDurationBetweenBeepsInState4 = 250;
```

4. Define constants for debouncing the button push:

```C++
const int buttonDebounceDelay = 50;
unsigned long lastDebounceTime = 0;

```

5. Define variables for timing the states:

```C++
unsigned long startTimeState1;
unsigned long durationState1 = 10000;

unsigned long startTimeState2;
unsigned long durationState2 = 3000;

unsigned long startTimeState3;
unsigned long durationState3 = 10000;

unsigned long startTimeState4;
unsigned long durationState4 = 5000;
```

6. Define variables for handling the blinking of the green LED (in state 4):

```C++
unsigned long pedestriansGreenLightLastBlinkTime;
unsigned long pedestriansGreenLightDurationBetweenBlinks = 250;
```

7. Create the last needed global variables - the ones related to the state of the app:

```C++
int state;
boolean isAppRunning;
```

8. Create an `initialisation` method, which (re)sets the state to `state 1`:

```C++
void setInitialState() {
  startTimeState1 = 0;
  startTimeState2 = 0;
  startTimeState2 = 0;
  startTimeState3 = 0;
  startTimeState4 = 0;

  buzzerStartTime = 0;
  buzzerEndTime = 0;

  isAppRunning = false;
  isBuzzerRunning = false;

  pedestriansGreenLightLastBlinkTime = 0;

  noTone(buzzerPin);

  setState1();
}
```

9. Create a method that `debounces` a function, using a given `delay`:


```C++
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
```

10. Create methods that switch the state to another one, by turning on / off the corresponding leds:

```C++

void setState1() {
  state = state1;
  
  digitalWrite(pedestriansGreenLedPin, LOW);
  digitalWrite(pedestriansRedLedPin, HIGH);

  digitalWrite(carsGreenLedPin, HIGH);
  digitalWrite(carsYellowLedPin, LOW);
  digitalWrite(carsRedLedPin, LOW);
}

void setState2() {
  state = state2;
  
  digitalWrite(carsGreenLedPin, LOW);
  digitalWrite(carsYellowLedPin, HIGH);
}

void setState3() {
  state = state3;

  digitalWrite(carsYellowLedPin, LOW);
  digitalWrite(carsRedLedPin, HIGH);

  digitalWrite(pedestriansRedLedPin, LOW);
  digitalWrite(pedestriansGreenLedPin, HIGH);
}

void setState4() {
  state = state4;
}
```

Note: `setState4()` is the simplest, because we are considering the order of the calls to be `setState1()`, `setState2()`, ..., `setState4()`, and we know that the last two states use the same LEDs; the difference will be in the buzzer & blinking of the green led (for pedestrians).

11. Define a `buzz(frequency, duration)` method, which generates a beep sound, using the Buzzer:

```C++
void buzz(unsigned long freq, unsigned long ms) {
  if (!isBuzzerRunning && buzzerStartTime == 0) {
    tone(buzzerPin, freq); 

    buzzerStartTime = millis();
    isBuzzerRunning = true;
    return;
  }

  if (millis() - buzzerStartTime > ms) {
    noTone(buzzerPin);
    isBuzzerRunning = false;
    buzzerEndTime = millis();
    buzzerStartTime = 0;
  }
}
```

12. Create an utilitary function, that switches the state of the green led, for pedestrians:

```C++
void switchPedestriansGreenLightState() {
  boolean currentPedestriansGreenLightValue = digitalRead(pedestriansGreenLedPin);

  digitalWrite(pedestriansGreenLedPin, !currentPedestriansGreenLightValue);
}
```

13. Define another utilitary function that, given the `start time` and `duration` of an action, checks if the given amount of time has passed:

```C++
boolean hasTimePassed(unsigned long startTime, unsigned long duration) {
  return millis() - startTime >= duration;
}
```

14. Create `handlers` for states (methods that will be called in `loop()`, based on the current state):

```C++
void handleState1() {
  if (!startTimeState1) {
    startTimeState1 = millis();
    return;
  }

  if (hasTimePassed(startTimeState1, durationState1)) {
    setState2();
  }
}

void handleState2() {
  if (!startTimeState2) {
    startTimeState2 = millis();
    return;
  }

  if (hasTimePassed(startTimeState2, durationState2)) {
    setState3();
  }
}

void handleState3() {
  if (!startTimeState3) {
    startTimeState3 = millis();
    return;
  }

   if (hasTimePassed(buzzerEndTime, buzzerDurationBetweenBeepsInState3)) {
      buzz(buzzerFrequency, buzzDuration);
    }

  if (hasTimePassed(startTimeState3, durationState3)) {
    setState4();
  }
}

void handleState4() {
  if (!startTimeState4) {
    startTimeState4 = millis();
    return;
  }

  if (hasTimePassed(buzzerEndTime, buzzerDurationBetweenBeepsInState4)) {
      buzz(buzzerFrequency, buzzDuration);
  }

  if (!pedestriansGreenLightLastBlinkTime) {
    pedestriansGreenLightLastBlinkTime = millis();
  }
  
  if (hasTimePassed(pedestriansGreenLightLastBlinkTime, pedestriansGreenLightDurationBetweenBlinks)) {
    switchPedestriansGreenLightState();

    pedestriansGreenLightLastBlinkTime = millis();
  }

  if (hasTimePassed(startTimeState4, durationState4)) {
    setInitialState();
  }
}
```

We can see some kind of pattern here. For states 1 and 2, the logic was simple - if the `startTime` for that state was still `0` from the initial state, we update it to `millis()` and stop.
If the time associated to the duration of the state has passed, we switch to the next state.

For state 3, the logic is similar. But, before checking if it's time to switch, we also check if it's time to buzz again.

In state 4, we have the logic for state 3, but we also add logic for handling the `lastBlinkingTime` for the `pedestriansGreenLight`, and, if enough time passed between the last blink and the current time in ms (i.e. `millis()`), we switch the state of the led, and update the `lastBlinkingTime`.

15. Define a function that we'll debounce, using `debounceButtonPush(function, delay)`, in order to start the application:

```C++
void startApp() {
  isAppRunning = true;
}
```

16. For `setup()`, we define the `pinModes` for the pins we're using and calling `Serial.begin()`. Also, we're setting the initial state to be active:

```C++
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(buzzerPin, OUTPUT);
  
  pinMode(pedestriansGreenLedPin, OUTPUT);
  pinMode(pedestriansRedLedPin, OUTPUT);

  pinMode(carsGreenLedPin, OUTPUT);
  pinMode(carsYellowLedPin, OUTPUT);
  pinMode(carsRedLedPin, OUTPUT);
  
  Serial.begin(9600);

  setInitialState();
}
```

17. Define `loop()`, the core of the application:

```C++
void loop() {
  if (!isAppRunning) {
    debounceButtonPush(startApp, buttonDebounceDelay);
    return;
  }

  switch(state) {
    case state1:
      handleState1();
      break;

     case state2:
      handleState2();
      break;

     case state3:
      handleState3();
      break;

     case state4:
      handleState4();
      break;
  }
}
```

Here we have two cases, depending on the state of the app:
1. Running:
  * We debounce the `startApp()` method
2. Not running:
  * Depending on the current `state`, we call the handler for that state.

Here are a few pictures of the application running, in each state (except for state 4, where images can't tell much):


1. State 1: ![state1](https://user-images.githubusercontent.com/56713436/139949797-e9b4361d-bdbb-4069-82d8-d2ecc8b52d92.jpeg)
2. State 2: ![state2](https://user-images.githubusercontent.com/56713436/139949830-ec0f48d5-5cf6-475c-aa79-001f72a9076a.jpeg) 
3. State 3: ![state3](https://user-images.githubusercontent.com/56713436/139949845-ea19b5b5-78d5-4aa1-a69d-db4b1fb96ff4.jpeg)

Here you can find a demo: https://www.youtube.com/watch?v=pSUoL5yYvAs

Enjoy! 🙂

--

## Homework #3

##### 1) Description

###### We need to create an EMF detector.


##### 2) Requirements

       1. 1x Arduino Uno
       2. 1x Green LED
       3. 1x Red Led
       4. 3x 220Ω resistor
       5. 1x 100Ω resistor
       6. 1x 1.000.000Ω resistor
       7. 1x 7 Segment Display
       8. 14x Jumper Wires
       9. 1x Push Button
       10. 1x Passive Buzzer
       11. 2x Paperclips (optional, for the antenna)

##### 3) Implementation details (with code & images)

The actual code can be found [here](https://github.com/qfl1ck32/IntroductionToRobotics/blob/master/Homework%203/main.ino).

Little explanation for each step:

1. Create constants for the pins we'll be using:

```C++
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
```

2. Define constant for app state:

```C++
bool isAppRunning = true;
```

3. Define buzzer-related constants:

```C++
unsigned long buzzerStartTime;
unsigned long buzzerEndTime;
unsigned long isBuzzerRunning;

const int buzzerPauseDurationBetweenBeepsBaseValue = 800;
const int buzzerPauseDurationBetweenBeepsReducingFactor = 2;

const int buzzerFrequency = 1000;
```

4. Define constants for debouncing the button push:

```C++
const int buttonDebounceDelay = 50;
unsigned long lastDebounceTime = 0;

```

5. Define constants & variables related to the antenna:

```C++
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
```

The `antennaValuesThresholds` will be used with a `upper_bound` method, in order to get a value from `0-9` which will represent the intensity of the antenna values.

6. Define variables for the seven segment display:

```C++
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
```

7. Create functions for displaying a digit and clearing the display:

```C++
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
```

8. Create a method that `debounces` a function, using a given `delay`:


```C++
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
```

9. Define a utility function that, given the `start time` and `duration` of an action, checks if the given amount of time has passed:

```C++
boolean hasTimePassed(unsigned long startTime, unsigned long duration) {
  return millis() - startTime >= duration;
}
```

10. Create methods for handling the buzzer:

```C++
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
```

11. Create a function to handle switching the app state (will be used with the button debounce):

```C++
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
```

12. Create an "upper_bound" method (will be used to identify the intensity of the antenna values):

```C++
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
```

13. Define a function that reads values from the antenna and stores them.

```C++
void handleAntennaSampleGathering() {
  if (currentAntennaSampleCount >= antennaSamplesCount) {
    currentAntennaSampleCount = 0;
  }

  float antennaValue = analogRead(antennaPin);

  antennaSamplesTotal = max(antennaSamplesTotal - antennaSamples[currentAntennaSampleCount], 0);
  
  antennaSamples[currentAntennaSampleCount] = antennaValue;
  
  antennaSamplesTotal += antennaValue;
  
  ++currentAntennaSampleCount;
}
```

Idea: In the beginning, we will fill `antennaSamples` with... well, antenna samples; and in the main loop,
we'll be calling this function, which will update `antennaSamplesTotal` and `antennaSamples[]`,
always keeping the array full and updating `antennaSamplesTotal`, to match the sum of `antennaSamples`.

14. Create one of the main functions, which gathers data from the antenna and handles the buzzer & the seven segment display.

```C++
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
```


15. For `setup()`, we define the `pinModes` and initialise the `antennaSamples` array: 

```C++
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
```

16. Define `loop()`, which debounces the button push with `switchAppRunningState` (so you can start / stop the app at any time, using the button) and runs `handleAntenna`,
if the application is running:

```C++
void loop() {
  debounceButtonPush(switchAppRunningState, buttonDebounceDelay);

  if (isAppRunning) {
    handleAntenna();
  }
} 
```

Here are a few pictures of the application :D

1. Turned off ![IMG_20211110_224052](https://user-images.githubusercontent.com/56713436/141194367-131a30cb-e2f1-42c3-a3e9-4824dc29501f.jpg)
2. Turned on ![IMG_20211110_224150](https://user-images.githubusercontent.com/56713436/141194407-bf0aaaea-b0ef-42eb-bac7-fabba6a22d06.jpg)
3. Bird-eye view ![IMG_20211110_224119](https://user-images.githubusercontent.com/56713436/141194446-503d0ef1-34e3-450b-ac5c-e4cfb6793318.jpg)

Here you can find a demo: https://youtu.be/TZ1IxyYX37Y

Sorry for the sad quality of the photos & video. Too late at night to have good light. Will probably update some time soon.

Enjoy 😸

--

## Homework #4

##### 1) Description

###### We need to control the digits of a seven segment display using joystick movements (and its integrated button).

In the first state, the decimal point of the current segment we are on (initially, the first one), will blink at a constant interval.
At this point, only movements on the Y axis are allowed.


Pushing the button of the joystick will select the segment, hence, the second state - the movements on the Y axis are disabled, and moving joystick
on the X axis will swap to the next/previous digit. Pushing again will make the app go back to the first state.

Note: Holding the joystick in one direction shouldn't lead to an infinite loop throughout segments / digits. Only the first change will count.

Another note: We will also store the digits that are being displayed in the EEPROM, and restoring them when starting the app.


##### 2) Requirements

       1. 1x Arduino Uno
       2. 1x 74HC595 Shift Register
       3. 1x Joystick
       4. 2x 220Ω Resistor
       5. 21x Jump Wire
       

##### 3) Implementation details (with code & images)

The actual code can be found [here](https://github.com/qfl1ck32/IntroductionToRobotics/blob/master/Homework%204/main.ino).

Little explanation, behind the idea. Will try to keep things simpler :D


1. We defined a few constants & functions for handling the EEPROM.

```
#define EEPROM_IS_DATA_STORED_INDEX 0
#define EEPROM_DISPLAY_DIGITS_START_INDEX 1
```

These are used so we know the index where the data is stored (or where the data starts at, in the case of _the number that we'll display_).

We have 4 basic functions for this:

       1. areDisplayDigitsStoredInEEPROM() - which returns `true` if the EEPROM has a value of `1` at the defined index;
       
       2. storeInitialDisplayDigitsInEEPROM() - which writes the `displayDigits` array to the EEPROM (in consecutive positions);
       
       3. restoreDisplayDigitsFromEEPROM() - which does exactly what it says - it reads the value from EEPROM and stores them into `displayDigits`;
       
       4. updateDisplayDigitInEEPROM(index, digit)` - which will update the value in EEPROM for a given index (the `segment`) with the given `digit`.
       
2. For handling the joystick, we have the following:
       
       1. handleSw() - which is used in `interrupt(...)`, for selecting a segment to change its digit;
       
       2. handleJoystickMovementOnAxisX and handleJoystickMovementOnAxisY - which handle the corresponding edge cases (i.e. when a segment is selected,
       y-axis movements should do nothing, and when a segment isn't selected, it ignores x-axis movements) and update the `currentSegment` and
       the value of that segment, respectively. When updating the value of a segment, it also updates the value in `EEPROM`.
       
       3. handleJoystick() - which is a simple wrapper over the previous two functions;
       
       4. handleDisplayDecimalPoint() - which, if `displayDecimalPointDelayBetweenBlinks`ms have passed, it changes the state of the decimal point and resets
       the value for `lastBlink`.

3. For showing a number, using multiplexing, we've used 3 functions:

       1. showOnlyNthSegment(n) - which turns off all segments but the n-th;
       
       2. writeRegister(digit) - which writes, using `shiftOut`, a digit (using `most significant bit first` for the bit order);
       
       3. showNumber(int *number) - this function takes an array of 4 digits as argument, and displays, on each 7-segment display, rapidly,
       every digit, from left to right.
       
4. In `setup()`, except for `pinMode(...)` calls and turning off all `segmentDigits`, we're also attaching an `interrupt` on the `SW` pin, with `handleSw`,
with mode = `RISING`. Also, we need to restore data saved in EEPROM. So - if there's data stored already, we restore it, otherwise, we store the initial `displayDigits`
(which are by default `{ 0, 0, 0, 0 }`).

5. In the main `loop()`, we have the following logic:

       1. Create an array for the digits that we'll show - call it digits[];
       
       2. Call handleJoystick();
       
       3. If there's no segment selected, call handleDisplayDecimalPoint()
       
       4. Iterate through every digit in the global displayDigits array, get the corresponding bits from digitArray and,
       if there is no segment selected (i.e. we are in state 1), we do a logical or with displayDecimalPointState
       (to add, or not, the decimal point, to the current segment).


1. Bird-eye view: ![IMG_20211115_173226](https://user-images.githubusercontent.com/56713436/141816038-dd9c97fe-3149-45f1-a29b-f2b4b28911fa.jpg)
2. Stylish image 😆 : ![IMG_20211115_173105](https://user-images.githubusercontent.com/56713436/141815119-b7c9100b-60b8-4d18-96b3-4247548c25f7.jpg) 

Here you can find a demo: https://youtu.be/96cb6XyhVAA

Sorry again for the bad lighting. I always get to work @RoboticsHomeWorks at night.

Enjoy 😸
