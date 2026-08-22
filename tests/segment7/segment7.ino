#include "button.h"
#include "funshield.h"

constexpr int DIGITS_COUNT = 4;
constexpr int MAX_DIGIT = 9;
constexpr int MIN_DIGIT = 0;

const byte segmentMap[] = {
    0xC0, // 0  0b11000000
    0xF9, // 1  0b11111001
    0xA4, // 2  0b10100100
    0xB0, // 3  0b10110000
    0x99, // 4  0b10011001
    0x92, // 5  0b10010010
    0x82, // 6  0b10000010
    0xF8, // 7  0b11111000
    0x80, // 8  0b10000000
    0x90  // 9  0b10010000
};

int currentSegment = 0;
int digitsSegs[DIGITS_COUNT];

Button INCREMENT_BUTTON(button1_pin);
Button DECREMENT_BUTTON(button2_pin);
Button CHANGE_ORDER_BUTTON(button3_pin);

void init_buttons() {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTON_PINS[i], INPUT);
  }
}

void update_7seg(int segment, int value) {
  shiftOut(data_pin, clock_pin, MSBFIRST, segmentMap[value]);
  shiftOut(data_pin, clock_pin, MSBFIRST, 0b1000 >> segment);

  digitalWrite(latch_pin, HIGH);
  digitalWrite(latch_pin, LOW);
}

void init_7seg_registers() {
  pinMode(latch_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);

  digitalWrite(latch_pin, LOW);
  digitalWrite(data_pin, LOW);
  update_7seg(0, 0);
}

void increment_counter(int order) {
  if (order > (DIGITS_COUNT - 1) || order < 0) {
    return;
  }

  if (digitsSegs[order] == MAX_DIGIT) {
    increment_counter(order + 1);
    digitsSegs[order] = MIN_DIGIT;
  } else {
    digitsSegs[order] += 1;
  }
}

void decrement_counter(int order) {
  if (order > (DIGITS_COUNT - 1) || order < 0) {
    return;
  }

  if (digitsSegs[order] == MIN_DIGIT) {
    decrement_counter(order + 1);
    digitsSegs[order] = MAX_DIGIT;
  } else {
    digitsSegs[order] -= 1;
  }
}

void setup() {
  init_buttons();
  // init_leds();
  init_7seg_registers();

  for (int i = 0; i < DIGITS_COUNT; i++) {
    digitsSegs[i] = 0;
  }
}

void loop() {
  unsigned long elapsed_time = millis();

  if (INCREMENT_BUTTON.SignalsAction(elapsed_time)) {
    increment_counter(currentSegment);
    update_7seg(currentSegment, digitsSegs[currentSegment]);
  }

  if (DECREMENT_BUTTON.SignalsAction(elapsed_time)) {
    decrement_counter(currentSegment);
    update_7seg(currentSegment, digitsSegs[currentSegment]);
  }

  if (CHANGE_ORDER_BUTTON.SignalsAction(elapsed_time)) {
    currentSegment += 1;

    if (currentSegment >= DIGITS_COUNT) {
      currentSegment = 0;
    }

    update_7seg(currentSegment, digitsSegs[currentSegment]);
  }
}