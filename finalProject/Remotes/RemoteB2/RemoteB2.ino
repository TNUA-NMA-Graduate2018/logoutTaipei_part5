#include <SPI.h>
#include "RF24.h"
int mode = 0; //輪椅模式
int modeChanged = 0; //輪椅模式確認
//0遙控 1互動
RF24 rf24(9, 10); // CE腳, CSN腳
const byte addr[] = "1Node";
byte pipe = 1;  // 指定通道編號
const int readLForwardButton = 4;
const int readLBackwardButton = 5;
const int readRForwardButton = 6;
const int readRBackwardButton = 7;
const int readModePin = 3;
int ToOtherRight = 1;
int ToOtherLeft = 1;
void setup() {
  Serial.begin(115200);
  rf24.begin();
  rf24.setChannel(80);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_LOW);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
  pinMode(readLForwardButton, INPUT);
  pinMode(readLBackwardButton, INPUT);
  pinMode(readRForwardButton, INPUT);
  pinMode(readRBackwardButton, INPUT);
  pinMode(readModePin, INPUT);
}
void loop() {
  ToOtherLeft = directionDetect(readLForwardButton, readLBackwardButton);
  ToOtherRight =  directionDetect(readRForwardButton, readRBackwardButton);
  readMode();
  //  Serial.print("Left:\t");
  //  Serial.print(ToOtherLeft);
  //  Serial.print("\tRight:\t");
  //  Serial.println(ToOtherRight);
  Send(ToOtherLeft, ToOtherRight);
}
void readMode() {
  //有電1互控 沒電0遙控
  int flag = digitalRead(readModePin);
  if (flag != mode) {
    char msg[16] = "0";
    mode = flag;
    if (mode == 0)msg[0] = 'A'; //遙控
    if (mode == 1)msg[0] = 'B'; //互控
    rf24.write(&msg, sizeof(msg));
    delay(100);
  }
}
void Send(int toLeft, int toRight) {
  char msg[16] = "0";
  msg[0] = 'R';
  msg[1] = char(toLeft % 10) + '0';
  msg[2] = char(toRight % 10) + '0';
  msg[3] = ';';
  Serial.println(msg);
  rf24.write(&msg, sizeof(msg));
  delay(100);
}
int directionDetect(int F, int B) {
  int direction = 0;    //0前進 1不動 2後退
  int forward = digitalRead(F);
  int backward = digitalRead(B);
  if (forward == 0 && backward == 0) {
    direction = 1;
    // Serial.println("不動");
  }
  else if (forward == 1) {
    direction = 0;
    // Serial.println("前進");
  }
  else if (backward == 1) {
    direction = 2;
    // Serial.println("退後");
  }
  return direction;
}
