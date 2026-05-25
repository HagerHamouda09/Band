#ifndef BLEMODULE_H
#define BLEMODULE_H

#include <Arduino.h>

#define SERVICE_UUID  "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_CHAR_UUID  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   
#define RX_CHAR_UUID  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   

void ble_begin();
void ble_update();                             
void ble_sendData(float battery_percentage, float bpm , int32_t spO2   );   
void ble_send_systemcheck(bool passed);
void ble_send_exceeded_trials();
void ble_handleCommand(char cmd);
bool ble_isConnected();
void ble_send_maximumTrials();

#endif