#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

int pixelNumber = 20;//記得改成正確的
int mid = 10;//記得改成正確的

RF24 rf24(9, 10); // CE腳, CSN腳
const byte addr[] = "8Node";
byte pipe = 1;  // 指定通道編號
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNumber, 2, NEO_GRB + NEO_KHZ800);
boolean mode = 1;
boolean modeChange = 1; //0=遙控 1=互控
//const int modeChanging = 3;
int countNoSignal = 0;

const int Lout1 = 5;
const int Lout2 = 6;
const int Rout1 = 7;
const int Rout2 = 8;

const int Ledstrip = 2;
const int LedR = 3;
const int LedL = 4;

int fromOtherR = 1;
int fromOtherL = 1;
int fromOtherRFixed = 1;
int fromOtherLFixed = 1;
void setup() {
  Serial.begin(115200);
  strip.begin();

  pinMode(Rout1, OUTPUT);
  pinMode(Rout2, OUTPUT);
  pinMode(Lout1, OUTPUT);
  pinMode(Lout2, OUTPUT);

  pinMode(Ledstrip, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedL, OUTPUT);

  rf24.begin();
  rf24.setChannel(106);  // 設定頻道編號
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_250KBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");
}
void loop() {
  connectCheck();
  controlByOther(fromOtherL, fromOtherR);
  delay(100);
}
void controlByOther(int FromOtherL, int FromOtherR) {//0後退 1不動 2前進
  if (countNoSignal > 100) {
    motorstop(Rout1, Rout2);
    motorstop(Lout1, Lout2);
    Serial.println("Stop");
  }
  else {
    if (FromOtherR == 0) {
      forward(abs(FromOtherR), Rout1, Rout2);
      digitalWrite(LedR, HIGH);
    }
    else if (FromOtherR == 2) {
      backward(abs(FromOtherR), Rout1, Rout2);
      digitalWrite(LedR, HIGH);
    }
    else {
      motorstop(Rout1, Rout2);
      digitalWrite(LedR, LOW);
    }

    if (FromOtherL == 0) {
      forward(abs(FromOtherL), Lout1, Lout2);
      digitalWrite(LedL, HIGH);
    }
    else if (FromOtherL == 2) {
      backward(abs(FromOtherL), Lout1, Lout2);
      digitalWrite(LedL, HIGH);
    }
    else {
      motorstop(Lout1, Lout2);
      digitalWrite(LedL, LOW);
    }
    if (fromOtherRFixed != fromOtherR || fromOtherLFixed != fromOtherL) {
      breath(fromOtherL, fromOtherR);
    }
  }
  fromOtherRFixed = fromOtherR;
  fromOtherLFixed = fromOtherL;
}
void motorstop(const int x, const int y)
{
  digitalWrite(x, LOW);
  digitalWrite(y, LOW);
}
void forward(int gospeed, const int x, const int y)
{
  digitalWrite(x, 255);
  digitalWrite(y, 0);
}
void backward(int backspeed, const int x, const int y)
{
  digitalWrite(x, 0);
  digitalWrite(y, 255);
}
void breath(int l, int r) {
  int i, j, a;
  for (i = 0; i < mid; i += 10) {
    for (j = 0; j < 10; j++) {
      strip.setPixelColor(i + j, strip.Color(35-l*10 , 5 +j   , 28+ l * 27+3*j ));
    }
  }
  for (i = mid; i < strip.numPixels(); i += 10) {
    for (j = 0; j < 10; j++) {
      strip.setPixelColor(i + j, strip.Color(35-r*15 , 5 + j  , 28 + r * 27+3*j ));
    }
  }
  strip.show();
}
void connectCheck() {
  if (rf24.available(&pipe)) {
    char mg[32] = "";
    rf24.read(&mg, sizeof(mg));
    Serial.println(mg);
    //轉換遙控互動 0遙控 1互控
    if (mg[0] == 'A') {
      mode = 0;//遙控
      Serial.println("遙控");
    }
    else if (mg[0] == 'B') {
      mode = 1;//互控
      Serial.println("互控");
    }
    //偵測方向 2後退 1停止 0前進
    if (mode == 0) {
      if (mg[0] == 'R') {
        countNoSignal = 0;
        fromOtherL = mg[1] - '0';
        fromOtherR = mg[2] - '0';
      }
    }
    else if (mode == 1) {
      if (mg[0] == 'I') {
        countNoSignal = 0;
        fromOtherL = mg[1] - '0';
        fromOtherR = mg[2] - '0';
      } if (mg[0] == 'R') {
        countNoSignal = 0;
      }
    }
  }
  else {
    countNoSignal++;
  }
}
