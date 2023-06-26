#include <Arduino.h>
#include "global.h"

namespace _10klab {
namespace global {

bool machine_state = false;
bool garnish_light_control_by_time = false;
unsigned long garnish_time = 0;
bool screen_mode = false;

String coctel_name = "FIUSH";

int pump1 = 99;
float ml1 = 99;
int priority1 = 99;
bool rotation1 = 0;
float ka1 = 1;
float kb1 = 0;

int pump2 = 99;
float ml2 = 99;
int priority2 = 99;
bool rotation2 = 0;
float ka2 = 1;
float kb2 = 0;

int pump3 = 99;
float ml3 = 99;
int priority3 = 99;
bool rotation3 = 0;
float ka3 = 1;
float kb3 = 0;

int pump4 = 99;
float ml4 = 99;
int priority4 = 99;
bool rotation4 = 0;
float ka4 = 1;
float kb4 = 0;

int pump5 = 99;
float ml5 = 99;
int priority5 = 99;
bool rotation5 = 0;
float ka5 = 1;
float kb5 = 0;

int pump6 = 99;
float ml6 = 99;
int priority6 = 99;
bool rotation6 = 0;
float ka6 = 1;
float kb6 = 0;

bool cherry = 0;
bool lemon = 0;
bool orange = 0;
bool herb = 0;

void SetUpPins() {
  // pinMode(CUP_SENSOR_ONE, INPUT);
  // pinMode(CUP_SENSOR_TWO, INPUT);
  pinMode(ENABLE_PUMPS, OUTPUT);
  pinMode(HOSE_LIGHTS, OUTPUT);
  pinMode(AMBIENTAL_LIGHTS, OUTPUT);
  pinMode(INDICATION_LIGHTS, OUTPUT);
  pinMode(LED_ON_BOARD, OUTPUT);
  pinMode(AUX_PIN, OUTPUT);
}

void ResetVariables(){
  _10klab::global::pump1 = 99;
  _10klab::global::pump2 = 99;
  _10klab::global::pump3 = 99;
  _10klab::global::pump4 = 99;
  _10klab::global::pump5 = 99;
  _10klab::global::pump6 = 99;
  _10klab::global::ml1 = 99;
  _10klab::global::ml2 = 99;
  _10klab::global::ml3 = 99;
  _10klab::global::ml4 = 99;
  _10klab::global::ml5 = 99;
  _10klab::global::ml6 = 99;
  _10klab::global::priority1 = 99;
  _10klab::global::priority2 = 99;
  _10klab::global::priority3 = 99;
  _10klab::global::priority4 = 99;
  _10klab::global::priority5 = 99;
  _10klab::global::priority6 = 99;
}

} // namespace global

} // namespace _10klab