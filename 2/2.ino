#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
Adafruit_7segment matrix = Adafruit_7segment();
int currentNum = 0;
int swstatus = 0;
#define sw1 35
#define sw2 34
#define sw3 39
int segPins[7]={32,33,25,26,27,12,13};
byte number[11][7]={
// a,b,c,d,e,f,g
  {1,1,1,1,1,1,0}, //0
  {0,1,1,0,0,0,0}, //1
  {1,1,0,1,1,0,1}, //2
  {1,1,1,1,0,0,1}, //3
  {0,1,1,0,0,1,1}, //4
  {1,0,1,1,0,1,1}, //5
  {1,0,1,1,1,1,1}, //6
  {1,1,1,0,0,0,0}, //7
  {1,1,1,1,1,1,1}, //8
  {1,1,1,1,0,1,1}, //9
  {0,0,0,0,0,0,0}, //ดับ
};
int t = 0;
int h = 0;
int te = 0;
int u = 0;
void showNumber(int num){
  for(int c=0;c<7;c++){
     digitalWrite(segPins[c],number[num][c]);
  }
}
void showNumberI2C(int num) {
  matrix.clear();
  matrix.print(num);     // แสดงตัวเลข (0–9999)
  matrix.writeDisplay();
}
void setup() {
  Serial.begin(115200);
  Wire.begin(22, 23);   // กำหนด SDA=22, SCL=23
  matrix.begin(0x70);   // ที่อยู่ I2C ปกติคือ 0x70
  showNumberI2C(0);
  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
  pinMode(sw3,INPUT);
  //Serial.begin(115200);
  for(int pin=0;pin<7;pin++){
    pinMode(segPins[pin],OUTPUT);
  }
  Serial.println("กรอกตัวเลขที่ต้องการ 0 - 9999 : ");
}
void loop() {
  if(Serial.available()>0){  
    String input = Serial.readStringUntil('\n');
    input.trim();
    int value = input.toInt();
    t = (value/1000); //8765/1000=8
    h = (value/100)%10;  //8765/100=87%10 =7
    te = (value/10)%10;  //8765/100=876%10 =6
    u = value%10;  //8765%10 =5
    showNumberI2C(value);
    Serial.println("กดสวิตซ์ที่ 1 แสดงค่าหลักร้อย");
    Serial.println("กดสวิตซ์ที่ 2 แสดงค่าหลักสิบ");
    Serial.println("กดสวิตซ์ที่ 3 แสดงค่าหลักหน่วย");
    Serial.println("ถ้าไม่กดสวิตซ์ที่ใดๆแสดงหลักพัน");
    Serial.println("กรอกตัวเลขที่ต้องการ 0 - 9999 : ");
    } 
    if (digitalRead(sw1) == 1) {
      showNumber(h);
    }
    else if (digitalRead(sw2) == 1) {    
      showNumber(te);
    }
    else if (digitalRead(sw3) == 1) {      
      showNumber(u);
    }
    else{showNumber(t);}  
}

