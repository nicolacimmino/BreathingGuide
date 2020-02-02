
#include "PomodoroModeExecutor.h"

void PomodoroModeExecutor::doLoop()
{
    switch (this->pomodoroState)
    {
    case POMODORO_GOTOWORK:
        this->goToWorkPattern();
        break;
    case POMODORO_BREAK:
        this->breakPattern();
        break;
    case POMODORO_WORKDONE:
        this->workDonePattern();
        break;
    default:
        this->workPattern();
    }
}

void PomodoroModeExecutor::goToWorkPattern()
{
    this->floodLight->setColor(CRGB::DarkRed, CRGB::Green);
    this->floodLight->setFade(this->getTimeSinceModeChange() % 1000 < 200 ? 0 : 255);
}

void PomodoroModeExecutor::workDonePattern()
{
    this->floodLight->setColor(CRGB::Yellow, CRGB::Green);
    this->floodLight->setFade(this->getTimeSinceModeChange() % 1000 < 200 ? 0 : 255);
}

void PomodoroModeExecutor::breakPattern()
{
    if (this->getPomodoroBreakTimeSeconds() > this->pomodoroBreakDurationSeconds)
    {
        this->pomodoroState = POMODORO_GOTOWORK;
        return;
    }

    // Keep breathing! See Sean Voisen great post from which I grabbed the formula.
    // https://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
    float val = (exp(sin(millis() / 3000.0 * PI)) - 0.36787944) * 108.0;
    this->floodLight->setColor(CRGB::Yellow, CRGB::Green);
    this->floodLight->setFade(val, 255 - val);
}

void PomodoroModeExecutor::workPattern()
{
    if (this->getPomodoroTimeSeconds() > this->pomodoroDurationSeconds)
    {
        this->pomodoroState = POMODORO_WORKDONE;
        this->ledBarController->showBar(0);

        return;
    }

    uint8_t val = 255 * (this->getPomodoroTimeSeconds() / (float)this->pomodoroDurationSeconds);
    this->floodLight->setColor(CRGB::DarkRed, CRGB::Green);
    this->floodLight->setFade(255 - val, max(val, 127));

    if (this->pomodoroDurationSeconds - this->getPomodoroTimeSeconds() < this->pomodoroWarningSecondsPerLight * 4)
    {
        this->ledBarController->showBar(1 + ((this->pomodoroDurationSeconds - this->getPomodoroTimeSeconds()) / this->pomodoroWarningSecondsPerLight));
    }
}

uint16_t PomodoroModeExecutor::getPomodoroTimeSeconds()
{
    return (this->getTimeSinceModeChange() / 1000);
}

uint16_t PomodoroModeExecutor::getPomodoroBreakTimeSeconds()
{
    return (millis() - this->breakStartTime) / 1000;
}

void PomodoroModeExecutor::doEnterMode()
{
    this->pomodoroDurationSeconds = 1500;     // 25 min
    this->pomodoroBreakDurationSeconds = 300; // 5 min
    this->pomodoroWarningSecondsPerLight = 60;

    this->resetModeChangeTime();
    this->pomodoroState = POMODORO_GOTOWORK;
}

void PomodoroModeExecutor::enterDemoMode()
{
    this->pomodoroDurationSeconds = 30;
    this->pomodoroBreakDurationSeconds = 10;
    this->pomodoroWarningSecondsPerLight = 1;

    this->resetModeChangeTime();
    this->pomodoroState = POMODORO_GOTOWORK;
}

void PomodoroModeExecutor::doExitMode()
{
    this->floodLight->setColor(CRGB::Black);
}

ColorsTuple PomodoroModeExecutor::getModeSignatureColor()
{
    return {CRGB::Red, CRGB::Green};
}

void PomodoroModeExecutor::doOnShake()
{
    if (millis() - this->lastShakeTime > 3000)
    {
        this->shakeCount = 0;
    }

    this->shakeCount++;

    if (this->shakeCount == 3)
    {
        this->shakeCount = 0;
        this->pomodoroState = POMODORO_WORK;
        this->resetModeChangeTime();
    }

    this->lastShakeTime = millis();
}

void PomodoroModeExecutor::doOnTilt(uint8_t axis, bool positive)
{
    if (this->pomodoroState == POMODORO_WORKDONE && axis == Z_AXIS)
    {
        this->breakStartTime = millis();
        this->pomodoroState = POMODORO_BREAK;
    }

    if (this->pomodoroState == POMODORO_GOTOWORK && axis == Y_AXIS)
    {
        this->pomodoroState = POMODORO_WORK;
        this->resetModeChangeTime();
    }
}
