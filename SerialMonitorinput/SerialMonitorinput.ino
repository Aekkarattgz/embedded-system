void setup() {
  Serial.begin(115200);   // เริ่ม Serial
  Serial.println("กรอกตัวเลขแล้วกด Enter:");
}

void loop() {
  // ถ้ามีข้อมูลจากคีย์บอร์ดเข้ามา
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n'); // อ่านจนจบแถว
    input.trim();                                // เอา space / \r ออก

    // แปลงเป็นตัวเลข (int)
    int value = input.toInt();

    // แสดงผลกลับไปที่คอมพิวเตอร์
    Serial.print("คุณส่งตัวเลข: ");
    Serial.println(value);
  }
}
