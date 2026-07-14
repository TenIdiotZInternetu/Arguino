constexpr int BUTTON_PIN = 0;
constexpr int FIRST_LED_PIN = 1;
constexpr int LAST_LED_PIN = 13;

constexpr int MAX_WAVES = 10;
constexpr unsigned long NO_WAVE = 0;
constexpr unsigned long WAVE_STEP_MS = 100;
constexpr int WAVE_MAX_STEPS = 13;

constexpr unsigned int AUTOREPEAT_DELAY_MS = 1000;     // how long before button starts to perform periodic updates [ms]
constexpr unsigned int AUTOREPEAT_INTERVAL_MS = 300;   // delay between consecutive periodic updates [ms]
constexpr unsigned int JITTER_SPAN_MS = 25;

constexpr int ON = 1;
constexpr int OFF = 0;

enum ButtonState { BTN_OFF, BTN_ON, BTN_AUTOREPEAT};

class Button {
    public:
        Button(int input_pin) {
            BS = BTN_OFF;
            pin = input_pin;
            // pinMode(pin, INPUT);
        }
        
        bool SignalsAction(unsigned long timestamp) {
            bool isSignalOn = false;

            if (Jitters(timestamp)) {
                return false;
            }

            if (digitalRead(pin) == ON) {
                isSignalOn = OnPressDown(timestamp);
            } else {
                OnRelease(timestamp);
            }

            return isSignalOn;
        }

        int GetPin() {
            return pin;
        }

    private:
        int pin;
        ButtonState BS;
        
        unsigned long time_when_pressed = 0;
        unsigned long time_of_last_action = 0;
        unsigned long time_when_released = 0;

        bool OnPressDown(unsigned long timestamp) {
            switch (BS) {
                case BTN_OFF:
                    return ActivateButton(timestamp);

                case BTN_ON:
                    return WaitForAutorepeat(timestamp);

                case BTN_AUTOREPEAT:
                    return Autorepeat(timestamp);
            }

            return false;            
        }

        void OnRelease(unsigned long timestamp) {
            if (BS != BTN_OFF) {
                time_when_released = timestamp;
                BS = BTN_OFF;
            }
        }

        bool Jitters(unsigned long timestamp) {
            if (BS == BTN_OFF) {
                return timestamp - time_when_released < JITTER_SPAN_MS;
            } else {
                return timestamp - time_when_pressed < JITTER_SPAN_MS;
            }
        }

        bool ActivateButton(unsigned long timestamp) {
            BS = BTN_ON;
            time_when_pressed = timestamp;

            return true;
        }

        bool WaitForAutorepeat(unsigned long timestamp) {
            if (timestamp - time_when_pressed >= AUTOREPEAT_DELAY_MS) {
                BS = BTN_AUTOREPEAT;
                time_of_last_action = timestamp;

                return true;
            }

            return false;
        }

        bool Autorepeat(unsigned long timestamp) {
            if (timestamp - time_of_last_action >= AUTOREPEAT_INTERVAL_MS) {
                time_of_last_action = timestamp;
                return true;                
            }

            return false;
        }
};

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