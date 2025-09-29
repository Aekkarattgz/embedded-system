// ---------------- LED 7 หลอดธรรมดา ----------------
#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13}; // LED 7 หลอดต่อกับ GPIO

// ---------------- 7-Segment Display ----------------
// a, b, c, d, e, f, g
int segPins[7] = {4, 16, 17, 5, 18, 19, 21}; // ขาที่ต่อกับ segment

// รูปแบบการเปิด/ปิด segment สำหรับเลข 0–9
byte numberPatterns[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

// ฟังก์ชันแสดงเลขบน 7-Segment
void showNumber(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numberPatterns[num][i]);
  }
}

// ฟังก์ชันควบคุม LED 7 หลอดธรรมดา
// ให้เปิด/ปิดตามรูปแบบเดียวกับ 7-Segment
void showNumberLEDs(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], numberPatterns[num][i]);
  }
}

void setup() {
  // ตั้งค่า LED 7 หลอดเป็น OUTPUT
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
  }
  
  // ตั้งค่า 7-Segment เป็น OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }
}

void loop() {
  // แสดงเลข 0–9 ทีละ 1 วินาที
  for (int n = 0; n <= 9; n++) {
    showNumber(n);       // แสดงที่ 7-Segment
    showNumberLEDs(n);   // แสดงด้วย LED 7 หลอดธรรมดา
    delay(1000);
  }
}
