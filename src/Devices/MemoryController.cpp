
#include "MemoryController.h"

uint8_t MemoryController::getByte(uint8_t address)
{
    if (this->isInEEPROMAddressSpace(address))
    {
        return EEPROM.read(address - MEMORY_EEPROM_LO_BOUNDARY);
    }

    return 0;
}

void MemoryController::setByte(uint8_t address, uint8_t value)
{
    if (this->isInEEPROMAddressSpace(address))
    {
        EEPROM.write(address - MEMORY_EEPROM_LO_BOUNDARY, value);
    }

    return;
}

bool MemoryController::isInEEPROMAddressSpace(uint8_t address)
{
    return (address >= MEMORY_EEPROM_LO_BOUNDARY && address <= MEMORY_EEPROM_HI_BOUNDARY);
}