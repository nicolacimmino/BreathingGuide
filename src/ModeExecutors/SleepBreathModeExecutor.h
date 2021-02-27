#ifndef __SLEEPBREATHMODEEXECUTOR_H__
#define __SLEEPBREATHMODEEXECUTOR_H__

#include "ModeExecutor.h"
#include <avr/sleep.h>

class SleepBreathModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnClick();

private:
    void breatheFloodLight();
    unsigned long getEffectiveSideDuration();
    int8_t offset = 0;
};

#endif