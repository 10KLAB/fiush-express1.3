#include <Arduino.h>

#include "Adafruit_NeoPixel.h"
#include "global.h"

Adafruit_NeoPixel ambientStrip =
    Adafruit_NeoPixel(200, AMBIENTAL_LIGHTS, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel indicationStrip =
    Adafruit_NeoPixel(108, INDICATION_LIGHTS, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel hoseStrip =
    Adafruit_NeoPixel(16, HOSE_LIGHTS, NEO_GRB + NEO_KHZ800);

namespace _10klab {
namespace lights {
void RainbowHoseLights(int delay_time);
void SetIconsLight();
void SetStandbyGarnishLight();


void SetUpLights(int brightness) {
  ambientStrip.setBrightness(brightness);
  indicationStrip.setBrightness(brightness);
  hoseStrip.setBrightness(brightness);
  SetIconsLight();
  SetStandbyGarnishLight();
}

void TurnOffHoseLight() {
  for (int i = 0; i < hoseStrip.numPixels(); i++) {
    hoseStrip.setPixelColor(i, 0, 0, 0);
  }
  hoseStrip.show();
}

void HoseLightControl(int rainbow_fast_delay, int rainbow_slow_delay) {
  if (_10klab::global::machine_state) {
    RainbowHoseLights(rainbow_fast_delay);
  } else {
    RainbowHoseLights(rainbow_slow_delay);
  }
}

uint32_t WheelAmbiental(byte wheel_pos) {
  wheel_pos = 255 - wheel_pos;
  if (wheel_pos < 85) {
    return ambientStrip.Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  }
  if (wheel_pos < 170) {
    wheel_pos -= 85;
    return ambientStrip.Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  wheel_pos -= 170;
  return ambientStrip.Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}

uint32_t WheelHose(byte wheel_pos) {
  wheel_pos = 255 - wheel_pos;
  if (wheel_pos < 85) {
    return hoseStrip.Color(255 - wheel_pos * 3, 0, wheel_pos * 3);
  }
  if (wheel_pos < 170) {
    wheel_pos -= 85;
    return hoseStrip.Color(0, wheel_pos * 3, 255 - wheel_pos * 3);
  }
  wheel_pos -= 170;
  return hoseStrip.Color(wheel_pos * 3, 255 - wheel_pos * 3, 0);
}

void RainbowAmbienlaLights(int delay_time) {
  static unsigned long current_time = millis();
  static int i, j = 0;

  if (millis() < current_time + delay_time) {
    return;
  }
  j++;
  for (int i = 0; i < ambientStrip.numPixels(); i++) {
    ambientStrip.setPixelColor(i, WheelAmbiental((i + j) & 255));
  }
  ambientStrip.show();
  if (j >= 255) {
    j = 0;
  }
  current_time = millis();
}

void RainbowHoseLights(int delay_time) {
  static unsigned long current_time = millis();
  static int i, j = 0;

  if (millis() < current_time + delay_time) {
    return;
  }
  j++;
  for (int i = 0; i < hoseStrip.numPixels(); i++) {
    hoseStrip.setPixelColor(i, WheelAmbiental((i + j) & 255));
  }
  hoseStrip.show();
  if (j >= 255) {
    j = 0;
  }
  current_time = millis();
}

void SetIconsLight() {
  
  for (int i = 40; i < 60; i++) // Icon
  {
    indicationStrip.setPixelColor(i, 255, 255, 255);
  }
  for (int i = 60; i < 104; i++) // Brand
  {
    indicationStrip.setPixelColor(i, 255, 255, 255);
  }
  for (int i = 104; i < 108; i++) // Drops
  {
    indicationStrip.setPixelColor(i, 0, 0, 0);
  }

  indicationStrip.show();
}

void SetStandbyGarnishLight() {
  for (int i = 0; i < 7; i++) // Garnish one right to left
  {
    indicationStrip.setPixelColor(i, 0, 188, 188);
  }
  for (int i = 7; i < 20; i++) // Garnish two right to left
  {
    indicationStrip.setPixelColor(i, 0, 188, 188);
  }
  for (int i = 20; i < 33; i++) // Garnish tree right to left
  {
    indicationStrip.setPixelColor(i, 0, 188, 188);
  }
  for (int i = 33; i < 40; i++) // Garnish four right to left
  {
    indicationStrip.setPixelColor(i, 0, 188, 188);
  }

  indicationStrip.show();
}

void SetDispenseGarnishLight(bool cherry, bool lemon, bool orange, bool herb) {
  if (cherry) {
    for (int i = 0; i < 7; i++) // Garnish one right to left
    {
      indicationStrip.setPixelColor(i, 204, 0, 0);//red
    }
  } else {
    for (int i = 0; i < 7; i++) // Garnish one right to left
    {
      indicationStrip.setPixelColor(i, 0, 0, 0);
    }
  }

  if (lemon) {
    for (int i = 7; i < 20; i++) // Garnish two right to left
    {
      indicationStrip.setPixelColor(i, 0, 204, 0);//green
    }
  } else {
    for (int i = 7; i < 20; i++) // Garnish two right to left
    {
      indicationStrip.setPixelColor(i, 0, 0, 0);
    }
  }

  if (orange) {
    for (int i = 20; i < 33; i++) // Garnish tree right to left
    {
      indicationStrip.setPixelColor(i, 255, 50, 0);//orange
    }
  } else {
    for (int i = 20; i < 33; i++) // Garnish tree right to left
    {
      indicationStrip.setPixelColor(i, 0, 0, 0);
    }
  }

  if (herb) {
    for (int i = 33; i < 40; i++) // Garnish four right to left
    {
      indicationStrip.setPixelColor(i, 255, 128, 0);//yellow
    }
  } else {
    for (int i = 33; i < 40; i++) // Garnish four right to left
    {
      indicationStrip.setPixelColor(i, 0, 0, 0);
    }
  }

  indicationStrip.show();
}

void SetOneColorDropslight() {
  for (int i = 104; i < 108; i++) {
    indicationStrip.setPixelColor(i, 255, 255, 255); //Drops
  }
  indicationStrip.show();
}

void TurnOffDropsLight(){
  for (int i = 104; i < 108; i++) {
    indicationStrip.setPixelColor(i, 0, 0, 0); //Drops
  }
  indicationStrip.show();  
}

void DropsIndicationLight(char mode, int blink_delay) {
  static unsigned long current_time = millis();
  static bool blink_flag = false;

  if (millis() < current_time + blink_delay) {
    return;
  }

  if (blink_flag) {
    switch (mode) {
    case 'W': // drops light while waiting for cup
      for (int i = 104; i < 108; i++) {
        indicationStrip.setPixelColor(i, 0, 0, 255);
      }
      break;
    case 'R':                       // drops light while waiting for retire cup
      for (int i = 104; i < 108; i++) // Drops
      {
        indicationStrip.setPixelColor(i, 255, 0, 0);
      }
    }
    blink_flag = false;
  } else {
    for (int i = 104; i < 108; i++) // Drops
    {
      indicationStrip.setPixelColor(i, 0, 0, 0);
    }
    blink_flag = true;
  }
  indicationStrip.show();
  current_time = millis();
}

unsigned long UpdateGarnishTime(){
return millis();
}

void TurnOffGarnishLightByTime(){
  const int delay_time = 3000;
  static bool start_time_count = false;
  static unsigned long current_time = millis();
  current_time = _10klab::global::garnish_time;

  if(!_10klab::global::garnish_light_control_by_time){
    // current_time = millis();
    // SetIconsLight();
    // Serial.println("Garnish by time false");
    return;
  }
  if(!start_time_count){
    start_time_count = true;
    current_time = millis();
  }
  if(millis() < current_time + delay_time){
    // Serial.println("Esperando tiempo para apagar garnish");
    return;
  }
  SetStandbyGarnishLight();
  SetIconsLight();
  Serial.println("Luces seteadas por tiempo");
  start_time_count = false;
  _10klab::global::garnish_light_control_by_time = false;

}

} // namespace lights
} // namespace _10klab