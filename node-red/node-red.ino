// -------------------------------
// นำเข้าไลบรารีที่จำเป็น
// -------------------------------
#include "DHT.h"              // ไลบรารีสำหรับเซนเซอร์ DHT22
#include <ArduinoJson.h>      // ไลบรารีสำหรับสร้างและจัดการข้อมูลแบบ JSON

// -------------------------------
// กำหนดขา (Pin Definitions)
// -------------------------------
#define DHTPIN 4              // ขาที่ต่อกับขา Data ของ DHT22
#define DHTTYPE DHT22         // กำหนดชนิดของเซนเซอร์เป็น DHT22

const int ldrPin = 34;        // ขา LDR ต่อกับ ADC1_CH6 (GPIO 34)

// ขาควบคุมรีเลย์
const int relay1Pin = 25;
const int relay2Pin = 26;
const int relay3Pin = 27;
const int relay4Pin = 33;

// -------------------------------
// สร้างอ็อบเจกต์สำหรับเซนเซอร์ DHT
// -------------------------------
DHT dht(DHTPIN, DHTTYPE);

// -------------------------------
// ตัวแปรจับเวลา (Timer) สำหรับส่งข้อมูลเป็นช่วง ๆ
// -------------------------------
unsigned long previousMillis = 0;
const long interval = 2000;  // ส่งข้อมูลทุก ๆ 2 วินาที

// -------------------------------
// ฟังก์ชัน setup() ทำงานครั้งเดียวตอนเริ่มต้น
// -------------------------------
void setup() {
  // เริ่มต้นการสื่อสาร Serial ที่ความเร็ว 115200 baud
  Serial.begin(115200);

  // เริ่มต้นการทำงานของเซนเซอร์ DHT22
  dht.begin();

  // กำหนดโหมดขารีเลย์เป็น OUTPUT
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(relay4Pin, OUTPUT);

  // ตั้งค่าเริ่มต้นให้รีเลย์ปิดทั้งหมด
  // หมายเหตุ: รีเลย์ส่วนใหญ่เป็นแบบ Active-Low
  // คือเขียนค่า HIGH = ปิดรีเลย์, LOW = เปิดรีเลย์
  digitalWrite(relay1Pin, HIGH);
  digitalWrite(relay2Pin, HIGH);
  digitalWrite(relay3Pin, HIGH);
  digitalWrite(relay4Pin, HIGH);
}

// -------------------------------
// ฟังก์ชัน loop() ทำงานวนซ้ำตลอดเวลา
// -------------------------------
void loop() {

  // ------------------------------------------------
  // ส่วนที่ 1: รับคำสั่งจาก Node-RED ผ่าน Serial
  // ------------------------------------------------
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // อ่านคำสั่งจนเจอขึ้นบรรทัดใหม่
    command.trim(); // ตัดช่องว่างหรืออักขระพิเศษหัว–ท้าย

    // ควบคุมรีเลย์ตามคำสั่งที่ได้รับ
    if (command == "R1_ON")  { digitalWrite(relay1Pin, LOW); }
    if (command == "R1_OFF") { digitalWrite(relay1Pin, HIGH); }

    if (command == "R2_ON")  { digitalWrite(relay2Pin, LOW); }
    if (command == "R2_OFF") { digitalWrite(relay2Pin, HIGH); }

    if (command == "R3_ON")  { digitalWrite(relay3Pin, LOW); }
    if (command == "R3_OFF") { digitalWrite(relay3Pin, HIGH); }

    if (command == "R4_ON")  { digitalWrite(relay4Pin, LOW); }
    if (command == "R4_OFF") { digitalWrite(relay4Pin, HIGH); }
  }

  // ------------------------------------------------
  // ส่วนที่ 2: อ่านค่าจากเซนเซอร์และส่งข้อมูลเป็นระยะ
  // ------------------------------------------------
  unsigned long currentMillis = millis();

  // ตรวจสอบว่าครบเวลาที่กำหนดไว้หรือยัง
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // อ่านค่าความชื้น (%)
    float humidity = dht.readHumidity();

    // อ่านค่าอุณหภูมิ (องศาเซลเซียส)
    float temperature = dht.readTemperature();

    // อ่านค่าแสงจาก LDR (ค่า ADC)
    int ldrValue = analogRead(ldrPin);
    int lightPercent = map(ldrValue, 0, 4095, 0, 100); 
    lightPercent = constrain(lightPercent, 0, 100);

    // ตรวจสอบว่าการอ่านค่าจาก DHT ล้มเหลวหรือไม่
    if (isnan(humidity) || isnan(temperature)) {
      // ส่งข้อความแจ้ง Error ในรูปแบบ JSON
      Serial.println("{\"error\":\"Failed to read from DHT sensor!\"}");
      return; // ออกจาก loop รอบนี้
    }

    // -------------------------------
    // สร้าง JSON Object เพื่อส่งข้อมูล
    // -------------------------------
    StaticJsonDocument<200> doc;
    doc["temp"]  = temperature; // อุณหภูมิ
    doc["humid"] = humidity;    // ความชื้น
    doc["ldr"]   = lightPercent;    // ค่าแสงจาก LDR

    // แปลง JSON เป็นข้อความและส่งออกทาง Serial
    serializeJson(doc, Serial);
    Serial.println(); // ขึ้นบรรทัดใหม่เพื่อจบข้อความหนึ่งชุด
  }
}
