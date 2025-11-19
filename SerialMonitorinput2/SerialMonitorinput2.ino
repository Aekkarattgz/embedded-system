void setup() {
  Serial.begin(115200);
  Serial.println("กรอกตัวเลข (0-9999) แล้วกด Enter:");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    int num = input.toInt();

    // ---- แยกหลัก ----
    int thousands = (num / 1000);   // หลักพัน
    int hundreds  = (num / 100) % 10;    // หลักร้อย
    int tens      = (num / 10) % 10;     // หลักสิบ
    int ones      = num % 10;            // หลักหน่วย

    // ---- แสดงผล ----
    Serial.println("==== ผลการแยกหลัก ====");
    Serial.print("เลขที่รับเข้า: ");
    Serial.println(num);

    Serial.print("หลักพัน : "); Serial.println(thousands);
    Serial.print("หลักร้อย: "); Serial.println(hundreds);
    Serial.print("หลักสิบ : "); Serial.println(tens);
    Serial.print("หลักหน่วย: "); Serial.println(ones);
    Serial.println("=====================");
  }
}
