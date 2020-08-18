#ifndef __FLOODLIGHT_H__
#define __FLOODLIGHT_H__

#include <Arduino.h>
#include <FastLED.h>

class FloodLight
{
public:
    void begin();
    void loop();
    void setColor(CRGB color);
    void setColor(CRGB color0, CRGB color1);
    void setBrightness(uint8_t brightness);
    void setFade(uint8_t fade);
    void setFade(uint8_t fade0, uint8_t fade1);
    void override(uint16_t timeMilliseconds, CRGB color0, uint8_t fade0, CRGB color1, uint8_t fade1);
    void override(uint16_t timeMilliseconds, CRGB color0, uint8_t fade0);
    void animateSync(CRGB color0, CRGB color1, uint16_t interval, uint8_t iterations);
    void shutdown();

private:
    CRGB leds[2];
    uint8_t fade[2] = {0, 0};
    CRGB colors[2];
    CRGB overrideColor[2];
    uint8_t overrideFade[2];
    uint32_t overrideUntil = 0;
};

#endif