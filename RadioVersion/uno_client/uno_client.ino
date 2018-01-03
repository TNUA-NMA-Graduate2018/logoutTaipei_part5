#include <SPI.h>
#include "RF24.h"
RF24 rf24(7, 8); // CE腳, CSN腳
const byte addr[] = "1Node";
byte pipe = 1;  // 指定通道編號

boolean modeChange = 1; //0 =selfs  1=other

const int modeChanging = 3;
//Radio Used 7,8,11,12,13
const int RelayR1 = 1;//4
const int RelayR2 = 2;//5
const int RelayR3 = 3;//6
const int RelayR4 = 4;//7

const int RelayL1 = 5;//8
const int RelayL2 = 6;//9
const int RelayL3 = 9;//10
const int RelayL4 = 10;//11

const int resR = A0;
const int resL = A1;

const int testHigh = 100;
const int testLow = 150;

int FromOtherR = 0;
int FromOtherL = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(RelayR1, OUTPUT);
  pinMode(RelayR2, OUTPUT);
  pinMode(RelayR3, OUTPUT);
  pinMode(RelayR4, OUTPUT);
  pinMode(RelayL1, OUTPUT);
  pinMode(RelayL2, OUTPUT);
  pinMode(RelayL3, OUTPUT);
  pinMode(RelayL4, OUTPUT);

  pinMode(resR, INPUT);
  pinMode(resL, INPUT);
  pinMode(modeChanging, INPUT);
  rf24.begin();
  rf24.setChannel(83);  // 設定頻道編號
  rf24.setPALevel(RF24_PA_MIN);
  rf24.setDataRate(RF24_250KBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");
}

void loop() {
  //modeChange = digitalRead(modeChanging);
  if (!modeChange) {
    sliderControlSelf(resR, resL);
  }
  else {
    ConnectCheck();
    sliderControlByOther(FromOtherR, FromOtherL);
  }

}
void ConnectCheck() {
  if (rf24.available(&pipe)) {
    char mg[16] = "";
    rf24.read(&mg, sizeof(mg));

    FromOtherL = 0;
    FromOtherR = 0;
    int now = 0;
    int s = 0;
    do {
      if (mg[s] == ' ')now = 1;
      else if (now == 0 && mg[s] >= '0' &&  mg[s] <= '9') {
        FromOtherL *= 10;
        FromOtherL += mg[s] - '0';
      } else if (now == 1 && mg[s] >= '0' &&  mg[s] <= '9') {
        FromOtherR *= 10;
        FromOtherR += mg[s] - '0';
      }
      s++;
    } while (mg[s] != ';');
  }
}


void sliderControlByOther(int FromOtherR, int FromOtherL) {
  Rgoahead(FromOtherR);
  Rgoback(FromOtherR);
  Rnomove(FromOtherR);

  Lgoahead(FromOtherL);
  Lgoback(FromOtherL);
  Lnomove(FromOtherL);
}


void sliderControlSelf(int resValueR, int resValueL) {
  Rgoahead(resValueR);
  Rgoback(resValueR);
  Rnomove(resValueR);

  Lgoahead(resValueL);
  Lgoback(resValueL);
  Lnomove(resValueL);
}

void Rgoahead(int valuein) {
  if (valuein > testHigh) {
    digitalWrite(RelayR1, LOW);
    digitalWrite(RelayR2, HIGH);
    digitalWrite(RelayR3, HIGH);
    digitalWrite(RelayR4, LOW);
  }
}

void Rgoback(int valuein) {
  if (valuein < testLow) {
    digitalWrite(RelayR1, HIGH);
    digitalWrite(RelayR2, LOW);
    digitalWrite(RelayR3, LOW);
    digitalWrite(RelayR4, HIGH);
  }
}

void Rnomove(int valuein) {
  if (valuein < testHigh && valuein > testLow) {
    digitalWrite(RelayR1, HIGH);
    digitalWrite(RelayR2, HIGH);
    digitalWrite(RelayR3, HIGH);
    digitalWrite(RelayR4, HIGH);
  }
}


void Lgoahead(int valuein) {
  if (valuein > testHigh) {
    digitalWrite(RelayL1, LOW);
    digitalWrite(RelayL2, HIGH);
    digitalWrite(RelayL3, HIGH);
    digitalWrite(RelayL4, LOW);
  }
}

void Lgoback(int valuein) {
  if (valuein < testLow) {
    digitalWrite(RelayL1, HIGH);
    digitalWrite(RelayL2, LOW);
    digitalWrite(RelayL3, LOW);
    digitalWrite(RelayL4, HIGH);
  }
}

void Lnomove(int valuein) {
  if (valuein < testHigh && valuein > testLow) {
    digitalWrite(RelayL1, HIGH);
    digitalWrite(RelayL2, HIGH);
    digitalWrite(RelayL3, HIGH);
    digitalWrite(RelayL4, HIGH);
  }
}
