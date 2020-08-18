
#include "ModesChanger.h"

void ModesChanger::begin(ModeExecutor **modeExecutors, uint8_t modesCount, FloodLight *floodLight)
{
    this->modeExecutors = modeExecutors;
    this->modesCount = modesCount;
    this->floodLight = floodLight;
}

void ModesChanger::loop()
{
    if (this->activeUntil == 0)
    {
        return;
    }

    if (this->activeUntil < millis())
    {
        this->floodLight->override(0, CRGB::Black, 0);

        this->activeUntil = 0;
    }
}

void ModesChanger::onClick()
{
    this->mode = (this->mode + 1) % this->modesCount;

    this->floodLight->override(this->timeout, this->modeExecutors[this->mode]->getModeSignatureColor().color0, 0, this->modeExecutors[this->mode]->getModeSignatureColor().color1, 0);

    this->activeUntil = millis() + this->timeout;
}

void ModesChanger::onLongPress()
{
    if (!this->isActive())
    {
        this->activeUntil = millis() + this->timeout;
        this->mode = this->selectedMode;

        this->floodLight->animateSync(CRGB::Black, CRGB::Black, 200, 1);
        this->floodLight->override(this->timeout, this->modeExecutors[this->mode]->getModeSignatureColor().color0, 0, this->modeExecutors[this->mode]->getModeSignatureColor().color1, 0);

        return;
    }

    this->modeExecutors[this->selectedMode]->exitMode();
    this->modeExecutors[this->mode]->enterMode();
    this->selectedMode = this->mode;
    
    this->floodLight->override(200, CRGB::Black, 0);

    this->activeUntil = 0;
}

uint8_t ModesChanger::getSelectedMode()
{
    return this->selectedMode;
}

bool ModesChanger::isActive()
{
    return this->activeUntil != 0;
}
