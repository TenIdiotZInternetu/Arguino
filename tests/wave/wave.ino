#include "button.h"

constexpr int BUTTON_PIN = 0;
constexpr int FIRST_LED_PIN = 1;
constexpr int LAST_LED_PIN = 13;

constexpr int MAX_WAVES = 32;
constexpr unsigned long NO_WAVE = 0;
constexpr unsigned long WAVE_STEP_MS = 100;
constexpr int WAVE_MAX_STEPS = 13;


Button triggerButton(BUTTON_PIN);
unsigned long waveStartTimes[MAX_WAVES];
unsigned long wavesPerStep[WAVE_MAX_STEPS];

unsigned long lastWaveSteps[MAX_WAVES]; 

void registerNewWave(unsigned long timestamp) {
    unsigned long targetTime = (timestamp == NO_WAVE) ? 1 : timestamp;
    for (int i = 0; i < MAX_WAVES; ++i) {
        if (waveStartTimes[i] == NO_WAVE) {
            waveStartTimes[i] = targetTime;
            lastWaveSteps[i] = 0;
            
            int pinToActivate = FIRST_LED_PIN;
            digitalWrite(pinToActivate, HIGH);
            ++wavesPerStep[0];
            break; 
        }
    }
}

void clearStep(int step) {
    if (step < 0 || step >= WAVE_MAX_STEPS) return;
    
    int pinToDeactivate = FIRST_LED_PIN + step;
    if (wavesPerStep[step] > 0) {
        --wavesPerStep[step];
    }
    digitalWrite(pinToDeactivate, wavesPerStep[step] != 0);
}

void updateWaves(unsigned long current_time) {
    for (int i = 0; i < MAX_WAVES; ++i) {
        if (waveStartTimes[i] == NO_WAVE) {
            continue;
        }

        unsigned long elapsed = current_time - waveStartTimes[i];
        unsigned long activeStep = elapsed / WAVE_STEP_MS;

        if (activeStep < WAVE_MAX_STEPS) {
            // ONLY run this logic if the wave has transitioned to a new step
            if (activeStep == lastWaveSteps[i]) continue;

            clearStep(lastWaveSteps[i]);
            
            lastWaveSteps[i] = activeStep;
            int pinToActivate = FIRST_LED_PIN + activeStep;
            digitalWrite(pinToActivate, HIGH);
            ++wavesPerStep[activeStep];
        } else {
            // Wave finished: clear the final step and retire the wave
            clearStep(lastWaveSteps[i]);
            waveStartTimes[i] = NO_WAVE;
            lastWaveSteps[i] = 0;
        }
    }
}

void setup() {
    pinMode(BUTTON_PIN, INPUT);
    for (int pin = FIRST_LED_PIN; pin <= LAST_LED_PIN; ++pin) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    for (int i = 0; i < MAX_WAVES; ++i) {
        waveStartTimes[i] = NO_WAVE;
        lastWaveSteps[i] = 0;
    }
    
    for (int i = 0; i < WAVE_MAX_STEPS; ++i) {
        wavesPerStep[i] = 0;
    }
}

void loop() {
    unsigned long now = millis();

    if (triggerButton.SignalsAction(now)) {
        registerNewWave(now);
    }

    updateWaves(now);
}