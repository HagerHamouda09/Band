#include "Battery.h"
#include <Arduino.h>


float battery_voltage = 0;
float battery_percentage = 0;

void battery_begin() {
    pinMode(BATTERY_INDICATOR, INPUT);
    battery_voltage = 0;
    battery_percentage = 0;
}

void battery_update() {
    int adcValue = analogRead(BATTERY_INDICATOR);     
    battery_voltage = (adcValue / 4095.0) * 2.1*2;   
    battery_percentage = (battery_voltage - 3.0 ) / (4.2 - 3.0) * 100;
    battery_percentage = constrain(battery_percentage, 0, 100);
}

float battery_getVoltage() {
    return battery_voltage;
}

float battery_getPercentage() {
    return battery_percentage;
}

bool  batteryIsDrained(){
    return battery_percentage<=10?true:false;
}