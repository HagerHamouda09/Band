#include "BLEModule.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include "config.h"

#define TELEMETRY_INTERVAL_MS 1000

static BLECharacteristic *pTxCharacteristic = nullptr;
static BLECharacteristic *pRxCharacteristic = nullptr;   
static bool deviceConnected                 = false;
extern SYSTEM_CASES systemstate ;

class BandCharacteristicCallbacks : public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *characteristic) override {

        String value = characteristic->getValue();

        if (value.length() > 0) {

            char cmd = value[0];

            Serial.print("[BLE] RX Command: ");
            Serial.println(cmd);

            ble_handleCommand(cmd);
        }
    }
};


class BandServerCallbacks : public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
        Serial.println("[BLE] Phone connected");
    }

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
        Serial.println("[BLE] Phone disconnected — restarting advertising");
        BLEDevice::startAdvertising();
    }

};


void ble_begin() {
    BLEDevice::init("ESP32_BAND");

    BLEServer  *server  = BLEDevice::createServer();
    server->setCallbacks(new BandServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

  
    pTxCharacteristic = service->createCharacteristic(
        TX_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxCharacteristic->addDescriptor(new BLE2902());

   
    pRxCharacteristic = service->createCharacteristic(
        RX_CHAR_UUID,
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_WRITE_NR
    );
    pRxCharacteristic->setCallbacks(new BandCharacteristicCallbacks());

    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    Serial.println("[BLE] BLE started — device name: ESP32_BAND");
}


void ble_update() {
    
}


void ble_sendData(float battery_percentage, float bpm, int32_t spO2) {
    if (!deviceConnected || pTxCharacteristic == nullptr) {
        return;
    }

    static unsigned long lastSend = 0;
    unsigned long now = millis();

    if (now - lastSend < TELEMETRY_INTERVAL_MS) {
        return;
    }

    lastSend = now;

    char buffer[64];

    snprintf(
        buffer,
        sizeof(buffer),
        "BATT:%.1f\nspO2:%ld\nbpm:%.1f\n",
        battery_percentage,
        (long)spO2,
        bpm
    );

    pTxCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
    pTxCharacteristic->notify();

    Serial.print("[BLE] TX -> ");
    Serial.print(buffer);
}

void ble_send_systemcheck(bool passed) {

    if (!deviceConnected || pTxCharacteristic == nullptr) return;

    static unsigned long lastSend = 0;
    if (millis() - lastSend < 1000) return;  
    lastSend = millis();

    const char* msg = passed ? "PRECHECK:PASS\n" : "PRECHECK:FAIL\n";
    pTxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
    pTxCharacteristic->notify();
}

void ble_send_exceeded_trials(){
    if (!deviceConnected || pTxCharacteristic == nullptr) return;
    // static unsigned long lastSend = 0;
    // if (millis() - lastSend < 1000) return;  
    // lastSend = millis();

    const char* msg = "Driver wrong adjustment exceeded";
    pTxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
    pTxCharacteristic->notify();

}
void ble_send_maximumTrials()
{
    if (!deviceConnected || pTxCharacteristic == nullptr) return;
    // static unsigned long lastSend = 0;
    // if (millis() - lastSend < 1000) return;  
    // lastSend = millis();

    const char* msg = "driver Not wearing the band for too long , trip Ended";
    pTxCharacteristic->setValue((uint8_t*)msg, strlen(msg));
    pTxCharacteristic->notify();

}
void ble_handleCommand(char cmd) {

    switch (cmd) {

        case 'R':
            Serial.println("[BLE] Command: Ready");
            systemstate= SYSTEM_NORMAL;
            break;
        case 'E':
            Serial.println("[BLE] Command: END system");
            systemstate= SYSTEM_END;
            break;
        default:
            Serial.println("[BLE] Unknown command");
            break;
    }
}

bool ble_isConnected() {
    return deviceConnected;
}
