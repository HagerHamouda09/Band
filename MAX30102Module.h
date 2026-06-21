#ifndef MAX30102MODULE_H
#define MAX30102MODULE_H

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "config.h"

void max30102_init();
void max30102_update();
float max30102_getBPM();
int32_t max30102_getSpo2();
bool maxReady();
bool exceededTrials();
bool maximumTrials();

#endif