#define sw1 35
#define sw2 34
#define sw3 39
int currentNum = 0;        
int swstatus = 0; //1 2 3  
int beforsatatus = 0;
              //a, b,  c, d, e, f, g
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
  {0,0,0,0,0,0,0},
};
void showNumber(int num){
  for(int c=0;c<7;c++){
     digitalWrite(segPins[c],number[num][c]);
  }
}
void setup() {
  Serial.begin(115200);
  for(int pin=0;pin<7;pin++){
    pinMode(segPins[pin],OUTPUT);
  }
  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
  pinMode(sw3,INPUT);
}
void loop() {
  if (digitalRead(sw1) == 1) {
    while (digitalRead(sw1) == 1) {
      delay(10);  }
    if(swstatus == 3){
      swstatus = 3;
    }
    else{swstatus = 1;}
  }
  if (digitalRead(sw2) == 1) {
     while (digitalRead(sw2) == 1) {
      delay(10);  }
    if(swstatus == 3){
      swstatus = 3;
    }
    else{swstatus = 2;}
  }
  if(digitalRead(sw3) == 1){
    while (digitalRead(sw3) == 1) {
      delay(10);  }
    if(swstatus != 3){
      beforsatatus = swstatus;
      swstatus = 3;
    }
    else{
      swstatus = beforsatatus;
      beforsatatus = 0;
    }
  }
  if (swstatus == 1) {    
      currentNum++;
      if (currentNum > 9) currentNum = 0;
      delay(500);
      Serial.println("up");
  }
  if (swstatus == 2) {  
    currentNum--;
    if (currentNum < 0) currentNum = 9;
    delay(500);
    Serial.println("Down");
  }
  if (swstatus == 3){
    showNumber(currentNum);
    delay(300);
    for (int i = 0; i < 7; i++) {
          digitalWrite(segPins[i], LOW);
        }
      delay(300);
    Serial.println("paused");
  }
  Serial.print("beforsatatus");
  Serial.println(beforsatatus);
  Serial.print("swstatus");
  Serial.println(swstatus);
  showNumber(currentNum);
}
