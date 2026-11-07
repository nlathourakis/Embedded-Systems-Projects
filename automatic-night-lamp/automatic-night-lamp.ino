const int threshold = 400;
int previous_state = 0;
const int hysteresis = 20;
void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop() {
  int x = analogRead(A0);
  Serial.print("raw = ");
  Serial.print(x);
  Serial.print(" voltage = ");
  float voltage = (x / 1023.0) * 5.0;
  Serial.print(voltage);
  Serial.println("V");
  int y = max(((threshold - x) * 255.0 / threshold), 0);
  if (x < threshold - hysteresis) {
    analogWrite(9, y);
    previous_state = y;
  } else if (x > threshold + hysteresis) {
    analogWrite(9, 0);
    previous_state = y;
  } else {
    analogWrite(9, previous_state);
  }

  delay(200);
}
