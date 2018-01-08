
#include <Wire.h>
#include <HMC5883L.h>
#include <Servo.h>

Servo myservo;
HMC5883L compass;

int previousDegree;
int returnDegree;
const int servoPin = 9; // the digital pin used for the servo
float nowDegree = 110;
float easing = 0.01;
float easDegree = 110;
int angle = 0 ;
int returnMotor = 0;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myservo.write(nowDegree); // set the intial position of the servo, as fast as possible, wait until done
  delay(1000);
  Serial.begin(115200);
  while (!compass.begin())
  {
     delay(500);
  }
  compass.setRange(HMC5883L_RANGE_1_3GA);
  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);
  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);
  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  returnDegree = detectDegree();
//  returnMotor = motor();
//  Serial.print("heading: ");
  //Serial.println(returnDegree);
//  Serial.print(" motor: ");
//  Serial.println(returnMotor);
  delay(50);
}

int motor() {
  myservo.write((int)nowDegree);      // move the servo to 180, max speed, wait until done
  angle += 5; //可以控制去程的時間(加的數字越大 轉的時間越短)
  nowDegree = 110 + (20 * sin(radians(angle)));//＊sin前的那個數字 控制轉速
  //nowDegree += (easDegree - nowDegree) * easing;
  delay(300);//控制回程時間，delay越短 轉的時間越短 甚至不回轉 但時間加長去程的時間也會加長
  
 
  return nowDegree;
}

int detectDegree() {

  //long x = micros();
  Vector norm = compass.readNormalize();

 
  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
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

  delay(30);

 Serial.println(smoothHeadingDegrees);
 
  return smoothHeadingDegrees;
  //char writeDegrees = char(map(smoothHeadingDegrees, 0, 360, 0, 255));
  

}
