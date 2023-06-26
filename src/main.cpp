#include <Arduino.h>

#include "global.h"
#include "pump_control.h"
#include "web_services.h"
#include "lights_control.h"
#include "screen_control.h"
#include "Pushbutton.h"


Pushbutton sensorOne(CUP_SENSOR_ONE);
Pushbutton sensorTwo(CUP_SENSOR_TWO);


TaskHandle_t secondCore;
TaskHandle_t firstCore;


void FirstCore(void *pvParameters);
void SecondCore(void *pvParameters);

void StartCocktailPreparation();
void TimmingMessage();

void setup() {
  Serial.begin(115200);


  _10klab::global::SetUpPins();
  _10klab::web_services::ConnectWiFi();

  _10klab::screen::SetupScreen();

  _10klab::lights::SetUpLights(200);
  _10klab::pumps::PumpsInitialization();
  xTaskCreatePinnedToCore(SecondCore, "secondCore", 10000, NULL, 1, &secondCore, 0);
  delay(500);
  xTaskCreatePinnedToCore(FirstCore, "firstCore", 10000, NULL, 1, &firstCore, 1);
  delay(500);
  Serial.println("iniciado");

}

void FirstCore(void *pvParameters) {
  for (;;) {
    // _10klab::screen::StaticText(_10klab::global::coctel_name);
    TimmingMessage();

    if(_10klab::global::machine_state){
      StartCocktailPreparation();
      _10klab::global::machine_state = false;
    }
    _10klab::lights::TurnOffGarnishLightByTime();
    delay(10);
  }
  vTaskDelay(10);
}

void SecondCore(void *pvParameters) {
  for (;;) {

    _10klab::web_services::LoopServer();
    _10klab::lights::RainbowAmbienlaLights(300);
    _10klab::lights::HoseLightControl(5, 300);
    // _10klab::lights::TurnOffGarnishLightByTime();

    delay(2);

  }
  vTaskDelay(10);
}

void loop() {

  delay(10);
}

// void CupSensorActivation(){
//   const int verification_sensor_delay = 100;
//   bool detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
//   bool detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
//   static char weigting_for_cup = 'W';
//   static int blink_time = 200;
//   const int speed_scroll = 2;
  
//   while(!detection_sensor_one && !detection_sensor_two){
//     // Serial.println("estoy en sensor activation");
//     // _10klab::screen::ScrollText(speed_scroll);
//     _10klab::lights::DropsIndicationLight(weigting_for_cup, blink_time);
//     detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
//     detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
//     if(detection_sensor_one || detection_sensor_two){
//       delay(verification_sensor_delay);
//       detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
//       detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
//     }
//     delay(10);
//   }
// }

// void CupSensorDeactivation(){
//   const int verification_sensor_delay = 100;
//   bool detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
//   bool detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
//   static char weigting_for_retire_cup = 'R';
//   static int blink_time = 100;
//   const int speed_scroll = 2;
  
//   while(detection_sensor_one || detection_sensor_two){
//     // Serial.println("estoy en sensor deactivation");
//     _10klab::screen::ScrollText(speed_scroll);
//     _10klab::lights::DropsIndicationLight(weigting_for_retire_cup, blink_time);
//     detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
//     detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
//     if(detection_sensor_one && detection_sensor_two){
//       delay(verification_sensor_delay);
//       detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
//       detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
//     }
//     delay(10);
//   }
// }
void TimmingMessage(){
  static unsigned long current_time = 0;
  const int delay_time = 3000;

  if(millis() < current_time + delay_time){
    return;
  }
  Serial.println("im here");
  current_time = millis();
}

bool CupSensorActivation(){
  // const int verification_sensor_delay = 100;
  // bool detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
  // bool detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
  static char weigting_for_cup = 'W';
  static int blink_time = 200;
  unsigned long current_time = millis();
  const unsigned long time_out = 3*60*1000;

  // const int speed_scroll = 2;
  Serial.println("Antes de leer sensor Activation");
  while(!sensorOne.getSingleDebouncedRelease() && !sensorTwo.getSingleDebouncedRelease()){
    // Serial.println("estoy en sensor activation");
    // _10klab::screen::ScrollText(speed_scroll);
    _10klab::lights::DropsIndicationLight(weigting_for_cup, blink_time);
    TimmingMessage();
    if(millis() > current_time + time_out){
      return false;
    }

    // Serial.println("leyendo sensores Activation");

    // detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
    // detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
    // if(detection_sensor_one || detection_sensor_two){
    //   delay(verification_sensor_delay);
    //   detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
    //   detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
    // }
    delay(10);
  }
  return true;
  Serial.println("Despues de leer sensor Activation");
}

