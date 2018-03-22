#include <SPI.h>
#include "RF24.h"

RF24 rf24(9, 10); // CE腳, CSN腳

const byte addr[] = "1Node";
//char msg[] = "Happy Hacking!";

const int readStartButton = 8; //讀取按鈕
int setNum = 0;
void setup() {
  Serial.begin(115200);
  rf24.begin();
  rf24.setChannel(95);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
  pinMode(readStartButton, OUTPUT)
}
void loop() {
  char msg[16] = "0";
  char msgTemp[16] = "0";
  int flag = 0;
  int set = setNum;
  int input = digitalRead(readStartButton);
  if (input) {
    if (input == 1) {
      msg[0] = 'S';
      rf24.write(&msg, sizeof(msg));  // 傳送資料
      Serial.println(msg);
      delay(500);
    }
  }
  //  if (set == 0)flag++;
  //  while (set > 0) {
  //    msgTemp[flag] = char(set % 10) + '0';
  //    set = set / 10;
  //    flag++;
  //  }
  //  for (int s = 0; s < flag; s++) {
  //    msg[s] = msgTemp[flag - s - 1];
  //  }
  //setNum++;

}
