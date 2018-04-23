#include <Adafruit_NeoPixel.h>

int pixelNumber = 20;//記得改成正確的
const int LedA = 4;
const int LedB = 6;

Adafruit_NeoPixel stripA = Adafruit_NeoPixel(pixelNumber, LedA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(pixelNumber, LedB, NEO_GRB + NEO_KHZ800);

int flag = 0;

void setup() {
  Serial.begin(115200);
  stripA.begin();
  stripB.begin();
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
}
void loop() {
  breath();
}

void breath() {
  int i, j;
  flag = 0;
  for (i = 0; i <= 360; i++) {
    flag ++;

    for (j = 0; j < 20; j++) {
      //int c = int(12 * abs(sin(radians(i))));
      int c = 10;
      stripA.setPixelColor((flag + j) % 20, stripA.Color(int(map(j, 0, 20, 0, c*3 )), c , c * 2));
      stripB.setPixelColor((flag + j) % 20, stripB.Color(int(map(j, 0, 20, 0, c*3 )), c , c * 2));
      //stripA.setPixelColor((flag + j) % 20, stripA.Color(int(map(j, 0, 20, 0, c / 2)), c / 2, c * 2));
      //stripB.setPixelColor((flag + j) % 20, stripB.Color(int(map(j, 0, 20, 0, c / 2)), c / 2, c * 2));

    }
    stripA.show();
    stripB.show();
    delay(50);
    Serial.println(int(255 * abs(sin(radians(i)))));
  }
}
