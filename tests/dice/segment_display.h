#ifndef ARDUINO_SEGMENT_DISPLAY__H
#define ARDUINO_SEGMENT_DISPLAY__H

#include "funshield.h"

class SegmentDisplay {
public:
  static constexpr int SEGMENT_COUNT = 4;
  static constexpr byte EMPTY_GLYPH = 0b11111111;

  void init_registers() {
    pinMode(latch_pin, OUTPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);

    digitalWrite(latch_pin, LOW);
    digitalWrite(data_pin, LOW);
    this->update(0, 0);
  }

  void multiplex(unsigned long elapsed_time) {
    static int currentSegment = 0;
    static unsigned long last_time_refreshed = 0;

    if (elapsed_time - last_time_refreshed <= REFRESH_RATE) {
      return;
    }

    currentSegment++;

    if (currentSegment > (SEGMENT_COUNT - 1)) {
      currentSegment = 0;
    }

    this->update(currentSegment, content[currentSegment]);
    last_time_refreshed = elapsed_time;
  }

  void set_content(byte position, byte value) {
    if (position >= 0 && position <= SEGMENT_COUNT) {
      content[position] = value;
    }
  }

  void display_number(int number) {
    int divisor = 1;
    bool foundFirstDigit = false;

    for (int i = 1; i < SEGMENT_COUNT; i++) {
      divisor *= 10;
    }

    for (int i = 0; i < SEGMENT_COUNT; i++) {
      int digit = number / divisor % 10;

      if (digit != 0 || foundFirstDigit) {
        foundFirstDigit = true;
        content[i] = digit;
      } else {
        content[i] = EMPTY_GLYPH;
      }

      divisor /= 10;
    }
  }

private:
  static constexpr int REFRESH_RATE = 1;
  static constexpr int MAX_DIGIT = 9;
  static constexpr int MIN_DIGIT = 0;

  static constexpr byte LETTER_GLYPH[]{
      0b10001000, // A
      0b10000011, // b
      0b11000110, // C
      0b10100001, // d
      0b10000110, // E
      0b10001110, // F
      0b10000010, // G
      0b10001001, // H
      0b11111001, // I
      0b11100001, // J
      0b10000101, // K
      0b11000111, // L
      0b11001000, // M
      0b10101011, // n
      0b10100011, // o
      0b10001100, // P
      0b10011000, // q
      0b10101111, // r
      0b10010010, // S
      0b10000111, // t
      0b11000001, // U
      0b11100011, // v
      0b10000001, // W
      0b10110110, // ksi
      0b10010001, // Y
      0b10100100, // Z
  };

  byte content[SEGMENT_COUNT];

  void update(int segment, byte value) {
    byte glyph = EMPTY_GLYPH;

    if (isAlpha(value)) {
      glyph = LETTER_GLYPH[value - (isUpperCase(value) ? 'A' : 'a')];
    }

    if (value < 10) {
      glyph = digits[value];
    }

    shiftOut(data_pin, clock_pin, MSBFIRST, glyph);
    shiftOut(data_pin, clock_pin, MSBFIRST, 0b0001 << segment);

    digitalWrite(latch_pin, HIGH);
    digitalWrite(latch_pin, LOW);
  }
};

constexpr byte SegmentDisplay::LETTER_GLYPH[];

#endif