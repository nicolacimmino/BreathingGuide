#include "ModeExecutor.h"

void ModeExecutor::begin(FloodLight *floodLight, LEDBarController *ledBarController, Accelerometer *accelerometer, MemoryController *memoryController)
{
    this->floodLight = floodLight;
    this->ledBarController = ledBarController;
    this->accelerometer = accelerometer;
    this->memoryController = memoryController;
}

void ModeExecutor::enterMode()
{
    this->modeChangeTime = millis();
    this->modeActive = true;
    this->inhibitLoopUntil = 0;

    this->doEnterMode();
}

void ModeExecutor::exitMode()
{
    this->doExitMode();

    this->modeActive = false;
}

void ModeExecutor::loop()
{
    if (!this->modeActive || this->getTimeSinceModeChange() < this->inhibitLoopUntil)
    {
        return;
    }

    this->inhibitLoopUntil = 0;

    doLoop();
}

uint32_t ModeExecutor::getTimeSinceModeChange()
{
    return millis() - this->modeChangeTime;
}

void ModeExecutor::inhibitLoopFor(uint32_t milliseconds)
{
    this->inhibitLoopUntil = this->getTimeSinceModeChange() + milliseconds;
}

void ModeExecutor::updateElapsedTimeBar(uint32_t dotDurationmS)
{
    uint8_t elapsedBlocks = this->getTimeSinceModeChange() / (dotDurationmS);

    if (elapsedBlocks > 4)
    {
        this->exerciseEnded = true;
        return;
    }

    this->ledBarController->setDim();
    if (this->accelerometer->getAveragedY() < -90)
    {
        this->ledBarController->setFullBrightness();
    }

    this->ledBarController->showBar(elapsedBlocks);
}

void ModeExecutor::onTilt(uint8_t axis, bool positive)
{
    if (!this->modeActive)
    {
        return;
    }

    this->doOnTilt(axis, positive);
}

void ModeExecutor::doOnTilt(uint8_t axis, bool positive)
{
    // Default behavior do not react to tilt.
    // Override on executors that need this.
    return;
}

void ModeExecutor::onShake()
{
    if (!this->modeActive)
    {
        return;
    }

    this->doOnShake();
}

void ModeExecutor::doOnShake()
{
    // Default behavior do not react to shake.
    // Override on executors that need this.
    return;
}

void ModeExecutor::resetModeChangeTime()
{
    this->modeChangeTime = millis();
}

void ModeExecutor::enterDemoMode()
{
    // Default behaviour is not to do anything special in demo mode.
}

void ModeExecutor::onClick()
{
    this->doOnClick();
}

void ModeExecutor::doOnClick()
{
    // Default behaviour, ignore clicks.
}