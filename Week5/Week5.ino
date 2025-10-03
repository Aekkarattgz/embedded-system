#include <Wire.h>                 // ไลบรารีสำหรับสื่อสาร I2C
#include <Adafruit_GFX.h>         // ไลบรารีกราฟิกพื้นฐานของ Adafruit (รองรับการแสดงผล)
#include <Adafruit_LEDBackpack.h> // ไลบรารีสำหรับโมดูล 7-seg HT16K33

// ---------------- LED 7 หลอดธรรมดา ----------------
#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13}; // GPIO ที่ต่อ LED 7 ดวง (แทน segment a–g)

// ---------------- ปุ่มกด ----------------
#define SW1 36   // ปุ่มนับขึ้น
#define SW2 39   // ปุ่มนับลง
#define SW3 34   // ปุ่มหยุด/เล่น (toggle pause)

// ---------------- 7-Segment แบบขาธรรมดา ----------------
// กำหนดพิน segment a,b,c,d,e,f,g ของ 7-seg (แบบ direct pin)
int segPins[7] = {4, 16, 17, 5, 18, 19, 21}; 

// ---------------- ตารางรูปแบบตัวเลข 0–9 ----------------
// แต่ละแถวคือ pattern ของตัวเลข (0–9) -> {a,b,c,d,e,f,g}
// ค่า 1 = เปิด segment, ค่า 0 = ปิด
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
Adafruit_7segment matrix = Adafruit_7segment(); // อ็อบเจ็กต์สำหรับควบคุมชิป HT16K33

// ---------------- ตัวแปรควบคุม ----------------
int currentNum = 0;           // เลขปัจจุบัน (เริ่มจาก 0)
bool countingUp = false;      // flag สำหรับนับขึ้น
bool countingDown = false;    // flag สำหรับนับลง
bool paused = false;          // flag สำหรับหยุด/กระพริบ
unsigned long lastUpdate = 0; // เก็บเวลาล่าสุดที่อัปเดต (ใช้ millis)

// ---------------- ฟังก์ชันแสดงเลข ----------------
void showNumber(int num) {
  // แสดงบน 7-segment แบบขาธรรมดา
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numberPatterns[num][i]); // เปิด/ปิด segment ตาม pattern
  }
}

void showNumberLEDs(int num) {
  // แสดงบน LED 7 ดวงธรรมดา
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], numberPatterns[num][i]); // เปิด/ปิด LED ตาม pattern
  }
}

void showNumberI2C(int num) {
  // แสดงบน 7-segment I2C HT16K33
  matrix.clear();        // ล้างหน้าจอ (เคลียร์ buffer)
  matrix.print(num);     // เขียนเลข num ลง buffer
  matrix.writeDisplay(); // ส่ง buffer ไปที่ชิปเพื่อแสดงผลจริง
}

// ---------------- setup ----------------
void setup() {
  // ตั้งค่า LED 7 ดวงเป็น OUTPUT
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
  }
  
  // ตั้งค่า 7-Segment ขาธรรมดาเป็น OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  // ตั้งค่าปุ่มกดเป็น INPUT
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);

  // เริ่มต้นแสดงเลข 0 บนทุกจอ
  showNumber(currentNum);
  showNumberLEDs(currentNum);

  // เริ่มต้นใช้งาน I2C สำหรับ HT16K33
  Wire.begin(22, 23);   // กำหนด SDA=22, SCL=23
  matrix.begin(0x70);   // ที่อยู่ I2C ของ HT16K33 (ปกติคือ 0x70)
  showNumberI2C(currentNum);
}

// ---------------- loop ----------------
void loop() {
  unsigned long currentMillis = millis(); // เวลา ณ ตอนนี้

  // ---------------- อ่านปุ่ม ----------------
  if (digitalRead(SW1) == 1) { // ถ้า SW1 ถูกกด
    countingUp = true;         // เริ่มนับขึ้น
    countingDown = false;      // ยกเลิกนับลง
    paused = false;            // ยกเลิกหยุด
    delay(200);                // หน่วงกันเด้งปุ่ม
  }
  if (digitalRead(SW2) == 1) { // ถ้า SW2 ถูกกด
    countingDown = true;       // เริ่มนับลง
    countingUp = false;        // ยกเลิกนับขึ้น
    paused = false;            // ยกเลิกหยุด
    delay(200);
  }
  if (digitalRead(SW3) == 1) { // ถ้า SW3 ถูกกด
    paused = !paused;          // toggle หยุด/เล่น
    delay(200);
  }

  // ---------------- กระพริบเมื่อ paused ----------------
  if (paused) {
    static bool blinkState = false;            // สถานะกระพริบ (คงค่าไว้ข้าม loop)
    if (currentMillis - lastUpdate > 500) {    // ทุก 0.5 วิ
      blinkState = !blinkState;                // สลับ on/off
      if (blinkState) {
        // แสดงตัวเลขปัจจุบัน
        showNumber(currentNum);
        showNumberLEDs(currentNum);
        showNumberI2C(currentNum);
      } else {
        // ปิดทุก segment/LED
        for (int i = 0; i < 7; i++) {
          digitalWrite(segPins[i], LOW);
          digitalWrite(leds[i], LOW);
        }
        matrix.clear();
        matrix.writeDisplay();
      }
      lastUpdate = currentMillis;              // อัปเดตเวลาล่าสุด
    }
    return; // ออกจาก loop → ไม่ทำการนับต่อ
  }

  // ---------------- นับเลขเมื่อไม่ paused ----------------
  if (currentMillis - lastUpdate > 1000) { // ทุก 1 วิ
    if (countingUp) {
      currentNum++;            // เพิ่มเลข
      if (currentNum > 9) currentNum = 0; // วนกลับ 0 ถ้าเกิน 9
    }
    if (countingDown) {
      currentNum--;            // ลดเลข
      if (currentNum < 0) currentNum = 9; // วนกลับ 9 ถ้าต่ำกว่า 0
    }

    // แสดงเลขปัจจุบันบนทั้ง 3 แบบ
    showNumber(currentNum);
    showNumberLEDs(currentNum);
    showNumberI2C(currentNum);

    lastUpdate = currentMillis; // อัปเดตเวลาล่าสุด
  }
}
