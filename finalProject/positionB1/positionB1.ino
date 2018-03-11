#include <HMC5883L.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
RF24 rf24(9, 10); // CE腳, CSN腳
const byte addr[] = "1Node";
byte pipe = 1;  // 指定通道編號
HMC5883L compass; //陀螺儀內定A4 SCL     A5 sda
int counter = 0;
int previousDegree;
int returnDegree;
SoftwareSerial mySerial(5, 6);

void setup() {
  Serial.begin(115200);
  mySerial.begin(57600);
  delay(10);

  while (!compass.begin())
  {
    Serial.println("Hi");
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);
  Serial.println("OK01");
  //
  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  Serial.println("OK02");
  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
  Serial.println("OK03");

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);
  Serial.println("OK04");

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
  Serial.println("OK");

  rf24.begin();
  rf24.setChannel(95);  // 設定頻道編號
  rf24.setPALevel(RF24_PA_MIN);
  rf24.setDataRate(RF24_250KBPS);
  rf24.openReadingPipe(pipe, addr);  // 開啟通道和位址
  rf24.startListening();  // 開始監聽無線廣播
  Serial.println("nRF24L01 ready!");
}
void loop() {
  detectDegree();
  delay(100);
  //mySerialFunction(detectDegree());
  if (rf24.available(&pipe)) {
    char msg[32] = "";
    rf24.read(&msg, sizeof(msg));
    Serial.println(msg); // 顯示訊息內容
    if (mg[0] == 'S') {
      turnServo();
    }

  }
}
void turnServo(){


}
void mySerialFunction(float DegreeReceive) {
  mySerial.print(counter++);
  while (mySerial.available()) {
    Serial.println(DegreeReceive);

    mySerial.print(DegreeReceive);
    mySerial.print('\n') ;
    // delay(100);

    // add it to the inputString:
  }
}
void detectDegree() {
  Serial.println("start detect");
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

  float writeDegrees = float(map(smoothHeadingDegrees, 0, 360, 0, 255));
  Serial.println(writeDegrees);
  mySerial.print(writeDegrees);
  mySerial.print('\n');

  //delay(100);

}
