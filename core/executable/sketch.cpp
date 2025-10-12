#include <Arduino.h>
#line 1 "/home/touster/Kodiky/arguino/arguino.ino"
constexpr unsigned long INTERVAL = 2000;
byte ledState = LOW;

#line 4 "/home/touster/Kodiky/arguino/arguino.ino"
void setup();
#line 9 "/home/touster/Kodiky/arguino/arguino.ino"
void loop();
#line 4 "/home/touster/Kodiky/arguino/arguino.ino"
void setup() {
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
}

void loop() {
    static unsigned long last_time_changed = 0;
    unsigned long elapsed_time = millis();

    if (elapsed_time - last_time_changed >= INTERVAL) {
        ledState = !ledState;
        digitalWrite(3, ledState);
        last_time_changed = elapsed_time;
    }
}
