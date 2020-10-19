#ifndef __DESKJOBMODEEXECUTOR_H__
#define __DESKJOBMODEEXECUTOR_H__

#include "ModeExecutor.h"

#define CRONS_COUNT 3
#define WATER_INTERVAL_MINUTES 15
#define WATER_OFFSET_MINUTES 15
#define WALK_INTERVAL_MINUTES 60
#define WALK_OFFSET_MINUTES 35
#define CALISTENICS_INTERVAL_MINUTES 120
#define CALISTENICS_OFFSET_MINUTES 65

class DeskJobModeExecutor : public ModeExecutor
{
public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnShake();
    void doOnTilt(uint8_t axis, bool positive);

private:
    typedef struct
    {
        uint8_t intervalMinutes;
        uint8_t offsetMinutes;
        CRGB color;
    } cronEntry;

    cronEntry cron[CRONS_COUNT] = {
        {WATER_INTERVAL_MINUTES, WATER_OFFSET_MINUTES, CRGB::Blue},
        {WALK_INTERVAL_MINUTES, WALK_OFFSET_MINUTES, CRGB::Yellow},
        {CALISTENICS_INTERVAL_MINUTES, CALISTENICS_OFFSET_MINUTES, CRGB::Purple},
    };

    bool alarmed[CRONS_COUNT] = {false, false, false};

    uint16_t getTimeMinutes();
    uint16_t getTimeSecondsInMinute();
};

#endif