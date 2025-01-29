#include "WifiTools.h"

// Initialize static members
WiFiModeSwitcher* WiFiModeSwitcher::instance = nullptr;
wifi_promiscuous_filter_t WiFiModeSwitcher::filter = {
    .filter_mask = WIFI_PROMIS_FILTER_MASK_ALL
};

WiFiModeSwitcher::WiFiModeSwitcher(const char* ap_ssid, const char* ap_password) 
    : currentMode(STA_MODE), ap_ssid(ap_ssid), ap_password(ap_password) {
    instance = this;
}

void WiFiModeSwitcher::begin() {
    Serial.begin(115200);
    printCommands();
    setMode(STA_MODE);
}

void WiFiModeSwitcher::handleSerial() {
    if (Serial.available() > 0) {
        char command = Serial.read();
        switch (command) {
            case '1':
                setMode(STA_MODE);
                break;
            case '2':
                setMode(AP_MODE);
                break;
            case '3':
                setMode(SNIFFER_MODE);
                break;
        }
    }
}

WiFiModeSwitcher::Mode WiFiModeSwitcher::getCurrentMode() const {
    return currentMode;
}

void WiFiModeSwitcher::setMode(Mode newMode) {
    if (currentMode == newMode) return;

    cleanup();
    currentMode = newMode;
    
    switch (currentMode) {
        case STA_MODE:
            setupSTAMode();
            break;
        case AP_MODE:
            setupAPMode();
            break;
        case SNIFFER_MODE:
            setupSnifferMode();
            break;
    }
}

void WiFiModeSwitcher::cleanup() {
    esp_wifi_set_promiscuous(false);
    WiFi.disconnect();
    WiFi.mode(WIFI_MODE_NULL);
}

void WiFiModeSwitcher::setupSTAMode() {
    WiFi.mode(WIFI_MODE_STA);
    Serial.println("Switched to STA mode");
}

void WiFiModeSwitcher::setupAPMode() {
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Switched to AP mode");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void WiFiModeSwitcher::setupSnifferMode() {
    WiFi.mode(WIFI_MODE_NULL);
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_promiscuous_filter(&filter);
    esp_wifi_set_promiscuous_rx_cb(&staticSnifferCallback);
    esp_wifi_start();
    Serial.println("Switched to Sniffer mode");
}

void WiFiModeSwitcher::staticSnifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
    if (instance) {
        instance->snifferCallback(buf, type);
    }
}

void WiFiModeSwitcher::snifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
    const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
    Serial.print("Packet: ");
    for (int i = 0; i < ppkt->rx_ctrl.sig_len; i++) {
        Serial.print(ppkt->payload[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void WiFiModeSwitcher::printCommands() {
    Serial.println("ESP32 WiFi Mode Switcher");
    Serial.println("Commands: ");
    Serial.println("'1' - Switch to STA mode");
    Serial.println("'2' - Switch to AP mode");
    Serial.println("'3' - Switch to Sniffer mode");
}



// WifiTools::WifiTools() {
//     // Constructor (can initialize variables here if needed)
//     WiFi.mode(WIFI_STA);
//     WiFi.disconnect();
// }
// 
// void WifiTools::begin() {
//     WiFi.mode(WIFI_STA);
//     WiFi.disconnect();
// }
// 
// void WifiTools::connectToWiFi(const char* ssid, const char* password) {
//     Serial.println("Connecting to WiFi...");
//     WiFi.begin(ssid, password);
// 
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.print(".");
//     }
// 
//     Serial.println("\nWiFi connected!");
// }
// 
// void WifiTools::scanNetworks() {
//     Serial.println("Scanning for WiFi networks...");
//     int networkCount = WiFi.scanNetworks(); // Start scanning for networks
// 
//     if (networkCount == 0) {
//         Serial.println("No networks found.");
//     } else {
//         Serial.println("Networks found:");
//         for (int i = 0; i < networkCount; i++) {
//             Serial.print(i + 1);
//             Serial.print(": ");
//             Serial.print(WiFi.SSID(i));             // Network SSID
//             Serial.print(" (RSSI: ");
//             Serial.print(WiFi.RSSI(i));            // Signal strength
//             Serial.print(" dBm) Encryption: ");
//             Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted");
//         }
//     }
//     WiFi.scanDelete(); // Clean up after scanning
// }
// 
// void WifiTools::deauthNetwork(const uint8_t* targetMAC, const uint8_t* apMAC) {
//     uint8_t deauthPacket[26] = {
//         0xC0, 0x00, // Frame Control: Deauth
//         0x00, 0x00, // Duration
//         apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // Destination MAC (target)
//         apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // Source MAC (AP)
//         apMAC[0], apMAC[1], apMAC[2], apMAC[3], apMAC[4], apMAC[5], // BSSID (AP)
//         0x00, 0x00, // Sequence Number
//         0x07, 0x00  // Reason Code: Class 3 frame received from nonassociated station
//     };
// 
//     // Copy the target's MAC into the destination field of the packet
//     memcpy(&deauthPacket[4], targetMAC, 6);
// 
//     // Initialize WiFi in promiscuous mode
//     WiFi.mode(WIFI_STA);
//     esp_wifi_set_promiscuous(true);
// 
//     // Send deauth packet
//     esp_wifi_80211_tx(WIFI_IF_STA, deauthPacket, sizeof(deauthPacket), false);
// 
//     // Disable promiscuous mode after sending
//     esp_wifi_set_promiscuous(false);
// 
//     Serial.println("Deauthentication packet sent.");
// }
// 
// String WifiTools::getIPAddress() {
//     if (WiFi.status() == WL_CONNECTED) {
//         return WiFi.localIP().toString();
//     }
//     return "Not connected";
// }
// 
// bool WifiTools::isConnected() {
//     return WiFi.status() == WL_CONNECTED;
// }
