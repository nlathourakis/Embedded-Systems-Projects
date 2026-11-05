const int RED_PIN = 12;
const int YELLOW_PIN = 8;
const int GREEN_PIN = 7;
const int BUTTON_PIN = 2;

int state = 0;                 // 0 = green, 1 = yellow, 2 = red
unsigned long stateStart = 0;  // when we entered the current state

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  unsigned long elapsed = millis() - stateStart;
  int pressed = (digitalRead(BUTTON_PIN) == LOW);

  if (state == 0 && pressed) {
    state = 1;
    stateStart = millis();
  }
  else if (state == 1 && elapsed >= 2000) {
    state = 2;
    stateStart = millis();
  }
  else if (state == 2 && elapsed >= 5000) {
    state = 0;
    stateStart = millis();
  }

  digitalWrite(GREEN_PIN,  state == 0);
  digitalWrite(YELLOW_PIN, state == 1);
  digitalWrite(RED_PIN,    state == 2);
}