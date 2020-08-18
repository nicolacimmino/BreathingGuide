#ifndef __LEDBARCONTROLLER_H__
#define __LEDBARCONTROLLER_H__

#define LEDBAR_PDM_DIM 20
#define LEDBAR_PDM_BRIGHT 1
#define LEDBAR_PDM_FULL 0
#define LEDBAR_NUM_LEDS 4

#include <Arduino.h>

class LEDBarController
{
public:
    void begin(uint8_t ledPins[LEDBAR_NUM_LEDS]);
    void loop();
    void showBar(uint8_t value);
    void setDim();
    void setBright();
    void setFullBrightness();

private:
    uint8_t ledPins[LEDBAR_NUM_LEDS];
    uint8_t pattern;
    uint32_t lastPDMPulseTime;
    uint8_t luminosityPDM;
    void showPattern();
};

#endif