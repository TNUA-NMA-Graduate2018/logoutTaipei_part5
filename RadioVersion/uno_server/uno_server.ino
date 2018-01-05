#include <SPI.h>
#include "RF24.h"
#include <Servo.h>

#include <Adafruit_NeoPixel.h>

int pixelNumber = 120;
Adafruit_NeoPixel stripA = Adafruit_NeoPixel(pixelNumber, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(pixelNumber, 6, NEO_GRB + NEO_KHZ800);
Servo myservo; 

const int servoPin = 9; // the digital pin used for the servo
float nowDegree = 110;
float easing = 0.01;
float easDegree = 110;
int angle = 0 ;

int value = 1;

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
  pinMode(A2,INPUT)//可變電阻調LED

  rf24.begin();
  rf24.setChannel(83);       // 設定頻道編號
  rf24.openWritingPipe(addr); // 設定通道位址
  rf24.setPALevel(RF24_PA_MIN);   // 設定廣播功率
  rf24.setDataRate(RF24_250KBPS); // 設定傳輸速率
  rf24.stopListening();       // 停止偵聽；設定成發射模式

  stripA.begin();
  stripB.begin();
  
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(nowDegree);
  delay(1000);
}

void loop() {
  ToOtherRight = slider(readSlider1);
  ToOtherLeft = slider(readSlider2);
  SendClient(ToOtherLeft, ToOtherRight);

   switch (value) {

    case 1: LedChangeOhyeah (1); break;
    case 2: LedChangeOhyeah (2); break;
    case 3: LedChangeOhyeah (3); break;

  }
  motor();
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



void   LedChangeOhyeah (int num) {
  if (num == 1) {
    theaterChase(50, 5);//50是亮度 5是等多久(速度)
  }
  else if (num == 2) {
    breath(20, 80);
    stripA.show();
    stripB.show();
  }
  else if (num == 3) {
    rainbowCycle(50);
  }
  if (value != valueold) {
    valueold = value;
    for (int n = 0; n < stripA.numPixels(); n++) {
      stripA.setPixelColor(n, stripA.Color(0, 0, 0));
      stripB.setPixelColor(n, stripB.Color(0, 0, 0));
    }
    stripA.show();
    stripB.show();
  }
}
void theaterChase(int c, uint8_t wait) {
  for (int j = 0; j < 1; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 2; q++) {
      for (int i = 0; i < 120 + 20; i = i + 10) {
        for (int a = 0; a < 10; a++) {
          stripA.setPixelColor(i + q - a, stripA.Color(c - (5 * (a + 1)), c - (5 * (a + 1)), c - (5 * (a + 1))));
          stripA.setPixelColor(i + q - 9 - a, stripA.Color(0, 0, 0));
          
          stripB.setPixelColor(i + q - a, stripB.Color(c - (5 * (a + 1)), c - (5 * (a + 1)), c - (5 * (a + 1))));
          stripB.setPixelColor( i+ q - 9 - a, stripB.Color(0, 0, 0));
          delay(wait);
        }//turn every third pixel on
        stripA.show();
        stripB.show();
      }
    }
  }
}
void breath(int c, uint8_t wait) {
  int i, j;

  for (j = 0; j < c+1; j++) {
    for (i = 0; i < stripA.numPixels(); i++) {
      stripA.setPixelColor(i, stripA.Color(c - j, c - j, c - j));
      stripB.setPixelColor(i, stripB.Color(c - j, c - j, c - j));

    }
    stripA.show();
    stripB.show();
    delay(wait);
  }
  for (j = 0; j < c+1; j++) {
    for (i = 0; i < stripA.numPixels(); i++) {
      stripA.setPixelColor(i, stripA.Color(j, j, j));
      stripB.setPixelColor(i, stripB.Color(j, j, j));
    }
    stripA.show();
    stripB.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 128; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < stripA.numPixels(); i++) {
      stripA.setPixelColor(i, Wheel(((i * 255 / stripA.numPixels()) + j ) & 127));
      stripB.setPixelColor(i, Wheel(((i * 255 / stripB.numPixels()) + j ) & 127));
    }
    stripA.show();
    stripB.show();
    //delay(wait);
  }
}


uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 42) {
    return stripA.Color(WheelPos * 3, 127 - WheelPos * 3, 0);
  } else if (WheelPos < 85) {
    WheelPos -= 42;
    return stripA.Color(127 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 85;
    return stripA.Color(0, WheelPos * 3, 127 - WheelPos * 3);
  }
}


void motor() {
  myservo.write((int)nowDegree);      // move the servo to 180, max speed, wait until done
  angle += 5; //可以控制去程的時間(加的數字越大 轉的時間越短)
  nowDegree = 110 + (20 * sin(radians(angle)));//＊sin前的那個數字 控制轉速
  //nowDegree += (easDegree - nowDegree) * easing;
  delay(300);//控制回程時間，delay越短 轉的時間越短 甚至不回轉 但時間加長去程的時間也會加長

}
