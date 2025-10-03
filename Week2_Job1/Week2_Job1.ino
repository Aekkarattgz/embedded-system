#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13};

void setup() {
  for(int i =0; i< NUM_LEDS; i++){
    pinMode(leds[i],OUTPUT);
  }
}

void loop() {
  // เปิด LED ทั้งหมด
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], 1);
  }
  delay(1000);

  // ปิด LED ทั้งหมด
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(leds[i], 0);
  }
  delay(1000);

  // ------- ลูกเล่นไฟวิ่ง -------
  for (int i = 0; i < NUM_LEDS; i+=2) {
    digitalWrite(leds[i], 1);  // เปิดทีละดวง
  }
  delay(1000);
  for (int i = 0; i < NUM_LEDS; i+=2) {
    digitalWrite(leds[i], 0);  // เปิดทีละดวง
  }
  delay(1000);
  for (int i = 1; i < NUM_LEDS; i+=2) {
    digitalWrite(leds[i], 1);  // เปิดทีละดวง
  }
  delay(1000);
  for (int i = 1; i < NUM_LEDS; i+=2) {
    digitalWrite(leds[i], 0);  // เปิดทีละดวง
  }
  delay(1000);
}
