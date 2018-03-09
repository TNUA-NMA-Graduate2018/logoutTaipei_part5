#include <SPI.h>
#include "RF24.h"

//0遙控 1互動

RF24 rf24(9, 10); // CE腳, CSN腳

const byte addr[] = "1Node";
byte pipe = 1;  // 指定通道編號
const int readLForwardButton = 4;
const int readLBackwardButton = 5;
const int readRForwardButton = 6;
const int readRBackwardButton = 7;

const int readLeft = A5;
const int readRight = A4;

int ToOtherRight = 1;
int ToOtherLeft = 1;

void setup() {
  Serial.begin(115200);
  rf24.begin();
  rf24.setChannel(84);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MAX);   // 設定廣播功率
  rf24.setDataRate(RF24_1MBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式

  pinMode(readLForwardButton, INPUT);
  pinMode(readLBackwardButton, INPUT);
  pinMode(readRForwardButton, INPUT);
  pinMode(readRBackwardButton, INPUT);
  pinMode(readLeft, INPUT);
  pinMode(readRight, INPUT);
}

void loop() {
  ToOtherLeft = fixed(readLeft);
  ToOtherRight = fixed(readRight);
  //0後退 1不動 2前進
  //ToOtherLeft = directionDetect(readLForwardButton, readLBackwardButton);
  //ToOtherRight =  directionDetect(readRForwardButton, readRBackwardButton);
  Serial.print("Left:\t");
  Serial.print(ToOtherLeft);
  Serial.print("\tRight:\t");
  Serial.println(ToOtherRight);
  Send(ToOtherLeft, ToOtherRight);
}
int fixed(int pin) {
  int data = int(map(analogRead(pin), 0, 1024, 0, 255));
  if (data<80){
    return 0;
  }else if(data >= 80 && data <165){
    return 1;
  }
  else if(data >=165){
    return 2;
  }
}
void Send(int toLeft, int toRight) {
  char msg[16] = "0";
  msg[0] = 'I'; //互動
  msg[1] = char(toLeft % 10) + '0';
  msg[2] = char(toRight % 10) + '0';
  msg[3] = ';';
  rf24.write(&msg, sizeof(msg));
  delay(100);
}
void SendClient(int sendToOtherL, int sendToOtherR) {
  char msgTempA[16] = "0";
  char msgTempB[16] = "0";
  char msg[16] = "0";
  int set = sendToOtherL;    // FirstData
  int flag = 0, flagB = 0;
  if (set == 0)msgTempA[flag++] = '0';
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
  if (set == 0)msgTempA[flagB++] = '0';
  while (set > 0) {
    msgTempB[flagB] = char(set % 10) + '0';
    set = set / 10;
    flagB++;
  }
  for (int s = 0; s < flagB; s++) {
    msg[s + flag] = msgTempB[flagB - s - 1];
  }
  msg[flag + flagB] = ';';
  //Serial.println(output);
  //Serial.println(msg);
  rf24.write(&msg, sizeof(msg));  // 傳送資料
  delay(100);
}
int directionDetect(int F, int B) {

  int direction = 0;    //0後退 1不動 2前進
  int forward = digitalRead(F);
  int backward = digitalRead(B);

  if (forward == 0 && backward == 0) {
    direction = 1;
    Serial.println("不動");
  }
  else if (forward == 1) {
    direction = 2;
    Serial.println("前進");
  }
  else if (backward == 1) {
    direction = 0;
    Serial.println("退後");
  }
  return direction;
}
