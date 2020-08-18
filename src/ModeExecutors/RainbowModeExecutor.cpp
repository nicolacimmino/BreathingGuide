
#include "RainbowModeExecutor.h"

void RainbowModeExecutor::doLoop()
{
    if (this->autoMode)
    {
        this->floodLight->setColor(CRGB::CRGB(CHSV::CHSV(255 * ((this->getTimeSinceModeChange() % 10000) / 10000.0), 180, 255)));

        return;
    }

    this->floodLight->setColor(CRGB::CRGB(this->getColorComponent(this->accelerometer->getAveragedX()), this->getColorComponent(this->accelerometer->getAveragedY()), this->getColorComponent(this->accelerometer->getAveragedZ())));
}

void RainbowModeExecutor::doEnterMode()
{
    this->floodLight->setFade(0);
}

void RainbowModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
    this->floodLight->setFade(255);
}

uint8_t RainbowModeExecutor::getColorComponent(int8_t axisReading)
{
    return abs(axisReading) * 2;
}

void RainbowModeExecutor::doOnShake()
{
    this->autoMode = !this->autoMode;
    this->floodLight->override(300, CRGB::Black, 0);
}

ColorsTuple RainbowModeExecutor::getModeSignatureColor()
{
    return {CRGB::Blue, CRGB::DarkRed};
}