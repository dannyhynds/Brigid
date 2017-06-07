#define NUM_SOLENOIDS 6
// botnum/board type/board num
#define BOT_ID 1
#define BOT_TYPE 1
#define ARDUINO_ID 4

// will trigger each solinoid during init
#define BOOT_TEST 1
// will flash LED's when board receives message
#define LED_FEEDBACK 1

#include <avr/interrupt.h>
#include <avr/io.h>

#define RED_LED A4
#define GREEN_LED A3
#define BLUE_LED A2

char bytes[2];
short note_active[NUM_SOLENOIDS];

int handshake = 0;
int statustimer = 0;

// actuator pins
int actuators[] = {
  3, 5, 6, 9, 10, 11
};

void setup() {
  Serial.begin(57600);
  // interrupt timer parameters
  TCCR2A = 1;
  TCCR2B = 3;
  TIMSK2 = 1;
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
    bootRoutine(350);
  }
}

ISR(TIMER2_OVF_vect) {
  // solenoid control
  for (int i = 0; i < NUM_SOLENOIDS; i++) {
    if (note_active[i] > 0) {
      digitalWrite(actuators[i], HIGH);
      note_active[i]--;
    }
    else {
      digitalWrite(actuators[i], LOW);
    }
  }
  if (statustimer > 0 && LED_FEEDBACK) {
    digitalWrite(BLUE_LED, HIGH);
    statustimer--;
  }
  else {
    digitalWrite(BLUE_LED, LOW);
  }
}

void bootRoutine(int delayTime) {
  for (int i; i < NUM_SOLENOIDS; i++) {
    note_active[i] = 50;
    delay(delayTime);
  }
}

void pollSerial() {
  if (Serial.available()) {
    if (Serial.read() == 0xff) {
      // reads in a two index array from ChucK
      Serial.readBytes(bytes, 2);
      // bit wise operations
      // ~~~~~~~~~~~~~~~~~~~
      // reads the first six bits for the note number
      // then reads the last ten bits for the note velocity
      int pitch = byte(bytes[0]) >> 2;
      int velocity = (byte(bytes[0]) << 8 | byte(bytes[1])) & 1023;
      // message required for "handshake" to occur
      // happens once per Arduino at the start of the ChucK serial code
      if (pitch == 63 && velocity == 1023 && handshake == 0) {
        Serial.write(BOT_ID);
        Serial.write(BOT_TYPE);
        Serial.write(ARDUINO_ID);
        handshake = 1;
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, LOW);
      }
      if (pitch >= 0 && pitch <= NUM_SOLENOIDS) {
        statustimer = 120;
        note_active[pitch] = (velocity * 0.5);
      }
    }
  }
}

void loop() {
  pollSerial();
}
