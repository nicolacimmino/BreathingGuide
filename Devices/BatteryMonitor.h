#ifndef __BATTERYMOMITOR_H__
#define __BATTERYMOMITOR_H__

#include <Arduino.h>

class BatteryMonitor
{
public:
    void begin(void (*onLowBattery)(), void (*onBatteryCritical)());
    void loop();

private:
    void (*onLowBattery)();
    void (*onBatteryCritical)();
    uint32_t lastCheckTime = 0;
};

#endif