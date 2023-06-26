#ifndef GLOBAL_H_
#define GLOBAL_H_

#define CUP_SENSOR_ONE 39
#define CUP_SENSOR_TWO 36
#define ENABLE_PUMPS 23
#define HOSE_LIGHTS 17
#define AMBIENTAL_LIGHTS 26
#define INDICATION_LIGHTS 25
#define LED_ON_BOARD 2
#define AUX_PIN 16

namespace _10klab {
namespace global {
    extern bool machine_state;
    extern bool garnish_light_control_by_time;
    extern unsigned long garnish_time;
    extern bool screen_mode;
    extern String coctel_name;

    extern int pump1;
    extern float ml1;
    extern int priority1;
    extern bool rotation1;
    extern float ka1;
    extern float kb1;

    extern int pump2;
    extern float ml2;
    extern int priority2;
    extern bool rotation2;
    extern float ka2;
    extern float kb2;

    extern int pump3;
    extern float ml3;
    extern int priority3;
    extern bool rotation3;
    extern float ka3;
    extern float kb3;

    extern int pump4;
    extern float ml4;
    extern int priority4;
    extern bool rotation4;
    extern float ka4;
    extern float kb4;

    extern int pump5;
    extern float ml5;
    extern int priority5;
    extern bool rotation5;
    extern float ka5;
    extern float kb5;

    extern int pump6;
    extern float ml6;
    extern int priority6;
    extern bool rotation6;
    extern float ka6;
    extern float kb6;
    
    extern bool cherry;
    extern bool lemon;
    extern bool orange;
    extern bool herb;
    
    void SetUpPins();

    void ResetVariables();

} // namespace global

} // namespace _10klab
#endif