// Convention: the arrays are meant to store { R, G, B }-related values (i.e. potPins[0] = the pin of the potentiometer associated with the red light)

const int potPins[numberOfColors] = { A0, A1, A2 };

const int ledPins[numberOfColors] = { 3, 5, 6 };


const int numberOfColors = 3;

const int maxAnalogWriteValue = 255;

const int maxAnalogReadValue = 1023;


void setup() {
  for (unsigned int i = 0; i < numberOfColors; ++i) {
    pinMode(potPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }

   Serial.begin(9600);
}

float *getPotValues() {
  float *potValues = (float *) malloc(numberOfColors * sizeof(float));
 
  for (unsigned int i = 0; i < numberOfColors; ++i) {
    potValues[i] = analogRead(potPins[i]);
  }

  return potValues;
}

void printPotValues(float *potValues) {
  for (unsigned int i = 0; i < numberOfColors - 1; ++i) {
    Serial.print(potValues[i]);
    Serial.print(", ");
  }

  Serial.println(potValues[numberOfColors - 1]);
}

void writePotValuesToLedPins(float *potValues) {
  for (unsigned int i = 0; i < numberOfColors; ++i) {
    float brightness = map(potValues[i], 0, maxAnalogReadValue, 0, maxAnalogWriteValue);
    
    analogWrite(ledPins[i], brightness);
  }
}

void loop() {
  float *potValues = getPotValues();

  printPotValues(potValues);

  writePotValuesToLedPins(potValues);

  free(potValues);
}