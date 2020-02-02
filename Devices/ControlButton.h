#ifndef __CONTROLBUTTON_H__
#define __CONTROLBUTTON_H__

#include <Arduino.h>

class ControlButton
{
public:
    void begin(uint8_t pinButton, void (*onClick)(), void (*onLongPress)());
    void loop();
    bool isButtonPressed();
    
private:
    uint8_t pinButton;
    void (*onClick)();
    void (*onLongPress)();
    void scanButton();
    bool inhibitUntilReleased = false;
};

#endif
