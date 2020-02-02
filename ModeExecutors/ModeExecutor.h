#ifndef __MODEEXECUTOR_H__
#define __MODEEXECUTOR_H__

#include <Arduino.h>
#include "FloodLight.h"
#include "LEDBarController.h"
#include "Accelerometer.h"
#include "MemoryController.h"

class ModeExecutor
{
public:
    void begin(FloodLight *floodLight, LEDBarController *ledBarController, Accelerometer *accelerometer, MemoryController *memoryController);
    void loop();
    void enterMode();
    void exitMode();
    virtual void enterDemoMode();
    void onTilt(uint8_t axis, bool positive);
    void onShake();
    void onClick();
    virtual struct ColorsTuple getModeSignatureColor();

protected:
    virtual void doLoop();
    virtual void doEnterMode();
    virtual void doExitMode();
    virtual void doOnTilt(uint8_t axis, bool positive);
    virtual void doOnShake();
    virtual void doOnClick();
    uint32_t getTimeSinceModeChange();
    void inhibitLoopFor(uint32_t milliseconds);
    void updateElapsedTimeBar(uint32_t dotDurationmS);
    FloodLight *floodLight;
    LEDBarController *ledBarController;
    Accelerometer *accelerometer;
    MemoryController *memoryController;
    bool exerciseEnded = false;
    void resetModeChangeTime();

private:
    uint32_t modeChangeTime;
    bool modeActive = false;
    uint32_t inhibitLoopUntil = 0;
};

struct ColorsTuple
{
    CRGB color0;
    CRGB color1;
};

#endif