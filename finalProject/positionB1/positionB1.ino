#include <HMC5883L.h>
#include <SoftwareSerial.h>

HMC5883L compass; //陀螺儀內定A4A5
int previousDegree;
int returnDegree;
SoftwareSerial mySerial(6, 5); 

void setup() {
  Serial.begin(115200);
  delay(10);
  while (!compass.begin())
  {
    Serial.println("Hi");
    delay(500);
  }
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
  
}


void loop() {

  
  mySerialFunction(detectDegree()); 
}

void mySerialFunction(char DegreeReceive) {
   
  while (mySerial.available()) {

    mySerial.println(DegreeReceive);
    Serial.println();
     
    // add it to the inputString:
    
  }
}

char detectDegree() {

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
  return smoothHeadingDegrees;

  delay(50);

  }
