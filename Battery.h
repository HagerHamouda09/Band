#ifndef BATTERY_H
#define BATTERY_H

#include "config.h"

class BatteryModule {
public:
    void begin();
    void readVoltage();
    float getVoltage();

private:
    float voltage;
};

extern BatteryModule Battery;

#endif