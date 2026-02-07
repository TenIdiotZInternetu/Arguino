constexpr int ON = 1;
constexpr int OFF = 0;

constexpr int LEDS[] { 6, 7, 8, 9 };
constexpr int LED_COUNT = sizeof(LEDS) / sizeof(LEDS[0]);

constexpr int COUNTER_INCREMENT = 1;
constexpr int COUNTER_DECREMENT = 1;

constexpr unsigned int AUTOREPEAT_DELAY_MS = 1000;     // how long before button starts to perform periodic updates [ms]
constexpr unsigned int AUTOREPEAT_INTERVAL_MS = 300;   // delay between consecutive periodic updates [ms]
constexpr unsigned int JITTER_SPAN_MS = 25;

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

unsigned int counter = 0;

Button INCREMENT_BUTTON(2);
Button DECREMENT_BUTTON(3);

void init_leds() 
{
    for (int i = 0; i < LED_COUNT; i++) {
        pinMode(LEDS[i], OUTPUT);
        digitalWrite(LEDS[i], OFF);
    }
}

void num_to_leds(int number)
{
    short mask = 1;

    for (int i = 0; i < LED_COUNT; i++) {
        if (number & mask) {
            digitalWrite(LEDS[i], ON);
        }
        else {
            digitalWrite(LEDS[i], OFF);
        }

        mask = mask << 1;
    } 
}

void setup() 
{
    init_leds();
    pinMode(INCREMENT_BUTTON.GetPin(), INPUT);
    pinMode(DECREMENT_BUTTON.GetPin(), INPUT);
}

void loop() 
{
    unsigned long elapsed_time = millis();
    
    if (INCREMENT_BUTTON.SignalsAction(elapsed_time)) {
        counter += COUNTER_INCREMENT;
        num_to_leds(counter);
    }
    
    if (DECREMENT_BUTTON.SignalsAction(elapsed_time)) {
        counter -= COUNTER_DECREMENT;
        num_to_leds(counter);
    }
}
