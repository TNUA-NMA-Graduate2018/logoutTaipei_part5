#include <SPI.h>
#include "RF24.h"

RF24 rf24(7, 8); // CE腳, CSN腳

const byte addr[] = "1Node";
char msgTemp[32];
char msg[32];

int now = 0;
int set = 0;

void setup() {
  Serial.begin(9600);
  rf24.begin();
  rf24.setChannel(83);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
}

void loop() {
  set = now;
  int flag = 0;

  while (set > 0) {
    //Serial.print("setInLoop:");
    //Serial.println(set);

    msgTemp[flag] = char(set % 10) + '0';
    set = set / 10;
    flag++;
  }

  for (int s = 0; s < flag; s++) {
    msg[s] = msgTemp[flag - s - 1];
    Serial.print(msg[s]);
  }
    Serial.println();
//  Serial.print("now:");
//  Serial.print(now);
//  Serial.print("    set:");
//  Serial.print(set);
//  Serial.print("    flag:");
//  Serial.println(flag);
  now++;

  rf24.write(&msg, sizeof(msg));  // 傳送資料
  delay(500);
}
