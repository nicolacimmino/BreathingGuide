
#include "NightlightModeExecutor.h"

void NighlightModeExecutor::doLoop()
{
    if (abs(this->lastX - this->accelerometer->getX()) > 64 || abs(this->lastY - this->accelerometer->getY()) > 64 || abs(this->lastZ - this->accelerometer->getZ()) > 64)
    {
        this->refreshActiveUntil();

        this->floodLight->setColor(this->getCurrentColor());
    }

    this->floodLight->setFade(this->fade);

    if (!this->latched && this->activeUntil < this->getTimeSinceModeChange())
    {
        this->floodLight->setColor(CRGB::Black);
    }

    this->lastX = this->accelerometer->getAveragedX();
    this->lastY = this->accelerometer->getAveragedY();
    this->lastZ = this->accelerometer->getAveragedZ();
}

void NighlightModeExecutor::doEnterMode()
{
    this->currentColorIndex = this->memoryController->getByte(MEMORY_NIGHTLIGHT_LAST_USED_COLOR_INDEX) % NIGHTLIGHT_COLORS_COUNT;

    this->floodLight->setColor(this->getCurrentColor());
    this->fade = 250;
}

void NighlightModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
    this->floodLight->setFade(255);
}

ColorsTuple NighlightModeExecutor::getModeSignatureColor()
{
    return {CRGB::White, CRGB::DarkSlateGray};
}

void NighlightModeExecutor::doOnShake()
{
    if (this->latched)
    {
        return;
    }

    this->fade = this->fade == 250 ? 50 : 250;
}

void NighlightModeExecutor::doOnClick()
{
    if (abs(this->accelerometer->getX()) < 96)
    {
        return;
    }

    if (this->accelerometer->getX() > 0)
    {
        this->latched = !this->latched;
        this->ledBarController->showBar(this->latched ? 1 : 0);

        return;
    }

    if (this->latched)
    {
        return;
    }

    this->currentColorIndex = (this->currentColorIndex + 1) % NIGHTLIGHT_COLORS_COUNT;
    this->floodLight->setColor(this->getCurrentColor());
    this->refreshActiveUntil();

    this->memoryController->setByte(MEMORY_NIGHTLIGHT_LAST_USED_COLOR_INDEX, this->currentColorIndex);
}

CRGB NighlightModeExecutor::getCurrentColor()
{
    return this->lightColors[this->currentColorIndex];
}

void NighlightModeExecutor::refreshActiveUntil()
{
    this->activeUntil = this->getTimeSinceModeChange() + 5000;
}