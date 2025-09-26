#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <DHT.h>

// --- LCD I2C (Hardware I2C: SDA = GPIO 21, SCL = GPIO 22)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// --- HT16K33 (Software I2C: SDA = GPIO 1, SCL = GPIO 3)
TwoWire myWire(1);
Adafruit_7segment segDisp = Adafruit_7segment();

// --- DHT22
#define DHTPIN 18
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- LDR
#define LDR_PIN 34

// --- Relays
#define RELAY1 25
#define RELAY2 26
#define RELAY3 27
#define RELAY4 33

// --- Buttons
#define SW1 13
#define SW2 14
#define SW3 15

// --- 7-segment (ธรรมดา)
const int segPins[8] = {4, 5, 12, 16, 17, 19, 23, 32}; // A-G, DP

// --- Segment code for digits 0-9
const byte segCode[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

void setup() {
  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();

  myWire.begin(1, 3); // SDA=1, SCL=3 software I2C
  segDisp.begin(0x70, &myWire);

  dht.begin();

  pinMode(LDR_PIN, INPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(segPins[i], OUTPUT);
    digitalWrite(segPins[i], LOW);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int ldrVal = analogRead(LDR_PIN);
  int lightPercent = map(ldrVal, 4095, 0, 0, 100); // Light ↑ = ADC ↓

  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Hum: ");
  if (isnan(hum)) lcd.print("Err ");
  else lcd.print((int)hum);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.print(lightPercent);
  lcd.print("%   ");

  // Button and Relay Logic
  int buttonRelay = 0;

  if (digitalRead(SW3) == LOW) {
    digitalWrite(RELAY3, HIGH);
    buttonRelay = 3;
  } else {
    digitalWrite(RELAY3, LOW);
  }

  if (digitalRead(SW2) == LOW && buttonRelay == 0) {
    digitalWrite(RELAY2, HIGH);
    buttonRelay = 2;
  } else {
    digitalWrite(RELAY2, LOW);
  }

  if (digitalRead(SW1) == LOW && buttonRelay == 0) {
    digitalWrite(RELAY1, HIGH);
    buttonRelay = 1;
  } else {
    digitalWrite(RELAY1, LOW);
  }

  // Relay4 light control
  if (lightPercent < 30) {
    digitalWrite(RELAY4, HIGH);
  } else {
    digitalWrite(RELAY4, LOW);
  }

  // 7-segment (ธรรมดา)
  showNumber7Seg(buttonRelay);

  // HT16K33
  if (isnan(temp)) temp = 0;
  int tempInt = (int)temp;
  int displayValue = tempInt * 100 + lightPercent; // 2 หลัก temp + 2 หลัก light
  segDisp.print(displayValue);
  segDisp.writeDisplay();

  delay(300);
}

void showNumber7Seg(int num) {
  if (num < 0 || num > 9) num = 0;
  for (int i = 0; i < 8; i++) {
    digitalWrite(segPins[i], segCode[num][i]);
  }
}


