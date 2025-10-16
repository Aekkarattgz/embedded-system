#include <Wire.h>                 // ไลบรารีสำหรับสื่อสาร I2C
#include <Adafruit_GFX.h>         // ไลบรารีกราฟิกพื้นฐาน
#include <Adafruit_LEDBackpack.h> // ไลบรารีสำหรับโมดูล 7-seg HT16K33

// ---------------- 7-Segment แบบขาธรรมดา (direct pin) ----------------
// กำหนดพิน segment a,b,c,d,e,f,g
int segPins[7] = {4, 16, 17, 5, 18, 19, 21};

// ---------------- ตารางรูปแบบตัวเลข 0–9 ----------------
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
int currentNum = 0;   // เลขที่รับจาก Serial
String inputString;   // buffer สำหรับข้อความที่พิมพ์เข้ามา

// ---------------- ฟังก์ชันแสดงเลข ----------------ฤ
void showNumberDirect(int num) {
  if (num < 0 || num > 9) {
    // ถ้าไม่ใช่ 0–9 ให้ปิด segment
    for (int i = 0; i < 7; i++) digitalWrite(segPins[i], LOW);
    return;
  }
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numberPatterns[num][i]);
  }
}

void showNumberI2C(int num) {
  matrix.clear();
  matrix.print(num);     // แสดงตัวเลข (0–9999)
  matrix.writeDisplay();
}

// ---------------- setup ----------------
void setup() {
  Serial.begin(115200);
  while (!Serial) ; // รอจนกว่า Serial พร้อม
  Serial.println("กกรุณาพิมพ์ตัวเลข 0–9999 แล้วกด Enter");

  // ตั้งค่า 7-Segment ขาธรรมดาเป็น OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  // เริ่มต้นใช้งาน I2C สำหรับ HT16K33
  Wire.begin(22, 23);   // กำหนด SDA=22, SCL=23
  matrix.begin(0x70);   // ที่อยู่ I2C ปกติคือ 0x70

  // เริ่มต้นแสดงเลข 0
  showNumberI2C(0);
  showNumberDirect(0);
}

// ---------------- loop ----------------
void loop() {
  // ถ้ามีข้อมูลเข้ามาจาก Serial
  if (Serial.available() > 0) {
    inputString = Serial.readStringUntil('\n'); // อ่านจนกด Enter
    inputString.trim();                         // ลบช่องว่าง

    if (inputString.length() > 0) {
      int num = inputString.toInt();            // แปลงเป็น int

      if (num >= 0 && num <= 9999) {
        currentNum = num;

        // แสดงบน 7-seg I2C
        showNumberI2C(currentNum);

        // แยกหลักร้อย (หลักที่ 3)
        int digit3 = (currentNum / 100) % 10;

        // แสดงบน 7-seg ขาธรรมดา
        showNumberDirect(digit3);

        // Debug บน Serial Monitor
        Serial.print("รับค่า: ");
        Serial.print(currentNum);
        Serial.print(" -> หลักร้อย = ");
        Serial.println(digit3);
        Serial.println("กรุณาพิมพ์ตัวเลข 0–9999 แล้วกด Enter");
      } else {
        Serial.println("กรุณาพิมพ์ตัวเลข 0–9999 เท่านั้น");
      }
    }
  }
}