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

The actual code can be find [here](https://github.com/qfl1ck32/IntroductionToRobotics/blob/master/Homework%201/main.ino).

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
