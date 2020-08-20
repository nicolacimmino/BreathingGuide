
#include "Accelerometer.h"

void Accelerometer::begin(void (*onTilt)(uint8_t axis, bool positive) = NULL, void (*onShake)() = NULL, bool (*onRoll)(char *pattern) = NULL)
{
    this->mpu = new Adafruit_MPU6050();
    
    this->mpu->begin();
    
    this->mpu->setAccelerometerRange(MPU6050_RANGE_2_G);
    this->mpu->setGyroRange(MPU6050_RANGE_500_DEG);
    this->mpu->setFilterBandwidth(MPU6050_BAND_21_HZ);

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

    int16_t acc = this->getAxis(axis);

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
    sensors_event_t a, g, temp;
    this->mpu->getEvent(&a, &g, &temp);

    float value = 0;

    switch (axis)
    {
    case X_AXIS:
        value = a.acceleration.y;
        break;
    case Y_AXIS:
        value = a.acceleration.x;
        break;
    case Z_AXIS:
        value = a.acceleration.z;
        break;
    }

    float calMin = this->calibationTable[axis * 2];
    float calMax = this->calibationTable[(axis * 2) + 1];

    float acc = min(value, calMax);
    acc = max(value, calMin);

    int16_t acceleration = ((acc - calMin) / ((calMax - calMin) / 250.0)) - 127;

    Serial.print("X: ");
    Serial.println(a.acceleration.x);
    Serial.print("Y: ");
    Serial.println(a.acceleration.y);
    Serial.print("Z: ");
    Serial.println(a.acceleration.z);

    return acceleration;
}
