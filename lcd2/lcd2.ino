#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- LCD ----------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- Switch ----------
#define SW1 32
#define SW2 33
#define SW3 25

// ---------- Relay ----------
#define RELAY1 26
#define RELAY2 27
#define RELAY3 14

void setup() {
  // ตั้งค่า I2C
  Wire.begin(22, 23);
  lcd.begin(16, 2);
  lcd.backlight();

  // สวิตช์
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);

  // รีเลย์
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  // ปิดรีเลย์ทั้งหมดก่อน (Active LOW)
  digitalWrite(RELAY1, 1);
  digitalWrite(RELAY2, 1);
  digitalWrite(RELAY3, 1);

  lcd.setCursor(0, 0);
  lcd.print("Relay Control");
  lcd.setCursor(0, 1);
  lcd.print("Press Switch");
}

void loop() {
  int sw1 = digitalRead(SW1);
  int sw2 = digitalRead(SW2);
  int sw3 = digitalRead(SW3);

  // ---------- Relay Control ----------
  if (sw1 == 1) {
    digitalWrite(RELAY1, 0);   // ON
  } else {
    digitalWrite(RELAY1, 1);  // OFF
  }

  if (sw2 == 1) {
    digitalWrite(RELAY2, 0);
  } else {
    digitalWrite(RELAY2, 1);
  }

  if (sw3 == 1) {
    digitalWrite(RELAY3, 0);
  } else {
    digitalWrite(RELAY3, 1);
  }

  // ---------- LCD Display ----------
  lcd.setCursor(0, 0);
  lcd.print("R1:");
  lcd.print(sw1 == 1 ? "ON " : "OFF");
  /*
    if (sw1 == 1) {
      lcd.print("ON ");
    } 
    else {
    lcd.print("OFF");
    }
  */
  lcd.print(" R2:");
  lcd.print(sw2 == 1 ? "ON " : "OFF");

  lcd.setCursor(0, 1);
  lcd.print("R3:");
  lcd.print(sw3 == 1 ? "ON " : "OFF");

  delay(200);
}
