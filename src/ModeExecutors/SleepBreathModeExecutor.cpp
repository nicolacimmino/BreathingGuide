
#include "SleepBreathModeExecutor.h"

void SleepBreathModeExecutor::doLoop()
{
    this->breatheFloodLight();

    if (this->getTimeSinceModeChange() > 1200000 /* 20 min */)
    {
        this->floodLight->shutdown();

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

    this->floodLight->setColor(cycleTime < effectiveSideDuration ? CRGB::Green : CRGB::Yellow);
    this->floodLight->setFade(fade);
}

unsigned long SleepBreathModeExecutor::getEffectiveSideDuration()
{
    uint8_t breathsPerMinute = max(4, 11 - floor(this->getTimeSinceModeChange() / 60000.0) + offset);

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

    if ((this->offset <= -5 && !positive) || (this->offset >= 5 && positive))
    {
        this->floodLight->override(300, positive ? CRGB::Black : CRGB::DarkRed, 127, !positive ? CRGB::Black : CRGB::DarkRed, 127);
        return;
    }

    this->offset += positive ? 1 : -1;

    this->floodLight->override(300, positive ? CRGB::Black : CRGB::DarkGreen, 127, !positive ? CRGB::Black : CRGB::DarkGreen, 127);
}