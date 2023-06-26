#ifndef LIGHTS_CONTROL_H_
#define LIGHTS_CONTROL_H_

namespace _10klab{
    namespace lights{
        void SetUpLights(int brightness);
        void HoseLightControl(int rainbow_fast_delay, int rainbow_slow_delay); 
        void RainbowAmbienlaLights(int delay_time);
        void SetDispenseGarnishLight(bool cherry, bool lemon, bool orange, bool herb);
        void SetStandbyGarnishLight();
        void DropsIndicationLight(char mode, int blink_delay);
        void SetOneColorDropslight();
        void TurnOffGarnishLightByTime();
        void TurnOffDropsLight();
        void SetIconsLight();
        unsigned long UpdateGarnishTime();
    }
}

#endif