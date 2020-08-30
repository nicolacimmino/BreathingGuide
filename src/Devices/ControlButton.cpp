#include "ControlButton.h"

void ControlButton::begin(uint8_t pinTouch, void (*onClick)(), void (*onLongPress)())
{
    this->pinTouch = pinTouch;

    this->onClick = onClick;
    this->onLongPress = onLongPress;
}

void ControlButton::loop()
{
    if (this->inhibitUntilReleased && this->isButtonPressed())
    {
        return;
    }

    this->inhibitUntilReleased = false;

    this->scanButton();
}

void ControlButton::scanButton()
{
    if (!this->isButtonPressed())
    {
        return;
    }
   
    // Wait for the switch to be released or a timeout of 500mS to expire.
    unsigned long initialTime = millis();
    while ((millis() - initialTime < 500) && this->isButtonPressed())
    {
        delay(1);
    }

    if (this->isButtonPressed())
    {
        if (this->onLongPress != NULL)
        {
            this->onLongPress();
            this->inhibitUntilReleased = true;
        }
    }
    else
    {
        if (this->onClick != NULL)
        {
            this->onClick();
        }
    }
}

bool ControlButton::isButtonPressed()
{    
    int touchSensor = ADCTouch.read(this->pinTouch);
    return touchSensor > 650                        // When plugged in and touching
     || (touchSensor < 100 && touchSensor > 40);     // Not pluggged touching     
}

