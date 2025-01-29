#include "BTTools.h"

// Private callback class implementations
class ESP32_BLE_Multi::AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        Serial.printf("Device found: %s \n", advertisedDevice.toString().c_str());
    }
};

class ESP32_BLE_Multi::ServerCallbacks : public BLEServerCallbacks {
    ESP32_BLE_Multi* parent;
    public:
        ServerCallbacks(ESP32_BLE_Multi* _parent) : parent(_parent) {}
        
        void onConnect(BLEServer* pServer) {
            parent->deviceConnected = true;
            Serial.println("Device connected to server");
        }

        void onDisconnect(BLEServer* pServer) {
            parent->deviceConnected = false;
            Serial.println("Device disconnected from server");
        }
};

// Constructor
ESP32_BLE_Multi::ESP32_BLE_Multi(
    const char* deviceName,
    const char* serviceUUID,
    const char* charUUID
) : DEVICE_NAME(deviceName)
  , SERVICE_UUID(serviceUUID)
  , CHARACTERISTIC_UUID(charUUID)
  , SCAN_TIME(5)
  , pBLEScan(nullptr)
  , pServer(nullptr)
  , pClient(nullptr)
  , pAdvertising(nullptr)
  , pService(nullptr)
  , pCharacteristic(nullptr)
  , deviceConnected(false)
  , currentMode(-1) {
    Serial.begin(115200);
    Serial.println("ESP32 BLE Multi-Mode Device Initialized");
}

// Destructor
ESP32_BLE_Multi::~ESP32_BLE_Multi() {
    end();
}

void ESP32_BLE_Multi::cleanupCurrentMode() {
    if (pAdvertising) {
        pAdvertising->stop();
    }
    if (pServer) {
        pServer->getAdvertising()->stop();
    }
    if (pBLEScan) {
        pBLEScan->stop();
    }
    BLEDevice::deinit();
    delay(500); // Give some time for cleanup
}

void ESP32_BLE_Multi::initBLE() {
    BLEDevice::init(DEVICE_NAME);
    Serial.println("BLE Device initialized");
}

bool ESP32_BLE_Multi::startScanning() {
    try {
        if (currentMode != 0) {
            cleanupCurrentMode();
            initBLE();
        }

        Serial.println("Starting Scanner Mode");
        pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
        pBLEScan->setActiveScan(true);
        pBLEScan->setInterval(100);
        pBLEScan->setWindow(99);
        
        BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME, false);
        Serial.printf("Devices found: %d\n", foundDevices.getCount());
        pBLEScan->clearResults();
        currentMode = 0;
        return true;
    } catch (...) {
        Serial.println("Error starting scanner mode");
        return false;
    }
}

bool ESP32_BLE_Multi::startServer() {
    try {
        if (currentMode != 1) {
            cleanupCurrentMode();
            initBLE();
        }

        Serial.println("Starting Server Mode");
        pServer = BLEDevice::createServer();
        pServer->setCallbacks(new ServerCallbacks(this));

        pService = pServer->createService(SERVICE_UUID);
        pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE
        );

        pCharacteristic->setValue("Hello from ESP32 Server");
        pService->start();

        pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->addServiceUUID(SERVICE_UUID);
        pAdvertising->setScanResponse(true);
        pAdvertising->setMinPreferred(0x06);
        pAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();
        
        Serial.println("Server is advertising...");
        currentMode = 1;
        return true;
    } catch (...) {
        Serial.println("Error starting server mode");
        return false;
    }
}

bool ESP32_BLE_Multi::startClient() {
    try {
        if (currentMode != 2) {
            cleanupCurrentMode();
            initBLE();
        }

        Serial.println("Starting Client Mode");
        pClient = BLEDevice::createClient();
        
        // Scan for server with our service UUID
        pBLEScan = BLEDevice::getScan();
        pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
        pBLEScan->setInterval(1349);
        pBLEScan->setWindow(449);
        pBLEScan->setActiveScan(true);
        pBLEScan->start(5, false);
        
        currentMode = 2;
        return true;
    } catch (...) {
        Serial.println("Error starting client mode");
        return false;
    }
}

int ESP32_BLE_Multi::getCurrentMode() const {
    return currentMode;
}

bool ESP32_BLE_Multi::isDeviceConnected() const {
    return deviceConnected;
}

bool ESP32_BLE_Multi::updateCharacteristicValue(const std::string& value) {
    if (currentMode == 1 && pCharacteristic) {
        pCharacteristic->setValue(value);
        return true;
    }
    return false;
}

void ESP32_BLE_Multi::end() {
    cleanupCurrentMode();
    currentMode = -1;
}