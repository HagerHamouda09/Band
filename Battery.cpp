#include "Battery.h"
#include <Arduino.h>

// Global variables
float battery_voltage = 0;
float battery_percentage = 0;

void battery_begin() {
    pinMode(BATTERY_INDICATOR, INPUT);
    battery_voltage = 0;
    battery_percentage = 0;
}

void battery_update() {
    int adcValue = analogRead(BATTERY_INDICATOR);      // read ADC
    battery_voltage = (adcValue / 4095.0) * 3.3 * 2;   // adjust factor for voltage divider
    battery_percentage = (battery_voltage - 3.3) / (4.2 - 3.3) * 100;
    battery_percentage = constrain(battery_percentage, 0, 100);
}

float battery_getVoltage() {
    return battery_voltage;
}

float battery_getPercentage() {
    return battery_percentage;
}