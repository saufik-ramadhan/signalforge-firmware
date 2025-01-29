#ifndef WIFI_TOOLS_H
#define WIFI_TOOLS_H

#include <Arduino.h>
#include <WiFi.h> // Include the WiFi library for ESP32/ESP8266
#include <esp_wifi.h> // Required for packet manipulation

// #include "freertos/FreeRTOS.h"
// #include "esp_wifi.h"
// #include "esp_wifi_types.h"
// #include "esp_system.h"
// #include "esp_event.h"
// #include "esp_event_loop.h"
// #include "nvs_flash.h"
// #include "driver/gpio.h"
// 
// #include <Update.h>
// #include <FS.h>
// #include <SD.h>
// 
// 
// class WifiTools {
//     public:
//         WifiTools();  // Constructor
//         void begin();
//         void connectToWiFi(const char* ssid, const char* password);
//         void scanNetworks();
//         void deauthNetwork(const uint8_t* targetMAC, const uint8_t* apMAC);
//         String getIPAddress();
//         bool isConnected();
// };

class WiFiModeSwitcher {
    public:
        enum Mode {
            STA_MODE = 0,
            AP_MODE = 1,
            SNIFFER_MODE = 2
        };

        WiFiModeSwitcher(const char* ap_ssid = "ESP32_AP", const char* ap_password = "12345678");
        void begin();
        void handleSerial();
        Mode getCurrentMode() const;
        void setMode(Mode newMode);
        void scanNetworks(); // New method for network scanning

    private:
        Mode currentMode;
        const char* ap_ssid;
        const char* ap_password;
        static wifi_promiscuous_filter_t filter;
        static WiFiModeSwitcher* instance;
        unsigned long lastScanTime;    // Track last scan time
        static const unsigned long SCAN_INTERVAL = 10000; // Scan every 10 seconds

        // Private methods
        void cleanup();
        void setupSTAMode();
        void setupAPMode();
        void setupSnifferMode();
        void snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type);
        void printCommands();
        void printNetworkInfo(int networkIndex); // New helper method

        // Static callback wrapper
        static void staticSnifferCallback(void* buf, wifi_promiscuous_pkt_type_t type);
};

#endif // WIFI_MODE_SWITCHER_H

