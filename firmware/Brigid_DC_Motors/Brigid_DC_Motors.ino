#define NUM_SOLENOIDS 6
// botnum/board type/board num
// pre-processor macros, never change //
#define BOT_ID 1
#define BOT_TYPE 1
#define ARDUINO_ID 5

#define BOOT_TEST 0
#define LED_FEEDBACK 1

#include <avr/interrupt.h>
#include <avr/io.h>

#define RED_LED A4
#define GREEN_LED A3
#define BLUE_LED A2

char bytes[2];
short notes[NUM_SOLENOIDS];

int handshake = 0;
int statustimer = 0;

// actuator pins
int actuators[] = {
  3, 5, 6, 9, 10, 11
};

void setup() {
  Serial.begin(57600);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);

  for (int i = 0; i < NUM_SOLENOIDS; i++) {
    pinMode(actuators[i], OUTPUT);
    digitalWrite(actuators[i], LOW);
  }
  if (BOOT_TEST == 1) {
    bootRoutine();
  }
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void bootRoutine() {
  for (int i; i < 6; i++) {
    digitalWrite(actuators[i], HIGH);
    delay(100);
    digitalWrite(actuators[i], LOW);
    delay(1000);
  }
}

void analogLoop(int delayTime) {
  for (int i = 255; i > 0; i--) {
    analogWrite(actuators[0], i);
    delay(delayTime);
    //digitalWrite(actuators[0], LOW);
    //delay(500);
  }
}

// this is still blocking, but its not the worst thing in the world
void silentAnalogWrite(int actNum[], int vel) {
  if (vel > 100) {
    vel = 100;
  }
  for (int i = 0; i < sizeof(ActNum) / sizeof(int); i++) {
    digitalWrite(actuators[actNum[i]], HIGH);
    delay(vel);
    digitalWrite(actuators[actNum[i]], LOW);
    delay(100 - vel);
  }
}

void loop() {
  int actNum[] = {0, 1, 4};
  for (int i = 100; i > 0; i--) {
    for (int t = 0; t < 10; t++) {
      silentAnalogWrite(actNum, i);
    }
  }
}







