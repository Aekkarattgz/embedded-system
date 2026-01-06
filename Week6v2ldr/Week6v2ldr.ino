#include "DHT.h"

#define LDR 14
#define DHTPIN 15
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
}

void loop() {
  delay(2000);
  int ldrraw = analogRead(LDR); //แรงดันไฟฟ้าจากขา Analog ที่ต่อกับ LDR
  int lightPercent = map(ldrraw, 0, 4095, 0, 100); 
                   //map(ค่า, ค่าต่ำสุดที่วัดได้, ค่าสูงสุดที่วัดได้, ค่าต่ำสุดหลังแปลง, ค่าสูงสุดหลังแปลง)
  lightPercent = constrain(lightPercent, 0, 100); //จำกัดค่าไม่ให้เกินช่วงที่กำหนด

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F(" C "));
  Serial.print(f);
  Serial.print(F(" F  Heat index: "));
  Serial.print(hic);
  Serial.print(F(" C "));
  Serial.print(hif);
  Serial.println(F(" F")); 
  Serial.println(lightPercent);

}
