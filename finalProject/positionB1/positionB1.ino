#include <HMC5883L.h>
#include <SoftwareSerial.h>

HMC5883L compass; //陀螺儀內定A4 SCL     A5 sda
int counter=0;
int previousDegree;
int returnDegree;
SoftwareSerial mySerial(5,6); 

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
}


void loop() {
  detectDegree();
  delay(500);
  //mySerialFunction(detectDegree());
  
  
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

  int writeDegrees = int(map(smoothHeadingDegrees, 0, 360, 0, 255));
  Serial.println(writeDegrees); 
  mySerial.print(writeDegrees);
  mySerial.print('\n');

  //delay(100);

  }
