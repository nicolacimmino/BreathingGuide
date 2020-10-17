
//  Copyright (C) 2019 Nicola Cimmino
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "src/hardware.h"
#include "src/Devices/Accelerometer.h"
#include "src/Devices/LEDBarController.h"
#include "src/Devices/ControlButton.h"
#include "src/Devices/FloodLight.h"
#include "src/Devices/BatteryMonitor.h"

#define EXECUTORS_COUNT 5
#include "src/ModeExecutors/ModeExecutor.h"
#include "src/ModeExecutors/SquareBreathModeExecutor.h"
#include "src/ModeExecutors/AlternateNostrilBreathModeExecutor.h"
#include "src/ModeExecutors/RainbowModeExecutor.h"
#include "src/ModeExecutors/PomodoroModeExecutor.h"
#include "src/ModeExecutors/NightlightModeExecutor.h"
#include "src/ModesChanger.h"
#include "src/Devices/MemoryController.h"
#include "src/BrightnessChanger.h"
#include "src/Cli.h"

#include <avr/sleep.h>

Accelerometer accelerometer;
LEDBarController ledBarController;
ControlButton controlButton;
FloodLight floodLight;
BatteryMonitor BatteryMonitor;
MemoryController memoryController;
Cli cli;

uint8_t mode = 0;
ModeExecutor *modeExecutors[] = {new SquareBreathModeExecutor(), new AlternateNostrilBreathModeExecutor(), new RainbowModeExecutor(), new PomodoroModeExecutor(), new NighlightModeExecutor()};

ModesChanger modesChanger;
BrightnessChanger brightnessChanger;

void onClick()
{
    if (modesChanger.isActive())
    {
        modesChanger.onClick();
        return;
    }

    if (brightnessChanger.isActive())
    {
        brightnessChanger.onClick();
        return;
    }

    modeExecutors[modesChanger.getSelectedMode()]->onClick();
}

void onLongPress()
{
    if (brightnessChanger.isActive())
    {
        brightnessChanger.onLongPress();
        return;
    }

    modesChanger.onLongPress();
}

void onTilt(uint8_t axis, bool positive)
{
    modeExecutors[modesChanger.getSelectedMode()]->onTilt(axis, positive);
}

void onShake()
{
    modeExecutors[modesChanger.getSelectedMode()]->onShake();
}

bool onRoll(char *pattern)
{
    if (strstr(pattern, "ZXZXZYZY") == pattern)
    {
        floodLight.animateSync(CRGB::Red, CRGB::Yellow, 200, 10);

        modeExecutors[modesChanger.getSelectedMode()]->enterDemoMode();

        return true;
    }

    return false;
}

void onLowBattery()
{
    floodLight.override(300, CRGB::Red, 0);
}

void onBatteryCritical()
{
    ledBarController.showBar(0);
    floodLight.animateSync(CRGB::Red, CRGB::Black, 200, 10);
    floodLight.shutdown();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    sleep_disable();
}

void setup()
{

    Serial.begin(115200);
    cli.begin(&Serial, &memoryController);

    controlButton.begin(PIN_BTN, &onClick, &onLongPress);

    uint8_t ledPins[] = {PIN_LED_G0, PIN_LED_G1, PIN_LED_G2, PIN_LED_G3};
    ledBarController.begin(ledPins);

    floodLight.begin();
    floodLight.setBrightness(controlButton.isButtonPressed() ? 50 : memoryController.getByte(MEMORY_GLOBAL_BRIGHTNESS_CAP));
    
    accelerometer.begin(onTilt, onShake, onRoll);

    for (int ix = 0; ix < EXECUTORS_COUNT; ix++)
    {
        modeExecutors[ix]->begin(&floodLight, &ledBarController, &accelerometer, &memoryController);
    }
    modeExecutors[0]->enterMode();

    BatteryMonitor.begin(onLowBattery, onBatteryCritical);

    modesChanger.begin(modeExecutors, EXECUTORS_COUNT, &floodLight);

    if (controlButton.isButtonPressed())
    {        
        brightnessChanger.begin(&floodLight, &memoryController, &accelerometer);
        while (controlButton.isButtonPressed())
        {
            //
        }
    }    
}

void loop()
{
    cli.loop();
    accelerometer.loop();
    controlButton.loop();
    modesChanger.loop();
    brightnessChanger.loop();

    if (!modesChanger.isActive() && !brightnessChanger.isActive())
    {
        for (int ix = 0; ix < EXECUTORS_COUNT; ix++)
        {
            modeExecutors[ix]->loop();
        }
    }

    ledBarController.loop();
    BatteryMonitor.loop();
    floodLight.loop();
}
