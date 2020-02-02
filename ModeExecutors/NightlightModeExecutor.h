#ifndef __NIGHTLIGHTMODEEXECUTOR_H__
#define __NIGHTLIGHTMODEEXECUTOR_H__

#include "ModeExecutor.h"

#define NIGHTLIGHT_COLORS_COUNT 6

class NighlightModeExecutor : public ModeExecutor
{

public:
    ColorsTuple getModeSignatureColor();

protected:
    void doLoop();
    void doEnterMode();
    void doExitMode();
    void doOnShake();
    void doOnClick();

private:
    int8_t lastX;
    int8_t lastY;
    int8_t lastZ;
    uint32_t activeUntil = 0;
    uint8_t fade = 250;
    bool latched = false;
    void refreshActiveUntil();
    CRGB getCurrentColor();
    CRGB lightColors[NIGHTLIGHT_COLORS_COUNT] = {
        CRGB::CRGB(CHSV::CHSV(42, 200, 255)),  // Yellow
        CRGB::CRGB(CHSV::CHSV(85, 255, 255)),  // Green
        CRGB::CRGB(CHSV::CHSV(128, 255, 255)), // Aqua
        CRGB::CRGB(CHSV::CHSV(171, 200, 255)), // Blue
        CRGB::CRGB(CHSV::CHSV(213, 200, 255)), // Purple
        CRGB::CRGB(CHSV::CHSV(0, 255, 255))    // Red
    };
    uint8_t currentColorIndex = 0;
};

#endif