const int buttonPin = 2;
const int buzzerPin = 3;

const int pedestriansGreenLedPin = A0;
const int pedestriansRedLedPin = A1;

const int carsGreenLedPin = A2;
const int carsYellowLedPin = A3;
const int carsRedLedPin = A4;

const int state1 = 0;
const int state2 = 1;
const int state3 = 2;
const int state4 = 4;

const int buzzerFrequency = 1000;
const int buzzDuration = 200;

unsigned long buzzerStartTime;
unsigned long buzzerEndTime;
unsigned long isBuzzerRunning;

unsigned long buzzerDurationBetweenBeepsInState3 = 1000;
unsigned long buzzerDurationBetweenBeepsInState4 = 250;

const int buttonDebounceDelay = 50;
unsigned long lastDebounceTime = 0;

unsigned long startTimeState1;
unsigned long durationState1 = 10000;

unsigned long startTimeState2;
unsigned long durationState2 = 3000;

unsigned long startTimeState3;
unsigned long durationState3 = 10000;

unsigned long startTimeState4;
unsigned long durationState4 = 5000;

unsigned long pedestriansGreenLightLastBlinkTime;
unsigned long pedestriansGreenLightDurationBetweenBlinks = 250;

int state;
boolean isAppRunning;

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

void switchPedestriansGreenLight() {
  boolean currentPedestriansGreenLightValue = digitalRead(pedestriansGreenLedPin);

  digitalWrite(pedestriansGreenLedPin, !currentPedestriansGreenLightValue);
}

boolean hasTimePassed(unsigned long startTime, unsigned long duration) {
  return millis() - startTime >= duration;
}

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
    switchPedestriansGreenLight();

    pedestriansGreenLightLastBlinkTime = millis();
  }

  if (hasTimePassed(startTimeState4, durationState4)) {
    setInitialState();
  }
}

void startApp() {
  isAppRunning = true;
}

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