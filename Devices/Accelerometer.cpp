
#include "Accelerometer.h"

void Accelerometer::begin(uint8_t pinXAxes, uint8_t pinYAxes, uint8_t pinZAxes,
                          void (*onTilt)(uint8_t axis, bool positive) = NULL, void (*onShake)() = NULL, bool (*onRoll)(char *pattern) = NULL)
{
    this->pinAxes[X_AXIS] = pinXAxes;
    this->pinAxes[Y_AXIS] = pinYAxes;
    this->pinAxes[Z_AXIS] = pinZAxes;

    pinMode(pinXAxes, INPUT);
    pinMode(pinYAxes, INPUT);
    pinMode(pinZAxes, INPUT);

    this->onTilt = onTilt;
    this->onShake = onShake;
    this->onRoll = onRoll;
}

void Accelerometer::loop()
{
    if (millis() - this->movingAverageSampleTime > 100)
    {
        this->movingAverageSampleTime = millis();

        for (int ix = 0; ix < 3; ix++)
        {
            int16_t axisTilt = this->getAxis(ix);

            this->averagedAxis[ix] = ((float)this->averagedAxis[ix] * 0.9) + axisTilt * 0.1;

            this->senseAxisTiltMotion(ix, axisTilt);
            this->senseAxisShakeMotion(ix);
        }
    }
}

void Accelerometer::senseAxisTiltMotion(uint8_t axis, int16_t axisTilt)
{
    if (this->onTilt[axis] == NULL)
    {
        return;
    }

    if (abs(axisTilt) > 96 && this->axesStatus[axis] == AXIS_LEVEL)
    {
        this->axesStatus[axis] = axisTilt > 0 ? AXIS_POSITIVE_TILT : AXIS_NEGATIVE_TILT;
        this->onTilt(axis, axisTilt > 0);

        if (millis() - this->lastRollTime > 2000)
        {
            memset(this->rollPattern, 0, ROLL_PATTERN_BUFFER_SIZE);
        }

        for (int ix = ROLL_PATTERN_BUFFER_SIZE - 1; ix > 0; ix--)
        {
            this->rollPattern[ix] = this->rollPattern[ix - 1];
        }

        this->rollPattern[0] = "XYZ"[axis];
        this->lastRollTime = millis();

        if (strlen(this->rollPattern) > 4)
        {
            bool patternRecognised = this->onRoll(this->rollPattern);

            if (patternRecognised)
            {
                memset(this->rollPattern, 0, ROLL_PATTERN_BUFFER_SIZE);
            }
        }
    }
    else if (axisTilt < 32 && axisTilt > -32 && this->axesStatus[axis] != AXIS_LEVEL)
    {
        this->axesStatus[axis] = AXIS_LEVEL;
    }
}

void Accelerometer::senseAxisShakeMotion(uint8_t axis)
{
    if (this->onShake == NULL || millis() - this->lastShakeTime < 500)
    {
        return;
    }

    int16_t acc = analogRead(this->pinAxes[axis]);

    if ((acc < this->calibationTableAxisShake[axis * 2] || acc > this->calibationTableAxisShake[1 + (axis * 2)]))
    {
        this->onShake();
        this->lastShakeTime = millis();
    }
}

int16_t Accelerometer::getX()
{
    return this->getAxis(X_AXIS);
}

int16_t Accelerometer::getY()
{
    return this->getAxis(Y_AXIS);
}

int16_t Accelerometer::getZ()
{
    return this->getAxis(Z_AXIS);
}

int16_t Accelerometer::getAveragedX()
{
    return this->averagedAxis[X_AXIS];
}

int16_t Accelerometer::getAveragedY()
{
    return this->averagedAxis[Y_AXIS];
}

int16_t Accelerometer::getAveragedZ()
{
    return this->averagedAxis[Z_AXIS];
}

int16_t Accelerometer::getAxis(uint8_t axis)
{
    int16_t acc = analogRead(this->pinAxes[axis]);

    int16_t calMin = this->calibationTable[axis * 2];
    int16_t calMax = this->calibationTable[(axis * 2) + 1];

    acc = min(acc, calMax);
    acc = max(acc, calMin);

    return ((acc - calMin) / ((calMax - calMin) / 250.0)) - 127;
}
