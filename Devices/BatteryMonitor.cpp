#include "BatteryMonitor.h"

void BatteryMonitor::begin(void (*onLowBattery)(), void (*onBatteryCritical)())
{
    this->onLowBattery = onLowBattery;
    this->onBatteryCritical = onBatteryCritical;
}

void BatteryMonitor::loop()
{
    if (millis() - this->lastCheckTime < 3000)
    {
        return;
    }

    // See this article for an in-depth explanation.
    // https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
    // tl;dr: we switch the ADC to measure the internal 1.1v reference using Vcc as reference, the rest is simple math.

    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    delay(2);
    ADCSRA |= _BV(ADSC);
    while (bit_is_set(ADCSRA, ADSC))
        ;

    long measuredVcc = 1125300L / (ADCL | (ADCH << 8));

    analogReference(DEFAULT);

    if (measuredVcc < 3000 && onLowBattery != NULL)
    {
        this->onLowBattery();
    }

    if (measuredVcc < 2700 && onBatteryCritical != NULL)
    {
        this->onBatteryCritical();
    }

    this->lastCheckTime = millis();
}