#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

// ---------------- LED 7 หลอดธรรมดา ----------------
#define NUM_LEDS 7
int leds[NUM_LEDS] = {32, 33, 25, 26, 27, 12, 13}; 

// ---------------- ปุ่มกด ----------------
#define SW1 36   // ปุ่มโหมดนับขึ้น
#define SW2 39   // ปุ่มโหมดนับลง
#define SW3 34   // ปุ่มกระพริบ

// ---------------- 7-Segment ธรรมดา ----------------
int segPins[7] = {4, 16, 17, 5, 18, 19, 21};

byte numberPatterns[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {1,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,1,0,1,1}  // 9
};

// ---------------- HT16K33 ----------------
Adafruit_7segment matrix = Adafruit_7segment();

// ---------------- ตัวแปร ----------------
int counter = 0;                 
bool countUp = false;            
bool countDown = false;          
bool blinkMode = false;          
unsigned long lastUpdate = 0;    
unsigned long lastBlink = 0;     

// ---------------- ฟังก์ชัน ----------------
void showDigit7Seg(int num) {
  for (int i=0; i<7; i++) {
    digitalWrite(segPins[i], numberPatterns[num][i]);
  }
}
void showLEDs(int num) {
  for (int i=0; i<7; i++) {
    digitalWrite(leds[i], numberPatterns[num][i]);
  }
}

// ---------------- setup ----------------
void setup() {
  for (int i=0; i<NUM_LEDS; i++) pinMode(leds[i], OUTPUT);
  for (int i=0; i<7; i++) pinMode(segPins[i], OUTPUT);

  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);

  Wire.begin(22, 23);   
  matrix.begin(0x70);   

  counter = 0;
  matrix.print(counter);
  matrix.writeDisplay();

  showDigit7Seg(0);
  showLEDs(0);
}

// ---------------- loop ----------------
void loop() {
  unsigned long now = millis();

  // ปุ่มกดเลือกโหมด
  if (digitalRead(SW1) == HIGH) { 
    countUp = true;
    countDown = false;
    blinkMode = false;
    delay(200);
  }
  if (digitalRead(SW2) == HIGH) { 
    countDown = true;
    countUp = false;
    blinkMode = false;
    delay(200);
  }
  if (digitalRead(SW3) == HIGH) { 
    blinkMode = !blinkMode;
    delay(200);
  }

  // โหมดกระพริบ
  if (blinkMode) {
    if (now - lastBlink > 500) {
      lastBlink = now;
      static bool state = false;
      state = !state;
      if (state) {
        matrix.print(counter);
        matrix.writeDisplay();
      } else {
        matrix.clear();
        matrix.writeDisplay();
      }
    }
    return;
  }

  // โหมดนับต่อเนื่อง (ทุก 1 วินาที)
  if (now - lastUpdate > 100) {
    if (countUp) {
      counter++;
      if (counter > 9999) counter = 0;
    }
    if (countDown) {
      counter--;
      if (counter < 0) counter = 9999;
    }

    matrix.print(counter);
    matrix.writeDisplay();

    // แสดงหลักที่ 3 บน 7-seg ธรรมดา
    int digit3 = (counter / 100) % 10;
    showDigit7Seg(digit3);
    showLEDs(digit3);

    lastUpdate = now;
  }
}
