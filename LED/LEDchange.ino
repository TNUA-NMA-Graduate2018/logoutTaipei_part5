
#include <Adafruit_NeoPixel.h>
//#include <ESP8266WiFi.h>
int pixelNumber = 120;
Adafruit_NeoPixel stripA = Adafruit_NeoPixel(pixelNumber, D6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB = Adafruit_NeoPixel(pixelNumber, D7, NEO_GRB + NEO_KHZ800);

int value = 1;
int valueold = 0;

//Hello I changed

//WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(10);
  stripA.begin();
  stripB.begin();

  //const char* ssid     = "dlink-DC88";
  //const char* password = "ydeok89348";
  

}
void loop() {

  value=Seiral.read();
  if (request.indexOf("/LED=one") != -1) {
    //LedChangeOhyeah (1);
    value = 1;
  }
  if (request.indexOf("/LED=two") != -1) {
    //LedChangeOhyeah (2);
    value = 2;
  }
  if (request.indexOf("/LED=three") != -1) {
    //LedChangeOhyeah (3);
    value = 3;
  }

  switch (value) {

    case 1: LedChangeOhyeah (1); break;
    case 2: LedChangeOhyeah (2); break;
    case 3: LedChangeOhyeah (3); break;

  }
/*
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.print("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");

  if (value == 1) {
    client.print("one");
  } else if (value == 2) {
    client.print("two");
  }
  else if (value == 3) {
    client.print("three");
  }
  else {
    client.print("one");
  }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=one\">here</a> turn the LED pin D7 to mode one<br>");
  client.println("Click <a href=\"/LED=two\">here</a> turn the LED  pin D7 to mode two<br>");
  client.println("Click <a href=\"/LED=three\">here</a> turn the LED  pin D7 to mode three<br>");
  client.println("</html>");
*/
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

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

