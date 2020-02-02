
#ifndef __MEMORYCONTROLLER_H__
#define __MEMORYCONTROLLER_H__

#define MEMORY_EEPROM_LO_BOUNDARY 0x10
#define MEMORY_EEPROM_HI_BOUNDARY 0x2F

#define MEMORY_NIGHTLIGHT_LAST_USED_COLOR_INDEX 0x10
#define MEMORY_GLOBAL_BRIGHTNESS_CAP 0x11

#include <Arduino.h>
#include <EEPROM.h>

class MemoryController
{
public:
    uint8_t getByte(uint8_t address);
    void setByte(uint8_t address, uint8_t value);

private:
    bool isInEEPROMAddressSpace(uint8_t address);
};

#endif
