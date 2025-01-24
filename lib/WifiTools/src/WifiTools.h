#ifndef WIFI_TOOLS_H
#define WIFI_TOOLS_H

#include <Arduino.h>
#include <WiFi.h> // Include the WiFi library for ESP32/ESP8266
#include <esp_wifi.h> // Required for packet manipulation

class WifiTools {
public:
    WifiTools();  // Constructor
    void begin();
    void connectToWiFi(const char* ssid, const char* password);
    void scanNetworks();
    void deauthNetwork(const uint8_t* targetMAC, const uint8_t* apMAC);
    String getIPAddress();
    bool isConnected();
};

#endif
