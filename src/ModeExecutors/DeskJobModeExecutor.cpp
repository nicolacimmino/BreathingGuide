
#include "DeskJobModeExecutor.h"

void DeskJobModeExecutor::doLoop()
{
    if (this->getTimeSecondsInMinute() < 5)
    {
        for (uint8_t ix = 0; ix < CRONS_COUNT; ix++)
        {
            if ((this->getTimeMinutes() - this->cron[ix].offsetMinutes) % this->cron[ix].intervalMinutes == 0 && this->getTimeMinutes() >= this->cron[ix].offsetMinutes)
            {
                this->alarmed[ix] = true;
            }
        }
    }

    bool alarm = false;
    for (uint8_t ix = 0; ix < CRONS_COUNT; ix++)
    {
        if (this->alarmed[ix])
        {
            CRGB color1 = (millis() % 2000 < 1000) ? this->cron[ix].color : CRGB::Black;
            CRGB color2 = (millis() % 2000 < 1000) ? CRGB::Black : this->cron[ix].color;
            this->floodLight->setColor(color1, color2);
            this->floodLight->setFade(0);
            alarm = true;
            break;
        }
    }

    if (!alarm)
    {
        this->floodLight->setColor(CRGB::Yellow, CRGB::Yellow);

        // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
        // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
        float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;

        this->floodLight->setFade(255 - val);
    }
}

void DeskJobModeExecutor::doEnterMode()
{
    this->resetModeChangeTime();
}

void DeskJobModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
}

ColorsTuple DeskJobModeExecutor::getModeSignatureColor()
{
    return {CRGB::Blue, CRGB::Purple};
}

void DeskJobModeExecutor::doOnShake()
{
    for (uint8_t ix = 0; ix < CRONS_COUNT; ix++)
    {
        if (this->alarmed[ix])
        {
            this->alarmed[ix] = false;
            this->floodLight->setColor(CRGB::Black);
            break;
        }
    }
}

void DeskJobModeExecutor::doOnTilt(uint8_t axis, bool positive)
{
}

uint16_t DeskJobModeExecutor::getTimeMinutes()
{
    return floor((this->getTimeSinceModeChange() / 1000) / 60.0);
}

uint16_t DeskJobModeExecutor::getTimeSecondsInMinute()
{
    return (this->getTimeSinceModeChange() / 1000) % 60;
}
