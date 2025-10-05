void setup() {
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
}

void loop() {
    static unsigned long last_time_changed = 0;
    unsigned long elapsed_time = millis();
    byte ledState = LOW;

    if (elapsed_time - last_time_changed >= INTERVAL) {
        ledState = !ledState;
        digitalWrite(3, ledState);
        last_time_changed = elapsed_time;
    }
}