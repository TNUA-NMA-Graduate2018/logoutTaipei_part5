#include <SPI.h>
#include "RF24.h"
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>

boolean inputString = 1;
boolean stringComplete = false;
//SoftwareSerial mySerial(6, 5); // RX, TX
Servo myservo;
int Mode = 1; //輪椅模式
int ModeChanged = 1; //輪椅模式確認 1互控
//0自控 1互控

const int servoPin = 8; // the digital pin used for the servo

RF24 rf24(9, 10); // CE腳, CSN腳

const byte addr[] = "1Node";
const int readSlider1 = A0;
const int readSlider2 = A1;

int ToOtherRight = 140;
int ToOtherLeft = 140;

void setup() {
  Serial.begin(115200);
  //mySerial.begin(57600);
  rf24.begin();
  rf24.setChannel(83);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MAX);   // 設定廣播功率
  rf24.setDataRate(RF24_1MBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式
  //
  pinMode(readSlider1, INPUT);
  pinMode(readSlider2, INPUT);

  pinMode(A3, INPUT); //調整輪椅模式
  //
  // myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  // myservo.write(nowDegree);
  delay(1000);

}

void loop() {
  //int readNum = 0;
  //mySerial.print("on");
  //mySerialFunction();

  /*  if (readNum < 512) {//自控
      ModeChanged = 0;
    }
    else if (readNum >= 512) { //互控
      ModeChanged = 1;
    }
  */
  //ModeChanged = inputString;
  //  Serial.println(ModeChanged);
  //  if (Mode == 0 && ModeChanged == 1) {
  //    SendControlChange(ModeChanged);
  //    Mode = 1;
  //  }
  //  if (Mode == 1 && ModeChanged == 0) {
  //    SendControlChange(ModeChanged);
  //    Mode = 0;
  //  }

  if (Mode == 1) {
    ToOtherRight = slider(readSlider1);
    ToOtherLeft =  slider(readSlider2);
    Serial.print("Left:\t");
    Serial.print(ToOtherLeft);
    Serial.print("\tRight:\t");
    Serial.println(ToOtherLeft);
    SendClient(ToOtherLeft, ToOtherRight);
  }
  //  motor();
  delay(50);

  //  detectDegree();
}
/*
void mySerialFunction() {
  //Serial.println("Hi");

  while (mySerial.available()) {

    Serial.println("here");
    inputString = mySerial.read();
    Serial.println(inputString);

    // add it to the inputString:

  }
}
*/
void SendControlChange(int control) {
  char msg[16] = "0";
  if (control == 0) {
    msg[0] = 'A';
  }
  else {
    msg[0] = 'B';
  }
  rf24.write(&msg, sizeof(msg));  // 傳送資料
  //Serial.println(msg);
  delay(50);
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
  if (set == 0)msgTempA[flagB] = '0';
  while (set > 0) {
    msgTempB[flagB] = char(set % 10) + '0';
    set = set / 10;
    flagB++;
  }
  for (int s = 0; s < flagB; s++) {
    msg[s + flag] = msgTempB[flagB - s - 1];
  }

  msg[flag + flagB] = ';';
  //Serial.println(msg);
  //Serial.println(output);
  //Serial.println(msg);
  rf24.write(&msg, sizeof(msg));  // 傳送資料
  delay(50);
}
int slider(int slider) {
  int sli = analogRead(slider);
  int value = int(map(sli, 0, 1024, 0, 255));
  //Serial.println(value);
  //delay(100);
  return value ;
}
/*
void motor() {
/  myservo.write((int)nowDegree);      // move the servo to 180, max speed, wait until done
  angle += 5; //可以控制去程的時間(加的數字越大 轉的時間越短)  nowDegree = 110 + (20 * sin(radians(angle)));//＊sin前的那個數字 控制轉速
  //nowDegree += (easDegree - nowDegree) * easing;
  delay(50);//控制回程時間，delay越短 轉的時間越短 甚至不回轉 但時間加長去程的時間也會加長
}
*/

