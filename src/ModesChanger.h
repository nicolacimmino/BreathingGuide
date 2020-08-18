#ifndef __MODESCHANGER_H__
#define __MODESCHANGER_H__

#include <Arduino.h>
#include "Devices/FloodLight.h"
#include "ModeExecutors/ModeExecutor.h"

class ModesChanger
{
public:
    void begin(ModeExecutor **modeExecutors, uint8_t modesCount, FloodLight *floodLight);
    void loop();
    void onClick();
    void onLongPress();
    uint8_t getSelectedMode();
    bool isActive();

private:
    FloodLight *floodLight;
    uint8_t mode = 0;
    uint8_t selectedMode = 0;
    uint8_t modesCount = 0;
    uint32_t activeUntil = 0;
    uint16_t timeout = 4000;
    ModeExecutor **modeExecutors;
};

#endif
