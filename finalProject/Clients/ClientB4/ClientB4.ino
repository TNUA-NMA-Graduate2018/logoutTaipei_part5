#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

int pixelNumber = 120;//記得改成正確的
int mid = 60;//記得改成正確的

RF24 rf24(9, 10); // CE腳, CSN腳
const byte addr[] = "1Node";
byte pipe = 1;  // 指定通道編號
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNumber, 2, NEO_GRB + NEO_KHZ800);

boolean mode = 1;
boolean modeChange = 1; //0=遙控 1=互控
//const int modeChanging = 3;
int countNoSignal = 0;

const int Rout1 = 5;
const int Rout2 = 6;
const int Lout1 = 7;
const int Lout2 = 8;

const int Ledstrip = 2;
const int LedR = 3;
const int LedL = 4;

int fromOtherR = 1;
int fromOtherL = 1;

void setup() {
  Serial.begin(115200);
  strip.begin();

  pinMode(Rout1, OUTPUT);
  pinMode(Rout2, OUTPUT);
  pinMode(Lout1, OUTPUT);
  pinMode(Lout2, OUTPUT);
  //pinMode(8, OUTPUT);
  //pinMode(7, OUTPUT);

  pinMode(Ledstrip, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedL, OUTPUT);

  rf24.begin();
  rf24.setChannel(84);  // 設定頻道編號
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_1MBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");
}
void loop() {
  connectCheck();
  controlByOther(fromOtherL, fromOtherR);
  breath(fromOtherL, fromOtherR);
  delay(50);
}

void controlByOther(int FromOtherL, int FromOtherR) {//0後退 1不動 2前進
  if (countNoSignal > 3000) {
    motorstop(Rout1, Rout2);
    motorstop(Lout1, Lout2);
    Serial.println("Stop");
  }
  else {
    if (FromOtherR == 2) {
      //digitalWrite(7, LOW);
      forward(abs(FromOtherR), Rout1, Rout2);
      digitalWrite(LedR, HIGH);
    }
    else if (FromOtherR == 0) {
      //digitalWrite(7, LOW);
      backward(abs(FromOtherR), Rout1, Rout2);
      digitalWrite(LedR, HIGH);
    }
    else {
      //digitalWrite(7, HIGH);
      motorstop(Rout1, Rout2);
      digitalWrite(LedR, LOW);
    }

    if (FromOtherL == 2) {
      //digitalWrite(8, LOW);
      forward(abs(FromOtherL), Lout1, Lout2);
      digitalWrite(LedL, HIGH);
    }
    else if (FromOtherL == 0) {
      //digitalWrite(8, LOW);
      backward(abs(FromOtherL), Lout1, Lout2);
      digitalWrite(LedL, HIGH);
    }
    else {
      //digitalWrite(8, HIGH);
      motorstop(Lout1, Lout2);
      digitalWrite(LedL, LOW);
    }
  }
}
void motorstop(const int x, const int y)
{
  digitalWrite(x, LOW);
  digitalWrite(y, LOW);
}
void forward(int gospeed, const int x, const int y)
{
  digitalWrite(x, gospeed);
  digitalWrite(y, 0);
}
void backward(int backspeed, const int x, const int y)
{
  digitalWrite(x, 0);
  digitalWrite(y, backspeed);
}
void breath(int l, int r) {
  int i, j, a;
  for (i = 0; i < mid; i += 10) {
    for (j = 0; j < 10; j++) {
      strip.setPixelColor(i + j, strip.Color(60 - l * 15, 10 + l * 40, 10 + l * 35 + j * 4));
      strip.show();
    }
  }
  for (i = mid; i < strip.numPixels(); i += 10) {
    for (j = 0; j < 10; j++) {
      strip.setPixelColor(i + j, strip.Color(60 - r * 15, 10 + r * 40, 10 + r * 35 + j * 4));
      strip.show();
    }
  }
}
void connectCheck() {
  if (rf24.available(&pipe)) {
    countNoSignal = 0;
    char mg[16] = "";
    rf24.read(&mg, sizeof(mg));
    Serial.println(mg);
    //轉換遙控互動 0遙控 1互控
    if (mg[0] == 'A') {
      mode = 0;
    }
    else if (mg[0] == 'B') {
      mode = 1;
    }
    //偵測方向 0後退 1停止 2前進
    if (mode == 0) {
      if (mg[0] == 'R') {
        fromOtherL = mg[1] - '0';
        fromOtherR = mg[2] - '0';
      }
    }
    else if (mode == 1) {
      if (mg[1] == 'I') {
        fromOtherL = mg[1] - '0';
        fromOtherR = mg[2] - '0';
      }
    }
  }
  else {
    countNoSignal++;
  }
}
void connectCheckBackUp() {
  if (rf24.available(&pipe)) {
    int countNoSignal = 0;
    char mg[16] = "";
    rf24.read(&mg, sizeof(mg));
    Serial.println(mg);
    if (modeChange) {
      fromOtherL = 0;
      fromOtherR = 0;
      int now = 0;
      int s = 0;
      if (mg[s] == 'B' || mg[s] == 'A') {
        if (mg[s] == 'B') {
          modeChange = 1;//互控
        }
        else if (mg[s] == 'A') {
          modeChange = 0;//自控
        }
      }
      else if (mg[0] >= '0' && mg[0] <= '9') {
        do {
          if (mg[s] == ' ')now = 1;
          else if (now == 0 && mg[s] >= '0' &&  mg[s] <= '9') {
            fromOtherL *= 10;
            fromOtherL += mg[s] - '0';
          } else if (now == 1 && mg[s] >= '0' &&  mg[s] <= '9') {
            fromOtherR *= 10;
            fromOtherR += mg[s] - '0';
          }
          s++;
        } while (mg[s] != ';');
      }
    }
    else {
      if (mg[0] == 'B' || mg[0] == 'A') {
        if (mg[0] == 'B') {
          modeChange = 1;
        }
        else if (mg[0] == 'A') {
          modeChange = 0;
        }
      }
    }
  }
  else {
    countNoSignal++;
  }
}
