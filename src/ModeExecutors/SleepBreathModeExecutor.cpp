
#include "SleepBreathModeExecutor.h"

void SleepBreathModeExecutor::doLoop()
{
    this->breatheFloodLight();

    if (this->getTimeSinceModeChange() > 600000 /* 10 min */)
    {
        this->floodLight->setColor(CRGB::Black);

        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
    }
}

void SleepBreathModeExecutor::breatheFloodLight()
{
    long effectiveSideDuration = this->getEffectiveSideDuration();
    long cycleTime = this->getTimeSinceModeChange() % (effectiveSideDuration * 2);

    uint8_t fade = 2 * abs(floor(127.0 * (effectiveSideDuration - cycleTime) / effectiveSideDuration));
    uint8_t fade2 = 255 - fade;

    this->floodLight->setColor(CRGB::Green);
    this->floodLight->setFade(fade);
}

unsigned long SleepBreathModeExecutor::getEffectiveSideDuration()
{
    uint8_t breathsPerMinute = max(5, 11 - floor(this->getTimeSinceModeChange() / 60000.0));

    return 60000 / (2 * breathsPerMinute);
}

void SleepBreathModeExecutor::doEnterMode()
{
    this->exerciseEnded = false;
}

void SleepBreathModeExecutor::doExitMode()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setColor(CRGB::Black);
}

ColorsTuple SleepBreathModeExecutor::getModeSignatureColor()
{
    return {CRGB::DarkOrange, CRGB::DarkOrange};
}

void SleepBreathModeExecutor::doOnClick()
{
    if (abs(this->accelerometer->getX()) < 96)
    {
        return;
    }

    bool positive = this->accelerometer->getX() > 0;
}