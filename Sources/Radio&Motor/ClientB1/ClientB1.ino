#include <SPI.h>
#include "RF24.h"
#include <Wire.h>
//#include <HMC5883L.h>

//HMC5883L compass; //陀螺儀內定A4A5
//int previousDegree;
//int returnDegree;

RF24 rf24(9, 10); // CE腳, CSN腳
const byte addr[] = "1Node";
byte pipe = 1;  // 指定通道編號

boolean modeChange = 1; //0 =selfs  1=other
//const int modeChanging = 3;
int countNoSignal = 0;

const int Rout1 = 5;
const int Rout2 = 6;
const int Lout1 = 7;
const int Lout2 = 8;

//const int LedM = 2;
const int LedR = 3;
const int LedL = 4;

const int resR = A1;
const int resL = A0;
int ToSelfRight = 140;
int ToSelfLeft = 140;

const int testHigh = 165;
const int testLow = 80;

int FromOtherR = 140;
int FromOtherL = 140;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(Rout1, OUTPUT);
  pinMode(Rout2, OUTPUT);
  pinMode(Lout1, OUTPUT);
  pinMode(Lout2, OUTPUT);
  //pinMode(8, OUTPUT);
  //pinMode(7, OUTPUT);

  //  pinMode(LedM, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedL, OUTPUT);

  pinMode(resR, INPUT);
  pinMode(resL, INPUT);
  //  pinMode(modeChanging, INPUT);
  rf24.begin();
  rf24.setChannel(83);  // 設定頻道編號
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_1MBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");

  /*while (!compass.begin())
  {
    Serial.println("Hi");
    delay(500);
  }*/
  /*
    // Set measurement range
    compass.setRange(HMC5883L_RANGE_1_3GA);
    //
    // Set measurement mode
    compass.setMeasurementMode(HMC5883L_CONTINOUS);

    // Set data rate
    compass.setDataRate(HMC5883L_DATARATE_30HZ);

    // Set number of samples averaged
    compass.setSamples(HMC5883L_SAMPLES_8);

    // Set calibration offset. See HMC5883L_calibration.ino
    compass.setOffset(0, 0);
  */
}


void loop() {
  if (!modeChange) {
    ConnectCheck();
    //ToSelfRight = slider(resR);
    //ToSelfLeft = map(slider(resL), 0, 255, 255, 0);
    //Serial.print("L :");
    //Serial.print(ToSelfLeft);
    //Serial.print("\tR :");
    //Serial.println(ToSelfRight);
    //sliderControlSelf(ToSelfRight, ToSelfLeft);
    Serial.println("Emily says stop");
  }
  else {
    ConnectCheck();
    sliderControlByOther(FromOtherR, FromOtherL);
  }
  delay(50);
  //detectDegree();
}


void sliderControlByOther(int FromOtherR, int FromOtherL) {
  if (countNoSignal > 3000) {
    motorstop(Rout1, Rout2);
    motorstop(Lout1, Lout2);
    Serial.println("Stop");
  }
  else {
    if (FromOtherR > testHigh && FromOtherR <= 225) {
      //digitalWrite(7, LOW);
      forward(abs(FromOtherR), Rout1, Rout2);
      digitalWrite(LedR, HIGH);
    }
    else if (FromOtherR < testLow && FromOtherR >= 1) {
      //digitalWrite(7, LOW);
      backward(abs(FromOtherR), Rout1, Rout2);
      digitalWrite(LedR, HIGH);
    }
    else {
      //digitalWrite(7, HIGH);
      motorstop(Rout1, Rout2);
      digitalWrite(LedR, LOW);
    }

    if (FromOtherL > testHigh && FromOtherL <= 225) {
      //digitalWrite(8, LOW);
      forward(abs(FromOtherL), Lout1, Lout2);
      digitalWrite(LedL, HIGH);
    }
    else if (FromOtherL < testLow && FromOtherL >= 1) {
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
/*

void sliderControlSelf(int resValueR, int resValueL) {

  if (FromOtherR > testHigh && FromOtherR <= 225) {
    //digitalWrite(7, LOW);
    forward(abs(FromOtherR), Rout1, Rout2);
    digitalWrite(LedR, HIGH);
  }
  else if (FromOtherR < testLow && FromOtherR >= 1) {
    //digitalWrite(7, LOW);
    backward(abs(FromOtherR), Rout1, Rout2);
    digitalWrite(LedR, HIGH);
  }
  else {
    //digitalWrite(7, HIGH);
    motorstop(Rout1, Rout2);
    digitalWrite(LedR, LOW);
  }

  if (FromOtherL > testHigh && FromOtherL <= 225) {
    //digitalWrite(8, LOW);
    forward(abs(FromOtherL), Lout1, Lout2);
    digitalWrite(LedL, HIGH);
  }
  else if (FromOtherL < testLow && FromOtherL >= 1) {
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
*/

int slider(int sliderInput) {
  int sli = analogRead(sliderInput);
  int value = int(map(sli, 0, 1024, 0, 255));
  return value ;
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


void ConnectCheck() {
  if (rf24.available(&pipe)) {
    int countNoSignal = 0;
    char mg[16] = "";
    rf24.read(&mg, sizeof(mg));
    Serial.println(mg);
    if (modeChange) {
      FromOtherL = 0;
      FromOtherR = 0;
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


/*void detectDegree() {

  long x = micros();
  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);


  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180 / M_PI;

  // Fix HMC5883L issue with angles
  float fixedHeadingDegrees;

  if (headingDegrees >= 1 && headingDegrees < 240)
  {
    fixedHeadingDegrees = map(headingDegrees, 0, 239, 0, 179);
  } else if (headingDegrees >= 240)
  {
    fixedHeadingDegrees = map(headingDegrees, 240, 360, 180, 360);
  }

  // Smooth angles rotation for +/- 3deg
  int smoothHeadingDegrees = round(fixedHeadingDegrees);

  if (smoothHeadingDegrees < (previousDegree + 3) && smoothHeadingDegrees > (previousDegree - 3))
  {
    smoothHeadingDegrees = previousDegree;
  }

  previousDegree = smoothHeadingDegrees;

  char writeDegrees = char(map(smoothHeadingDegrees, 0, 360, 0, 255));
  //Serial.print(writeDegrees);
  //return smoothHeadingDegrees;

  delay(50);

  }*/
