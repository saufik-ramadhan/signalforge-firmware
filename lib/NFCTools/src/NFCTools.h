#ifndef NFCTOOLS_H
#define NFCTOOLS_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

class NFCReader {
private:
    Adafruit_PN532* nfc;
    const uint8_t DEFAULT_KEY[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
    // Buffer for UID
    uint8_t uid[7];
    uint8_t uidLength;

    // Private methods for card handling
    bool authenticateMifareClassic(uint8_t block);
    void handleMifareClassic();
    void handleMifareUltralight();

public:
    NFCReader(uint8_t irq_pin, uint8_t reset_pin);
    ~NFCReader();

    bool begin();
    void update();
};

// class NFCTools {
//     public:
//         NFCTools();
//         void send();
//         void receive();
// };

#endif