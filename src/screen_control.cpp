#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "global.h"

Adafruit_SSD1306 display(128, 32, &Wire, -1);
int x, minX;

namespace _10klab {
namespace screen {

void SetupScreen() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(100);
  // display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setTextWrap(false);
  x = display.width();
  delay(500);
  display.clearDisplay();
  display.setTextSize(2);
  int center_text = (display.width() / 2) - ((10 * _10klab::global::coctel_name.length()) / 2);
  display.setCursor(center_text, 11);
  display.print(_10klab::global::coctel_name);
  display.display();
}

void StaticText(String text){
  static unsigned long current_time = 0;
  static int delay_time = 5000;

  // if(millis() < current_time + delay_time){
  //   return;
  // }
  Serial.println("Display seteado");

  display.clearDisplay();
  display.setTextSize(2);
  int center_text = (display.width() / 2) - ((12 * text.length()) / 2);
  display.setCursor(center_text, 11);
  display.print(text);
  display.display();
  current_time = millis();
}

void ScrollText(int speed){
  minX = -20 * _10klab::global::coctel_name.length();
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(x, 11);
  display.print(_10klab::global::coctel_name);
  display.display();
  x=x-speed;
  if(x < minX) x = display.width();
}

} // namespace screen
} // namespace _10klab