void CupSensorDeactivation(){
  // const int verification_sensor_delay = 100;
  // bool detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
  // bool detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
  static char weigting_for_retire_cup = 'R';
  static int blink_time = 100;
  const int speed_scroll = 2;

  Serial.println("Antes de leer sensor Deactivation");
  while(!sensorOne.getSingleDebouncedPress() && !sensorTwo.getSingleDebouncedPress()){
    // Serial.println("estoy en sensor deactivation");
    // _10klab::screen::ScrollText(speed_scroll);
    _10klab::lights::DropsIndicationLight(weigting_for_retire_cup, blink_time);
    TimmingMessage();
    // Serial.println("leyendo sensores Deactivation");
    // detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
    // detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
    // if(detection_sensor_one && detection_sensor_two){
    //   delay(verification_sensor_delay);
    //   detection_sensor_one = digitalRead(CUP_SENSOR_ONE);
    //   detection_sensor_two = digitalRead(CUP_SENSOR_TWO);
    // }
    delay(10);
  }
  Serial.println("Despues de leer sensor Deactivation");
}

void StartCocktailPreparation(){
  // _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
  // delay(100);
  Serial.println("StartPreparation step 1");
  // delay(100);
  _10klab::global::garnish_light_control_by_time = false;
  // delay(100);
  _10klab::lights::SetDispenseGarnishLight(_10klab::global::cherry, _10klab::global::lemon, _10klab::global::orange, _10klab::global::herb);
    Serial.println("StartPreparation step 2");
  // delay(100);

  Serial.println("StartPreparation step 3");

  _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
  _10klab::screen::StaticText(_10klab::global::coctel_name);
  _10klab::web_services::ConnectWiFi();
  delay(100);

  bool start = CupSensorActivation();

  // _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
  // delay(100);
  // _10klab::screen::StaticText(_10klab::global::coctel_name);
  // delay(100);
  // _10klab::web_services::ConnectWiFi();
  // delay(100);

  // delay(100);
    Serial.println("StartPreparation step 4");
  // delay(100);
  // delay(100);
  Serial.println("StartPreparation step 5");
  // delay(100);
  // _10klab::pumps::PumpsInitialization();//Cuidado solo prueba!!!!
  if(start){
    _10klab::lights::SetOneColorDropslight();

    _10klab::pumps::PriorityOrder(_10klab::global::pump1, _10klab::global::ml1, _10klab::global::priority1, _10klab::global::rotation1, _10klab::global::ka1, _10klab::global::kb1,
                                  _10klab::global::pump2, _10klab::global::ml2, _10klab::global::priority2, _10klab::global::rotation2, _10klab::global::ka2, _10klab::global::kb2,
                                  _10klab::global::pump3, _10klab::global::ml3, _10klab::global::priority3, _10klab::global::rotation3, _10klab::global::ka3, _10klab::global::kb3,
                                  _10klab::global::pump4, _10klab::global::ml4, _10klab::global::priority4, _10klab::global::rotation4, _10klab::global::ka4, _10klab::global::kb4,
                                  _10klab::global::pump5, _10klab::global::ml5, _10klab::global::priority5, _10klab::global::rotation5, _10klab::global::ka5, _10klab::global::kb5,
                                  _10klab::global::pump6, _10klab::global::ml6, _10klab::global::priority6, _10klab::global::rotation6, _10klab::global::ka6, _10klab::global::kb6);
  
  // _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
  // _10klab::screen::StaticText(_10klab::global::coctel_name);
  // _10klab::web_services::ConnectWiFi();
  // delay(100);

    Serial.println("StartPreparation step 6");
    CupSensorDeactivation();
  }

  // delay(100);
  _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
  // delay(100);
  _10klab::global::coctel_name = "FIUSH";
  // delay(100);
  _10klab::screen::StaticText("FIUSH");
  // delay(100);
  _10klab::web_services::ConnectWiFi();
  // delay(100);

    Serial.println("StartPreparation step 7");
  _10klab::lights::TurnOffDropsLight();
  // delay(100);
  // _10klab::lights::SetOneColorDropslight();
    Serial.println("StartPreparation step 8");
  _10klab::global::garnish_time = _10klab::lights::UpdateGarnishTime();
    Serial.println("StartPreparation step 9");
  _10klab::global::garnish_light_control_by_time = true;
    Serial.println("StartPreparation step 10");
  _10klab::global::ResetVariables();
  // delay(100);
  // delay(100);
  // _10klab::web_services::ConnectWiFi();
}



