#ifndef BLEMODULE_H
#define BLEMODULE_H
#include <Arduino.h>

// Functions
void ble_begin();
void ble_update();
void ble_sendData(float bpm, int spo2, float temp, float voltage, float percentage);

#endif