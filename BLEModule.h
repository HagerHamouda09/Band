#ifndef BLEMODULE_H
#define BLEMODULE_H

#include <Arduino.h>

// -----------------------------------------------
// UUIDs  (Nordic UART Service style)
// TX = band sends data TO the phone (notify)
// -----------------------------------------------
#define SERVICE_UUID  "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define TX_CHAR_UUID  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   // ESP32 → phone (notify)

// -----------------------------------------------
// Functions
// -----------------------------------------------
void ble_begin();
void ble_update();                              // call every loop — handles reconnect
void ble_sendData(float battery_percentage);    // sends battery % to phone

// Connection status
bool ble_isConnected();

#endif