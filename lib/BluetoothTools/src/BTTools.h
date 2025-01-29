#ifndef BTTOOLS_H
#define BTTOOLS_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLEClient.h>

// class BTTools {
//     public:
//         BTTools();
//         void advertise();
//         void scan();
//         void read();
// };

class ESP32_BLE_Multi {
    private:
        // Constants
        const char* DEVICE_NAME;
        const char* SERVICE_UUID;
        const char* CHARACTERISTIC_UUID;
        const int SCAN_TIME;

        // BLE objects
        BLEScan* pBLEScan;
        BLEServer* pServer;
        BLEClient* pClient;
        BLEAdvertising* pAdvertising;
        BLEService* pService;
        BLECharacteristic* pCharacteristic;

        // State variables
        bool deviceConnected;
        int currentMode; // -1: Not initialized, 0: Scanner, 1: Server, 2: Client

        // Forward declarations of private nested classes
        class AdvertisedDeviceCallbacks;
        class ServerCallbacks;

        // Private methods
        void cleanupCurrentMode();
        void initBLE();

    public:
        // Constructor and destructor
        ESP32_BLE_Multi(
            const char* deviceName = "ESP32_MULTI_MODE",
            const char* serviceUUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b",
            const char* charUUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
        );
        ~ESP32_BLE_Multi();

        // Mode control methods
        bool startScanning();
        bool startServer();
        bool startClient();

        // Getters
        int getCurrentMode() const;
        bool isDeviceConnected() const;

        // Characteristic operations
        bool updateCharacteristicValue(const std::string& value);

        // Resource management
        void end();
};

#endif // ESP32_BLE_MULTI_H

