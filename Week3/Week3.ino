
// a, b, c, d, e, f, g
int segPins[7] = {4, 16, 17, 5, 18, 19, 21};
// กำหนดตำแหน่งขาของ Segment a–g ที่ต่อกับ ESP32
// a → GPIO 4
// b → GPIO 16
// c → GPIO 17
// d → GPIO 5
// e → GPIO 18
// f → GPIO 19
// g → GPIO 21
byte numberPatterns[10][7] = {
  // a, b, c, d, e, f, g
  {1, 1, 1, 1, 1, 1, 0}, // 0 เปิด a b c d e f
  {0, 1, 1, 0, 0, 0, 0}, // 1 เปิด b c
  {1, 1, 0, 1, 1, 0, 1}, // 2 เปิด a b d e g
  {1, 1, 1, 1, 0, 0, 1}, // 3 เปิด a b c d g
  {0, 1, 1, 0, 0, 1, 1}, // 4 เปิด b c f g
  {1, 0, 1, 1, 0, 1, 1}, // 5 เปิด a c d f g
  {1, 0, 1, 1, 1, 1, 1}, // 6 เปิด a c d e f g
  {1, 1, 1, 0, 0, 0, 0}, // 7 เปิด a b c
  {1, 1, 1, 1, 1, 1, 1}, // 8 เปิดทุก segment
  {1, 1, 1, 1, 0, 1, 1}  // 9 เปิด a b c d f g
  // ตารางรูปแบบการแสดงผลตัวเลข 0–9 แต่ละบรรทัดแทนเลข 1 ตัว
};

// ฟังก์ชันแสดงตัวเลขตามที่ส่งเข้ามา
// num = 0–9
void showNumber(int num) {
  for (int i = 0; i < 7; i++) {
    // เขียนค่า HIGH/LOW ไปที่แต่ละ segment
    digitalWrite(segPins[i], numberPatterns[num][i]);
  }
}

void setup() {
  // ตั้งค่า GPIO ทุกตัวเป็น OUTPUT
  for (int i = 0; i < 7; i++) {
    pinMode(segPins[i], OUTPUT);
  }
}

void loop() {
  // วนแสดงเลข 0–9 ทีละ 1 วินาที
  for (int n = 0; n <= 9; n++) {
    showNumber(n);    // เรียกฟังก์ชันแสดงตัวเลข n
    delay(1000);      // หน่วงเวลา 1 วินาทีก่อนเปลี่ยนเป็นเลขถัดไป
  }
}


