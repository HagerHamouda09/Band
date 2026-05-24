#ifndef CONFIG_H
#define CONFIG_H

// I2C Pins
#define SDA_PIN 3
#define SCL_PIN 4

// MAX30102
#define MAX_INT 5
#define LED_BRIGHTNESS 0xFF
//8 would be better for average as the wrist moves when driving 
//So having the average each 8 reduces the effect
#define SAMPLE_AVERAGE 4
#define LED_MODE 2
#define SAMPLE_RATE 100
#define PULSE_WIDTH 411
#define ADC_RANGE 16384

// Battery
#define BATTERY_INDICATOR 1

// BLE
// #define BLE_COMMUNICATION 7

#endif