#ifndef BATTERY_H
#define BATTERY_H

#include "config.h"

void battery_begin();
void battery_update();         
float battery_getVoltage();
float battery_getPercentage();
bool batteryIsDrained();

extern float battery_voltage;
extern float battery_percentage;

#endif
