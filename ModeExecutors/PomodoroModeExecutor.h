#ifndef __POMODOROMODEEXECUTOR_H__
#define __POMODOROMODEEXECUTOR_H__

#include "ModeExecutor.h"

#define POMODORO_GOTOWORK 0
#define POMODORO_WORK 1
#define POMODORO_WORKDONE 2
#define POMODORO_BREAK 3

class PomodoroModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnShake();
    void doOnTilt(uint8_t axis, bool positive);
    void enterDemoMode();
    
private:
    void goToWorkPattern();
    void breakPattern();
    void workPattern();
    void workDonePattern();
    uint16_t pomodoroDurationSeconds = 10;
    uint16_t pomodoroBreakDurationSeconds = 10;
    uint16_t pomodoroWarningSecondsPerLight = 1;
    uint16_t getPomodoroTimeSeconds();
    uint16_t getPomodoroBreakTimeSeconds();
    uint8_t pomodoroState = POMODORO_GOTOWORK;
    uint32_t breakStartTime = 0;
    uint32_t lastShakeTime = 0;
    uint8_t shakeCount = 0;
};

#endif