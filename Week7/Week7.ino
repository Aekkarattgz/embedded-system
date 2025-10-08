#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- ใช้ Hardware I2C บน ESP32 ---
// SDA = GPIO 22, SCL = GPIO 23
LiquidCrystal_I2C lcd(0x27, 16, 2); // ที่อยู่ 0x27 หรือ 0x3F แล้วแต่โมดูล

void setup() {
  Serial.begin(115200);

  // เริ่มต้นใช้งาน I2C
  Wire.begin(22, 23);
  lcd.begin(16, 2);
  lcd.backlight();

  // แสดงข้อความต้อนรับ
  lcd.setCursor(0, 0);
  lcd.print("Hello ESP32!");
  lcd.setCursor(0, 1);
  lcd.print("LCD I2C Ready");

  Serial.println("LCD I2C พร้อมทำงาน (SDA=22, SCL=23)");
}

void loop() {
  // แสดงค่าเวลา millis() ที่ LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Running Time:");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
  lcd.print(" sec");

  delay(2000);
}