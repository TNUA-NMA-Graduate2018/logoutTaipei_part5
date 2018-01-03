#include <SPI.h>
#include "RF24.h"

RF24 rf24(7, 8); // CE腳, CSN腳

const byte addr[] = "1Node";
const int readSlider1 = A0;
const int readSlider2 = A1;

int ToOtherRight = 0;
int ToOtherLeft = 0;

void setup() {
  Serial.begin(115200);
  rf24.begin();
  rf24.setChannel(83);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式

  pinMode(readSlider1, INPUT);
  pinMode(readSlider2, INPUT);

  rf24.begin();
  rf24.setChannel(83);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
}

void loop() {
  ToOtherRight = slider(readSlider1);
  ToOtherLeft = slider(readSlider2);
  SendClient(ToOtherLeft, ToOtherRight);
}


void SendClient(int sendToOtherL, int sendToOtherR) {
  char msgTempA[16] = "0";
  char msgTempB[16] = "0";
  char msg[16] = "0";
  int set = sendToOtherL;    // FirstData
  int flag = 0, flagB = 0;
  while (set > 0) {
    msgTempA[flag] = char(set % 10) + '0';
    set = set / 10;
    flag++;
  }
  for (int s = 0; s < flag; s++) {
    msg[s] = msgTempA[flag - s - 1];
  }
  msg[flag++] = ' ';

  set = sendToOtherR;    // SecondData
  flagB = 0;
  while (set > 0) {
    msgTempB[flagB] = char(set % 10) + '0';
    set = set / 10;
    flagB++;
  }
  for (int s = 0; s < flagB; s++) {
    msg[s + flag] = msgTempB[flagB - s - 1];
  }

  msg[flag + flagB] = ';';
  Serial.println(msg);
  //Serial.println(output);
  rf24.write(&msg, sizeof(msg));  // 傳送資料
  delay(500);
}


int slider(int slider) {
  int sli = analogRead(slider);
  int value = int(map(sli, 0, 1024, 0, 255));
  Serial.println(value);
  delay(100);
  return value ;
}
