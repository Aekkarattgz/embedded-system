#include <Wire.h>                 // ไลบรารีสำหรับสื่อสาร I2C
#include <Adafruit_GFX.h>         // ไลบรารีกราฟิกพื้นฐาน
#include <Adafruit_LEDBackpack.h> // ไลบรารีสำหรับโมดูล 7-seg HT16K33
// ---------------- 7-Segment I2C HT16K33 ----------------
Adafruit_7segment matrix = Adafruit_7segment();

// ---------------- ตัวแปรควบคุม ----------------
int currentNum = 0;   // เลขที่รับจาก Serial
void showNumberI2C(int num) {
  matrix.clear();
  matrix.print(num);     // แสดงตัวเลข (0–9999)
  matrix.writeDisplay();
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // เริ่มต้นใช้งาน I2C สำหรับ HT16K33
  Wire.begin(22, 23);   // กำหนด SDA=22, SCL=23
  matrix.begin(0x70);   // ที่อยู่ I2C ปกติคือ 0x70
  // เริ่มต้นแสดงเลข 0
  showNumberI2C(999);
}

void loop() {
  // put your main code here, to run repeatedly:

}
