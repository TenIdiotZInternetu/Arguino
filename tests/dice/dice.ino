#include "button.h"
#include "funshield.h"
#include "segment_display.h"

class RandomNumberGenerator {
public:
  unsigned long nextNumber(unsigned long max, unsigned long wildcard,
                           unsigned long timestamp) {
    seed = (seed + wildcard) * timestamp % SEED_CAP;
    return (seed % max) + 1;
  }

private:
  static constexpr unsigned long SEED_CAP = 12321654;
  unsigned long seed = 1;
};

enum DISPLAY_MODE { ROLLING, RESULT, CONFIG };
enum DICE_TYPE { D4, D6, D8, D10, D12, D20, D100, Count };
constexpr int MIN_THROW_COUNT = 1;
constexpr int MAX_THROW_COUNT = 9;
constexpr int ROLLING_ANIMATION_FRAMERATE = 60;

const int SIDE_COUNT[] = {4, 6, 8, 10, 12, 20, 100};

Button ROLL_BUTTON(button1_pin);
Button CHANGE_COUNT_BUTTON(button2_pin);
Button CHANGE_TYPE_BUTTON(button3_pin);

SegmentDisplay SEGDISPLAY;
RandomNumberGenerator RNG;

DISPLAY_MODE currentDisplayMode;
DICE_TYPE currentDiceType;
byte currentThrowCount;

unsigned int throwResult;
unsigned short buttonPresses;

void init_buttons() {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTON_PINS[i], INPUT);
  }
}

void changeDisplayMode(enum DISPLAY_MODE mode);
void changeDisplayMode(enum DISPLAY_MODE mode) {
  currentDisplayMode = mode;

  if (mode == RESULT) {
    SEGDISPLAY.display_number(throwResult);
  }

  if (mode == CONFIG) {
    SEGDISPLAY.display_number(SIDE_COUNT[currentDiceType]);
    SEGDISPLAY.set_content(0, currentThrowCount);
    SEGDISPLAY.set_content(1, 'd');
  }
}

void rollingAnimation(unsigned long timestamp) {
  static unsigned long lastTimestamp = 0;
  static int frame = 0;

  for (int i = 0; i < SEGDISPLAY.SEGMENT_COUNT; i++) {
    SEGDISPLAY.set_content(i, SEGDISPLAY.EMPTY_GLYPH);
  }

  SEGDISPLAY.set_content(frame, 1);

  if (timestamp - lastTimestamp > ROLLING_ANIMATION_FRAMERATE) {
    frame = (frame + 1);

    if (frame >= SEGDISPLAY.SEGMENT_COUNT) {
      frame = 0;
    }

    lastTimestamp = timestamp;
  }
}

void setup() {
  init_buttons();
  SEGDISPLAY.init_registers();

  currentDisplayMode = CONFIG;
  currentDiceType = D6;
  currentThrowCount = 1;
  buttonPresses = 1;

  changeDisplayMode(CONFIG);
}

void loop() {
  unsigned long elapsedTime = millis();
  SEGDISPLAY.multiplex(elapsedTime);

  if (currentDisplayMode == ROLLING && !ROLL_BUTTON.isHeld()) {
    throwResult = 0;

    for (int i = 0; i < currentThrowCount; i++) {
      int numThrown = RNG.nextNumber(SIDE_COUNT[currentDiceType], buttonPresses,
                                     elapsedTime);
      throwResult += numThrown;
    }

    changeDisplayMode(RESULT);
  }

  if (ROLL_BUTTON.isHeld()) {
    RNG.nextNumber(SIDE_COUNT[currentDiceType], buttonPresses, elapsedTime);
    buttonPresses++;
    changeDisplayMode(ROLLING);
    rollingAnimation(elapsedTime);
  }

  else if (CHANGE_COUNT_BUTTON.SignalsAction(elapsedTime)) {
    currentThrowCount++;

    if (currentThrowCount > MAX_THROW_COUNT) {
      currentThrowCount = MIN_THROW_COUNT;
    }

    buttonPresses++;
    changeDisplayMode(CONFIG);
  }

  else if (CHANGE_TYPE_BUTTON.SignalsAction(elapsedTime)) {
    currentDiceType = static_cast<DICE_TYPE>(currentDiceType + 1);

    if (currentDiceType >= Count) {
      currentDiceType = D4;
    }

    buttonPresses++;
    changeDisplayMode(CONFIG);
  }
}