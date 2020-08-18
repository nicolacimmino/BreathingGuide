
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
    this->luminosityPDM = LEDBAR_PDM_DIM;
}

void LEDBarController::setBright()
{
    this->luminosityPDM = LEDBAR_PDM_BRIGHT;
}

void LEDBarController::setFullBrightness()
{
    this->luminosityPDM = LEDBAR_PDM_FULL;
}

void LEDBarController::loop()
{
    if (millis() - this->lastPDMPulseTime > this->luminosityPDM)
    {
        this->showPattern();
        this->lastPDMPulseTime = millis();
    }
}

void LEDBarController::showPattern()
{
    for (int ix = 0; ix < LEDBAR_NUM_LEDS; ix++)
    {
        digitalWrite(this->ledPins[ix], this->pattern & (1 << ix));
    }

    if(this->luminosityPDM == LEDBAR_PDM_FULL) {
        return;
    }

    for (int ix = 0; ix < LEDBAR_NUM_LEDS; ix++)
    {
        digitalWrite(this->ledPins[ix], LOW);
    }
}
