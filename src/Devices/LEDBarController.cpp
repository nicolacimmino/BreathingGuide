
#include "LEDBarController.h"

void LEDBarController::begin(uint8_t ledPins[LEDBAR_NUM_LEDS])
{
    memcpy(this->ledPins, ledPins, LEDBAR_NUM_LEDS);

    for (int ix = 0; ix < LEDBAR_NUM_LEDS; ix++)
    {
        pinMode(this->ledPins[ix], OUTPUT);
    }
}

void LEDBarController::showBar(uint8_t value)
{
    this->pattern = 0;
    for (int ix = 0; ix < value; ix++)
    {
        this->pattern = (this->pattern << 1) | 1;
    }
}

void LEDBarController::setDim()
{
    this->luminosityPWM = LEDBAR_PWM_DIM;
}

void LEDBarController::setBright()
{
    this->luminosityPWM = LEDBAR_PWM_BRIGHT;
}

void LEDBarController::setFullBrightness()
{
    this->luminosityPWM = LEDBAR_PWM_FULL;
}

void LEDBarController::loop()
{
    for (int ix = 0; ix < LEDBAR_NUM_LEDS; ix++)
    {
        analogWrite(this->ledPins[ix], this->pattern & (1 << ix) ? this->luminosityPWM : 0);
    }
}

