#include <Adafruit_NeoPixel.h>
int pixelNumber = 120;//記得改成正確的
int mid = 60; //記得改成正確的
int pinr = A0;
int pinl = A1;
int pinled = 2;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNumber, 2, NEO_GRB + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
  strip.begin();
  pinMode(pinl, OUTPUT);
  pinMode(pinr, OUTPUT);
  Serial.begin(115200);

}
// 0後退 1不動 2前進
void loop() {
  // put your main code here, to run repeatedly:
  breath(l, r);
}
void breath(int l, int r) {
  int i, j, a;
  for (i = 0; i < mid; i += 10) {
    for (j = 0; j < 10; j++) {
      strip.setPixelColor(i + j, strip.Color(60 - l * 15, 10 + l * 40, 10 + l * 35 + j * 4));
      strip.show();
    }
  }
  for (i = mid; i < strip.numPixels(); i += 10) {
    for (j = 0; j < 10; j++) {
      strip.setPixelColor(i + j, strip.Color(60 - r * 15, 10 + r * 40, 10 + r * 35 + j * 4));
      strip.show();
    }
  }
}
