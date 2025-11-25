int leds[7] = {32, 33, 25, 26, 27, 12, 13};
void setup() {
  Serial.begin(115200);  
  // ตั้งค่า pinMode ทุกตัวใน array
  for (int i = 0; i < 7; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // เปิด LED ทั้งหมด
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], 1);
  }
  delay(1000);

  // ปิด LED ทั้งหมด
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], 0);
  }
  delay(1000);

  // ------- ลูกเล่นไฟวิ่ง -------
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], 1);  // เปิดทีละดวง
    delay(300);
    digitalWrite(leds[i], 0);   // ดับทีละดวง
  }

}
