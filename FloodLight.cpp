
#include "FloodLight.h"

void FloodLight::begin()
{
    FastLED.addLeds<WS2812B, 4, GRB>(this->leds, 2);
}

void FloodLight::setColor(CRGB color)
{
    this->colors[0] = color;
    this->colors[1] = color;
}

void FloodLight::setColor(CRGB color0, CRGB color1)
{
    this->colors[0] = color0;
    this->colors[1] = color1;
}

void FloodLight::setBrightness(uint8_t brightness)
{
    FastLED.setBrightness(brightness);
}

void FloodLight::setFade(uint8_t fade)
{
    this->setFade(fade, fade);
}

void FloodLight::setFade(uint8_t fade0, uint8_t fade1)
{
    this->fade[0] = fade0;
    this->fade[1] = fade1;
}

void FloodLight::loop()
{
    bool override = millis() < this->overrideUntil;

    this->leds[0] = override ? this->overrideColor[0] : this->colors[0];
    this->leds[0].fadeLightBy(override ? this->overrideFade[0] : this->fade[0]);

    this->leds[1] = override ? this->overrideColor[1] : this->colors[1];
    this->leds[1].fadeLightBy(override ? this->overrideFade[1] : this->fade[1]);

    FastLED.show();
}

void FloodLight::override(uint16_t timeMilliseconds, CRGB color0, uint8_t fade0)
{
    this->override(timeMilliseconds, color0, fade0, color0, fade0);
}

void FloodLight::override(uint16_t timeMilliseconds, CRGB color0, uint8_t fade0, CRGB color1, uint8_t fade1)
{
    this->overrideColor[0] = color0;
    this->overrideColor[1] = color1;
    this->overrideFade[0] = fade0;
    this->overrideFade[1] = fade1;
    this->overrideUntil = millis() + timeMilliseconds;
}