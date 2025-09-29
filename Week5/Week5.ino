#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// ---------------- LED 7 หลอดธรรมดา ----------------
#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13}; // GPIO ของ LED 7 หลอด

// ---------------- ปุ่มกด ----------------
#define SW1 36   // นับขึ้น
#define SW2 39   // นับลง
#define SW3 34   // หยุด/กระพริบ

// ---------------- 7-Segment แบบขาธรรมดา ----------------
// a, b, c, d, e, f, g
int segPins[7] = {4, 16, 17, 5, 18, 19, 21}; 

// รูปแบบตัวเลข 0–9 สำหรับ LED และ 7-segment ธรรมดา
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

// ---------------- 7-Segment I2C HT16K33 ----------------
Adafruit_7segment matrix = Adafruit_7segment();

// ---------------- ตัวแปรควบคุม ----------------
int currentNum = 0;           // เลขปัจจุบัน
bool countingUp = false;      // นับขึ้น
bool countingDown = false;    // นับลง
bool paused = false;          // หยุด/กระพริบ
unsigned long lastUpdate = 0; // เวลาอัปเดตล่าสุด

// ---------------- ฟังก์ชันแสดงเลข ----------------
void showNumber(int num) {
  // แสดงบน 7-segment แบบขาธรรมดา
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numberPatterns[num][i]);
  }
}

void showNumberLEDs(int num) {
  // แสดงบน LED 7 หลอด
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], numberPatterns[num][i]);
  }
}

void showNumberI2C(int num) {
  // แสดงบน 7-segment I2C HT16K33
  matrix.clear();
  matrix.print(num);
  matrix.writeDisplay();
}

// ---------------- setup ----------------
void setup() {
  // ตั้งค่า LED 7 หลอดเป็น OUTPUT
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
  }
  
  // ตั้งค่า 7-Segment ขาธรรมดาเป็น OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  // ตั้งค่าปุ่มกดเป็น INPUT_PULLUP
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);

  // เริ่มต้นแสดงเลข 0
  showNumber(currentNum);
  showNumberLEDs(currentNum);

  // I2C HT16K33
  Wire.begin(22, 23); // SDA = 22, SCL = 23
  matrix.begin(0x70); // ที่อยู่ I2C ปกติ
  showNumberI2C(currentNum);
}

// ---------------- loop ----------------
void loop() {
  unsigned long currentMillis = millis();

  // อ่านปุ่ม (LOW = กด)
  if (digitalRead(SW1) == 1) {
    countingUp = true;
    countingDown = false;
    paused = false;
    delay(200); // กันเด้ง
  }
  if (digitalRead(SW2) == 1) {
    countingDown = true;
    countingUp = false;
    paused = false;
    delay(200);
  }
  if (digitalRead(SW3) == 1) {
    paused = !paused; // toggle หยุด/เล่น
    delay(200);
  }

  // ---------------- กระพริบเมื่อ paused ----------------
  if (paused) {
    static bool blinkState = false;
    if (currentMillis - lastUpdate > 500) { // ทุก 0.5 วิ
      blinkState = !blinkState;
      if (blinkState) {
        showNumber(currentNum);
        showNumberLEDs(currentNum);
        showNumberI2C(currentNum);
      } else {
        for (int i = 0; i < 7; i++) {
          digitalWrite(segPins[i], LOW);
          digitalWrite(leds[i], LOW);
        }
        matrix.clear();
        matrix.writeDisplay();
      }
      lastUpdate = currentMillis;
    }
    return; // ไม่ต้องนับต่อ
  }

  // ---------------- นับเลขเมื่อไม่ paused ----------------
  if (currentMillis - lastUpdate > 1000) { // ทุก 1 วิ
    if (countingUp) {
      currentNum++;
      if (currentNum > 9) currentNum = 0;
    }
    if (countingDown) {
      currentNum--;
      if (currentNum < 0) currentNum = 9;
    }

    // แสดงเลข
    showNumber(currentNum);
    showNumberLEDs(currentNum);
    showNumberI2C(currentNum);

    lastUpdate = currentMillis;
  }
}
