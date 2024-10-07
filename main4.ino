#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Global Variables
#define numOfValsRec 2
#define digitsPerValRec 1
char receivedarray[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec + 1;
int counter = 0;
bool counterStart = false;

// PWM Setup for servos
Adafruit_PWMServoDriver root = Adafruit_PWMServoDriver(0x40);

#define servo1 0 // shoulder
#define servo2 1 // shoulder
#define servo3 2 // elbow
#define servo4 3 // wrist
#define servo5 4 // gripper

// Stepper motor pins for L298N
#define stepperPin1 8
#define stepperPin2 9
#define stepperPin3 10
#define stepperPin4 11

int stepCount = 0;               // Current step count for the stepper motor
int homePosition = 90;           // Home position at 90 degrees
int stepperPosition = homePosition; // Track the stepper's current position
const int stepsPerDegree = 1.8;  // Steps per degree, adjust this based on motor specification

void setup() {
  Serial.begin(9600);
  root.begin();
  root.setPWMFreq(60);

  // Set stepper motor pins as output
  pinMode(stepperPin1, OUTPUT);
  pinMode(stepperPin2, OUTPUT);
  pinMode(stepperPin3, OUTPUT);
  pinMode(stepperPin4, OUTPUT);

  Serial.println("Waiting for commands in the format $xy where x is 1-8 and y is 0 or 1:");
}

// Function to move the stepper motor by a certain number of steps
void stepperMove(int steps) {
  for (int i = 0; i < abs(steps); i++) {
    if (steps > 0) {
      stepCount++;
    } else {
      stepCount--;
    }

    stepMotor(stepCount % 4);
    delay(2); // Delay to control speed of the stepper motor
  }
}

// Function to drive the stepper motor based on step sequence
void stepMotor(int thisStep) {
  switch (thisStep) {
    case 0:
      digitalWrite(stepperPin1, HIGH);
      digitalWrite(stepperPin2, LOW);
      digitalWrite(stepperPin3, LOW);
      digitalWrite(stepperPin4, HIGH);
      break;
    case 1:
      digitalWrite(stepperPin1, LOW);
      digitalWrite(stepperPin2, HIGH);
      digitalWrite(stepperPin3, LOW);
      digitalWrite(stepperPin4, HIGH);
      break;
    case 2:
      digitalWrite(stepperPin1, LOW);
      digitalWrite(stepperPin2, HIGH);
      digitalWrite(stepperPin3, HIGH);
      digitalWrite(stepperPin4, LOW);
      break;
    case 3:
      digitalWrite(stepperPin1, HIGH);
      digitalWrite(stepperPin2, LOW);
      digitalWrite(stepperPin3, HIGH);
      digitalWrite(stepperPin4, LOW);
      break;
  }
}

// Function to move the stepper motor to a specific angle
void moveStepperToAngle(int targetAngle) {
  int stepsRequired = (targetAngle - stepperPosition) * stepsPerDegree;
  stepperMove(stepsRequired);
  stepperPosition = targetAngle; // Update the current position
}

void moveServosToPosition(int position) {
  Serial.print("Moving to position: ");
  Serial.println(position);

  if (position == 1) {
    root.setPWM(servo1, 0, 100);
    root.setPWM(servo2, 0, 100);
    root.setPWM(servo3, 0, 100);
    root.setPWM(servo4, 0, 100);
    moveStepperToAngle(0);
  } else if (position == 2) {
    root.setPWM(servo1, 0, 200);
    root.setPWM(servo2, 0, 200);
    root.setPWM(servo3, 0, 200);
    root.setPWM(servo4, 0, 200);
    moveStepperToAngle(45);
  } else if (position == 3) {
    root.setPWM(servo1, 0, 300);
    root.setPWM(servo2, 0, 300);
    root.setPWM(servo3, 0, 300);
    root.setPWM(servo4, 0, 300);
    moveStepperToAngle(135);
  } else if (position == 4) {
    root.setPWM(servo1, 0, 400);
    root.setPWM(servo2, 0, 400);
    root.setPWM(servo3, 0, 400);
    root.setPWM(servo4, 0, 400);
    moveStepperToAngle(180);
  } else if (position == 5) {
    root.setPWM(servo1, 0, 500);
    root.setPWM(servo2, 0, 500);
    root.setPWM(servo3, 0, 500);
    root.setPWM(servo4, 0, 500);
    moveStepperToAngle(0);
  } else if (position == 6) {
    root.setPWM(servo1, 0, 600);
    root.setPWM(servo2, 0, 600);
    root.setPWM(servo3, 0, 600);
    root.setPWM(servo4, 0, 600);
    moveStepperToAngle(45);
  } else if (position == 7) {
    root.setPWM(servo1, 0, 700);
    root.setPWM(servo2, 0, 700);
    root.setPWM(servo3, 0, 700);
    root.setPWM(servo4, 0, 700);
    moveStepperToAngle(135);
  } else if (position == 8) {
    root.setPWM(servo1, 0, 800);
    root.setPWM(servo2, 0, 800);
    root.setPWM(servo3, 0, 800);
    root.setPWM(servo4, 0, 800);
    moveStepperToAngle(180);
  }

  delay(500); // Wait for servo movements to complete
}

// Move servos and stepper motor to home position
void moveServosToHome() {
  Serial.println("Moving to Home Position");

  root.setPWM(servo1, 0, 150);
  root.setPWM(servo2, 0, 150);
  root.setPWM(servo3, 0, 150);
  root.setPWM(servo4, 0, 150);
  root.setPWM(servo5, 0, 150); // Gripper in home position
  moveStepperToAngle(homePosition);

  delay(500);
}

// Function to receive data and handle commands
void receiveData() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '$') {
      counterStart = true;
      counter = 0;
    }

    if (counterStart) {
      if (c >= '0' && c <= '9') {
        receivedarray[counter] = c;
        counter++;
      }

      if (counter == numOfValsRec) {
        counterStart = false;
      }
    }
  }
}

void loop() {
  receiveData();

  if (counter == numOfValsRec) {
    int x = receivedarray[0] - '0';
    int y = receivedarray[1] - '0';

    Serial.print("Received X: ");
    Serial.println(x);
    Serial.print("Received Y: ");
    Serial.println(y);

    if (x == 0 && y == 0) {
      moveServosToHome();
    } else {
      moveServosToPosition(x);

      if (y == 1) {
        Serial.println("Gripper: Open");
        root.setPWM(servo5, 0, 150);
      } else {
        Serial.println("Gripper: Closed");
        root.setPWM(servo5, 0, 600);
      }
    }

    for (int i = 0; i < numOfValsRec; i++) {
      receivedarray[i] = 0;
    }
    counter = 0;
  }
}
