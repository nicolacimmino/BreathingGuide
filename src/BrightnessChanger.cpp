#include "BrightnessChanger.h"

void BrightnessChanger::begin(FloodLight *floodLight, MemoryController *memoryController, Accelerometer *accelerometer)
{
    this->memoryController = memoryController;
    this->accelerometer  =accelerometer;

    if (this->memoryController->getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP) > 0)
    {
        this->brightness = this->memoryController->getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP);
    }

    this->floodLight = floodLight;
    this->activeUntil = millis() + this->timeout;
    this->floodLight->setColor(CRGB::Yellow);
    this->floodLight->setBrightness(this->brightness);
    this->floodLight->loop();
}

void BrightnessChanger::loop()
{
    if (this->activeUntil == 0)
    {
        return;
    }

    if (this->activeUntil < millis())
    {
        this->floodLight->setBrightness(max(this->memoryController->getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP), 50));

        this->activeUntil = 0;
    }
}

void BrightnessChanger::onClick()
{
    this->activeUntil = millis() + this->timeout;

    if (abs(this->accelerometer->getX()) < 96)
    {
        return;
    }

    int8_t change = 50 * (this->accelerometer->getX() > 0 ? -1 : 1);

    if((change < 0 && this->brightness <= 50) ||  (change > 0 && this->brightness > 200)) {
        this->floodLight->override(200, CRGB::Red, 200);
        return;            
    }

    this->brightness = this->brightness + change;
    this->floodLight->setBrightness(this->brightness);    
}

void BrightnessChanger::onLongPress()
{
    this->floodLight->override(200, CRGB::Black, 0);
    this->activeUntil = 0;
    this->memoryController->setByte(MEMORY_GLOBAL_BRIGHTNESS_CAP, this->brightness);
}

bool BrightnessChanger::isActive()
{
    return this->activeUntil != 0;
}
