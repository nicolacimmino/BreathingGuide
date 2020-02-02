#include "ControlButton.h"

void ControlButton::begin(uint8_t pinButton, void (*onClick)(), void (*onLongPress)())
{
    this->pinButton = pinButton;

    pinMode(pinButton, INPUT_PULLUP);

    this->onClick = onClick;
    this->onLongPress = onLongPress;
}

void ControlButton::loop()
{
    if (this->inhibitUntilReleased && digitalRead(this->pinButton) == 0)
    {
        return;
    }

    this->inhibitUntilReleased = false;

    this->scanButton();
}

void ControlButton::scanButton()
{
    if (digitalRead(this->pinButton) != 0)
    {
        return;
    }

    // Allow the switch to stabilise.
    byte debounce = 0x55;
    while (debounce != 0x00)
    {
        debounce = (debounce << 1) | (digitalRead(this->pinButton) & 1);
        delay(1);
    }

    // Wait for the switch to be released or a timeout of 500mS to expire.
    unsigned long initialTime = millis();
    while ((millis() - initialTime < 500) && digitalRead(this->pinButton) == 0)
    {
        delay(1);
    }

    if (digitalRead(this->pinButton) == 0)
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
    return digitalRead(this->pinButton) == LOW;
}