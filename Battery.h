#ifndef BATTERY_H
#define BATTERY_H

#include "config.h"

// Functions
void battery_begin();
void battery_update();          // reads voltage and calculates %
float battery_getVoltage();
float battery_getPercentage();

// Global variables (internal use)
extern float battery_voltage;
extern float battery_percentage;

#endif