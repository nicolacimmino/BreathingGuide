
#include "SquareBreathModeExecutor.h"

void SquareBreathModeExecutor::doLoop()
{
    if (this->exerciseEnded)
    {
        if (this->getTimeSinceModeChange() % 2000 < 300)
        {
            this->floodLight->setColor(CRGB::Gold);
            this->floodLight->setFade(190);
            return;
        }
        this->floodLight->setColor(CRGB::Black);
        return;
    }

    this->updateElapsedTimeBar(300000 /* 5min */);
    this->breatheFloodLight();
}

void SquareBreathModeExecutor::breatheFloodLight()
{
    unsigned long effectiveSideDuration = this->getEffectiveSideDuration();

    unsigned long cycleTime = this->getTimeSinceModeChange() % (effectiveSideDuration * 4);

    uint8_t fade = 0;

    if (cycleTime < effectiveSideDuration)
    {
        fade = (255.0 * (effectiveSideDuration - cycleTime)) / effectiveSideDuration;
    }
    else if (cycleTime < effectiveSideDuration * 2.0)
    {
        fade = 0;
    }
    else if (cycleTime < effectiveSideDuration * 3)
    {
        fade = 255.0 * ((cycleTime - (effectiveSideDuration * 2.0)) / effectiveSideDuration);
    }
    else
    {
        fade = 255;
    }

    fade = min(250, fade);

    this->floodLight->setColor((cycleTime % effectiveSideDuration < 300) ? CRGB::DarkOrange : CRGB::Green);
    this->floodLight->setFade(fade);
}

unsigned long SquareBreathModeExecutor::getEffectiveSideDuration()
{
    // Slow down by one second every 5 minutes, but never above 7s.
    return min(7000, this->sideDuration + this->getTimeSinceModeChange() / (5 * 60));
}

void SquareBreathModeExecutor::doEnterMode()
{
    this->exerciseEnded = false;
}

void SquareBreathModeExecutor::doExitMode()
{
    this->ledBarController->setDim();
    this->ledBarController->showBar(0);

    this->floodLight->setColor(CRGB::Black);
}

ColorsTuple SquareBreathModeExecutor::getModeSignatureColor()
{
    return {CRGB::DarkGreen, CRGB::Yellow};
}

void SquareBreathModeExecutor::doOnClick()
{
    if (abs(this->accelerometer->getX()) < 96)
    {
        return;
    }

    bool positive = this->accelerometer->getX() > 0;

    if ((this->sideDuration <= 2000 && positive) || (this->sideDuration >= 7000 && !positive))
    {        
        this->floodLight->override(300, !positive ? CRGB::Black : CRGB::DarkRed, 127, positive ? CRGB::Black : CRGB::DarkRed, 127);
        return;
    }

    this->sideDuration += positive ? -1000 : 1000;

    this->floodLight->override(300, !positive ? CRGB::Black : CRGB::DarkGreen, 127, positive ? CRGB::Black : CRGB::DarkGreen, 127);
}