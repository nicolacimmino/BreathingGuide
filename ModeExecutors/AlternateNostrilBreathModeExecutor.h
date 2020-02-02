#ifndef __ALTERNATENOSTRILBREATHMODEEXECUTOR_H__
#define __ALTERNATENOSTRILBREATHMODEEXECUTOR_H__

#include "ModeExecutor.h"

class AlternateNostrilBreathModeExecutor : public ModeExecutor
{

public:
    struct ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();

private:
    void breatheFloodLight();
    bool leftNostril = true;
};

#endif