// void StartCocktailPreparation(){
//   // _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
//   // delay(100);
//   Serial.println("StartPreparation step 1");
//   // delay(100);
//   _10klab::global::garnish_light_control_by_time = false;
//   // delay(100);
//   _10klab::lights::SetDispenseGarnishLight(_10klab::global::cherry, _10klab::global::lemon, _10klab::global::orange, _10klab::global::herb);
//     Serial.println("StartPreparation step 2");
//   // delay(100);

//   Serial.println("StartPreparation step 3");
//   CupSensorActivation();

//   // _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
//   // delay(100);
//   // _10klab::screen::StaticText(_10klab::global::coctel_name);
//   // delay(100);
//   // _10klab::web_services::ConnectWiFi();
//   // delay(100);

//   // delay(100);
//     Serial.println("StartPreparation step 4");
//   // delay(100);
//   _10klab::lights::SetOneColorDropslight();
//   // delay(100);
//   Serial.println("StartPreparation step 5");
//   // delay(100);
//   // _10klab::pumps::PumpsInitialization();//Cuidado solo prueba!!!!
//   _10klab::pumps::PriorityOrder(_10klab::global::pump1, _10klab::global::ml1, _10klab::global::priority1, _10klab::global::rotation1, _10klab::global::ka1, _10klab::global::kb1,
//                                 _10klab::global::pump2, _10klab::global::ml2, _10klab::global::priority2, _10klab::global::rotation2, _10klab::global::ka2, _10klab::global::kb2,
//                                 _10klab::global::pump3, _10klab::global::ml3, _10klab::global::priority3, _10klab::global::rotation3, _10klab::global::ka3, _10klab::global::kb3,
//                                 _10klab::global::pump4, _10klab::global::ml4, _10klab::global::priority4, _10klab::global::rotation4, _10klab::global::ka4, _10klab::global::kb4,
//                                 _10klab::global::pump5, _10klab::global::ml5, _10klab::global::priority5, _10klab::global::rotation5, _10klab::global::ka5, _10klab::global::kb5,
//                                 _10klab::global::pump6, _10klab::global::ml6, _10klab::global::priority6, _10klab::global::rotation6, _10klab::global::ka6, _10klab::global::kb6);
  
//   _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
//   _10klab::screen::StaticText(_10klab::global::coctel_name);
//   _10klab::web_services::ConnectWiFi();
//   delay(100);

//   Serial.println("StartPreparation step 6");
//   CupSensorDeactivation();

//   // delay(100);
//   _10klab::screen::SetupScreen();//Cuidado solo prueba!!!!
//   // delay(100);
//   _10klab::global::coctel_name = "FIUSH";
//   // delay(100);
//   _10klab::screen::StaticText("FIUSH");
//   // delay(100);
//   _10klab::web_services::ConnectWiFi();
//   // delay(100);

//     Serial.println("StartPreparation step 7");
//   _10klab::lights::TurnOffDropsLight();
//   // delay(100);
//   // _10klab::lights::SetOneColorDropslight();
//     Serial.println("StartPreparation step 8");
//   _10klab::global::garnish_time = _10klab::lights::UpdateGarnishTime();
//     Serial.println("StartPreparation step 9");
//   _10klab::global::garnish_light_control_by_time = true;
//     Serial.println("StartPreparation step 10");
//   _10klab::global::ResetVariables();
//   // delay(100);
//   // delay(100);
//   // _10klab::web_services::ConnectWiFi();
// }