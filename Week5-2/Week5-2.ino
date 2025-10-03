#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// ---------------- LED 7 หลอดธรรมดา ----------------
#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13}; 

// ---------------- ปุ่มกด ----------------
#define SW1 36   
#define SW2 39   
#define SW3 34   

// ---------------- 7-Segment แบบขาธรรมดา ----------------
int segPins[7] = {4, 16, 17, 5, 18, 19, 21}; 

byte numberPatterns[10][7] = {
  {1,1,1,1,1,1,0}, {0,1,1,0,0,0,0}, {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1}, {0,1,1,0,0,1,1}, {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1}, {1,1,1,0,0,0,0}, {1,1,1,1,1,1,1},
  {1,1,1,1,0,1,1}
};

// ---------------- 7-Segment I2C HT16K33 ----------------
Adafruit_7segment matrix = Adafruit_7segment();

// ---------------- ตัวแปรควบคุม ----------------
int currentNum = 0;
bool countingUp = false;
bool countingDown = false;
bool paused = false;
unsigned long lastUpdate = 0;

// ---------------- ฟังก์ชันแสดงเลข ----------------
void showNumber(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segPins[i], numberPatterns[num][i]);
  }
}
void showNumberLEDs(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], numberPatterns[num][i]);
  }
}
void showNumberI2C(int num) {
  matrix.clear();
  matrix.print(num);
  matrix.writeDisplay();
}

// ---------------- setup ----------------
void setup() {
  for (int i = 0; i < NUM_LEDS; i++) pinMode(leds[i], OUTPUT);
  for (int i = 0; i < 7; i++) pinMode(segPins[i], OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);

  // Serial Monitor
  Serial.begin(115200);
  Serial.println("กรุณาป้อนตัวเลข 0 - 9999:");

  showNumber(currentNum);
  showNumberLEDs(currentNum);

  Wire.begin(22, 23);
  matrix.begin(0x70);
  showNumberI2C(currentNum);
}

// ---------------- loop ----------------
void loop() {
  unsigned long currentMillis = millis();

  // ----------- อ่านค่าจาก Serial Monitor -----------
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // อ่านจนเจอ Enter
    int value = input.toInt(); // แปลงเป็น int
    if (value >= 0 && value <= 9999) {
      currentNum = value;
      Serial.print("รับค่า: ");
      Serial.println(currentNum);
      showNumberI2C(currentNum);
    } else {
      Serial.println("กรุณาป้อนเลข 0 - 9999 เท่านั้น");
    }
  }
    showNumber(currentNum % 10);
    showNumberLEDs(currentNum % 10);
    showNumberI2C(currentNum);
  
}
