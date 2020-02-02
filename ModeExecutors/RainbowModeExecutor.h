#ifndef __RAINBOWMODEEXECUTOR_H__
#define __RAINBOWMODEEXECUTOR_H__

#include "ModeExecutor.h"

class RainbowModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnShake();

private:
    uint8_t getColorComponent(int8_t axisReading);
    bool autoMode = false;
};

#endif