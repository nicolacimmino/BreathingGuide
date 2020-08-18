#ifndef __BRIGHTNESSCHANGER_H__
#define __BRIGHTNESSCHANGER_H__

#include <Arduino.h>
#include "Devices/FloodLight.h"
#include "Devices/MemoryController.h"
#include "Devices/Accelerometer.h"

class BrightnessChanger
{
public:
    void begin(FloodLight *floodLight, MemoryController *memoryController, Accelerometer *accelerometer);
    void loop();
    void onClick();
    void onLongPress();
    bool isActive();

private:
    FloodLight *floodLight;
    uint8_t brightness = 50;
    uint32_t activeUntil = 0;
    uint16_t timeout = 5000;
    MemoryController *memoryController;
    Accelerometer *accelerometer;
};

#endif