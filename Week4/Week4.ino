// ---------------- LED 7 หลอดธรรมดา ----------------
#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13}; // LED 7 หลอดต่อกับ GPIO

// ---------------- ปุ่มกด ----------------
#define SW1 36
#define SW2 39
#define SW3 34

// ---------------- 7-Segment Display ----------------
// a, b, c, d, e, f, g
int segPins[7] = {4, 16, 17, 5, 18, 19, 21};

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
void showNumberLEDs(int num) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(leds[i], numberPatterns[num][i]);
  }
}

// ---------------- ตัวแปรควบคุม ----------------
int currentNum = 0;        // เลขที่แสดงอยู่
bool countingUp = false;   // โหมดนับขึ้น
bool countingDown = false; // โหมดนับลง
bool paused = false;       // หยุด/เล่น
unsigned long lastUpdate = 0; // เวลาอัปเดตล่าสุด

void setup() {
  Serial.begin(115200);  
  // ตั้งค่า LED 7 หลอดเป็น OUTPUT
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
  }
  // ตั้งค่า 7-Segment เป็น OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }

  // ตั้งค่าปุ่มกดเป็น INPUT
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);

  // เริ่มต้นแสดงเลข 0
  showNumber(currentNum);
  showNumberLEDs(currentNum);
}

void loop() {
  // อ่านค่าปุ่ม (0 = กด, 1 = ไม่กด)
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
    delay(200); // กันเด้ง
  }
  if (digitalRead(SW3) == 1) {
    paused = !paused; // toggle หยุด/เล่น
    delay(200); // กันเด้ง
  }

  // ถ้าหยุด → กระพริบเลข
  if (paused) {
    static bool blinkState = false;
    if (millis() - lastUpdate > 500) { // กระพริบทุก 0.5 วิ
      blinkState = !blinkState;
      if (blinkState) {
        showNumber(currentNum);
        showNumberLEDs(currentNum);
      } else {
        // ปิดหมด
        for (int i = 0; i < 7; i++) {
          digitalWrite(segPins[i], LOW);
          digitalWrite(leds[i], LOW);
        }
      }
      lastUpdate = millis();
    }
    return; // ออกไปไม่ต้องนับต่อ
  }

  // ถ้าไม่นิ่ง → อัปเดตเลขทุก 1 วิ
  if (millis() - lastUpdate > 1000) {
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

    lastUpdate = millis();
  }
}
