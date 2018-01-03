#include <Adafruit_NeoPixel.h>
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, PIN, NEO_GRB + NEO_KHZ800);
int sensorPin = A0;
int sensorValue = 0;
int value = 0;
int valueold = 0;

void setup(){
}
void loop(){
}


void   LedChangeOhyeah (int num) {
      if (num == 1) {
        theaterChase(50, 5);
        value = 1;
      }
      else if (num == 2) {
        breath(20, 80);
        strip.show();
        value = 2;
      }
      else if (num == 3) {
        rainbowCycle(50);
        value = 3;
      }
      if (value != valueold) {
        valueold = value;
        for (int n = 0; n < strip.numPixels(); n++) {
          strip.setPixelColor(n, strip.Color(0, 0, 0));
        }
        strip.show();
      }
    }
    void theaterChase(int c, uint8_t wait) {
      for (int j = 0; j < 1; j++) { //do 10 cycles of chasing
        for (int q = 0; q < 2; q++) {
          for (int i = 0; i < strip.numPixels() + 20; i = i + 10) {
            for (int a = 0; a < 10; a++) {
              strip.setPixelColor(i + q - a, strip.Color(c - (5 * (a + 1)), c - (5 * (a + 1)), c - (5 * (a + 1))));
              strip.setPixelColor(q - 9 - a, strip.Color(0, 0, 0));
              strip.setPixelColor(i + q - a, strip.Color(c - (5 * (a + 1)), c - (5 * (a + 1)), c - (5 * (a + 1))));
              strip.setPixelColor(i + q - 9 - a, strip.Color(0, 0, 0));
              delay(wait);
            }//turn every third pixel on

            strip.show();


          }
        }
      }
    }
    void breath(int c, uint8_t wait) {
      int i, j;

      for (j = 0; j < 21; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(c - j, c - j, c - j));
        }
        strip.show();
        delay(wait);
      }
      for (j = 0; j < 21; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(j, j, j));
        }
        strip.show();
        delay(wait);
      }
    }

    void rainbowCycle(uint8_t wait) {
      uint16_t i, j;

      for (j = 0; j < 128; j++) { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(((i * 255 / strip.numPixels()) + j ) & 127));
        }
        strip.show();
        //delay(wait);
      }
    }


    uint32_t Wheel(byte WheelPos) {
      if (WheelPos < 42) {
        return strip.Color(WheelPos * 3, 127 - WheelPos * 3, 0);
      } else if (WheelPos < 85) {
        WheelPos -= 42;
        return strip.Color(127 - WheelPos * 3, 0, WheelPos * 3);
      } else {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 127 - WheelPos * 3);
      }
    }

