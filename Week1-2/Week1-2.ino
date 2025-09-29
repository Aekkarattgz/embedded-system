#define NUM_LEDS 7

int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13};

void setup() {
  // ตั้งค่า pinMode ทุกตัวใน array
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // เปิด LED ทั้งหมด
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], HIGH);
  }
  delay(1000);

  // ปิด LED ทั้งหมด
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], LOW);
  }
  delay(1000);
}
