#define LED1 32
#define LED2 33
#define LED3 25
#define LED4 26
#define LED5 27
#define LED6 12 
#define LED7 13
void setup() {
  Serial.begin(115200);  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
}

void loop() {
  digitalWrite(LED1, 1);
  digitalWrite(LED2, 1);
  digitalWrite(LED3, 1);
  digitalWrite(LED4, 1);
  digitalWrite(LED5, 1);
  digitalWrite(LED6, 1);
  digitalWrite(LED7, 1);
  delay(1000);
  digitalWrite(LED1, 0);
  digitalWrite(LED2, 0);
  digitalWrite(LED3, 0);
  digitalWrite(LED4, 0);
  digitalWrite(LED5, 0);
  digitalWrite(LED6, 0);
  digitalWrite(LED7, 0);
  delay(1000);

}
