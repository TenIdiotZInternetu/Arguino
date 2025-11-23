constexpr unsigned long INTERVAL = 2000;
byte ledState = LOW;

void setup() {
  pinMode(1, OUTPUT);
  pinMode(8, INPUT);
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
  digitalWrite(1, digitalRead(8));
}