#include "BLEModule.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>

// -----------------------------------------------
// Internal state
// -----------------------------------------------
#define TELEMETRY_INTERVAL_MS 1000   // send battery % every 1 second

static BLECharacteristic *pTxCharacteristic = nullptr;
static bool deviceConnected               = false;

// -----------------------------------------------
// Connection callbacks
// -----------------------------------------------
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

// -----------------------------------------------
// ble_begin  —  call once in setup()
// -----------------------------------------------
void ble_begin() {
    BLEDevice::init("ESP32_BAND");

    BLEServer  *server  = BLEDevice::createServer();
    server->setCallbacks(new BandServerCallbacks());

    BLEService *service = server->createService(SERVICE_UUID);

    // TX characteristic: band → phone (notify only, band never receives)
    pTxCharacteristic = service->createCharacteristic(
        TX_CHAR_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pTxCharacteristic->addDescriptor(new BLE2902());

    service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    Serial.println("[BLE] BLE started — device name: ESP32_BAND");
}

// -----------------------------------------------
// ble_update  —  call every loop()
// (advertising restart is handled in onDisconnect,
//  so nothing extra is needed here for now)
// -----------------------------------------------
void ble_update() {
    // Reserved for future use (e.g. pairing UI, status LED)
}

// -----------------------------------------------
// ble_sendData  —  sends battery % to phone
// Rate-limited to TELEMETRY_INTERVAL_MS
// -----------------------------------------------
void ble_sendData(float battery_percentage) {
    if (!deviceConnected || pTxCharacteristic == nullptr) {
        return;
    }

    static unsigned long lastSend = 0;
    unsigned long now = millis();

    if (now - lastSend < TELEMETRY_INTERVAL_MS) {
        return;
    }
    lastSend = now;

    // Format: "BATT:XX.X\n"
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "BATT:%.1f\n", battery_percentage);

    pTxCharacteristic->setValue((uint8_t*)buffer, strlen(buffer));
    pTxCharacteristic->notify();

    Serial.print("[BLE] TX -> ");
    Serial.print(buffer);
}

// -----------------------------------------------
// ble_isConnected
// -----------------------------------------------
bool ble_isConnected() {
    return deviceConnected;
}