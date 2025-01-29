#include "NFCTools.h"

NFCReader::NFCReader(uint8_t irq_pin, uint8_t reset_pin) {
    nfc = new Adafruit_PN532(irq_pin, reset_pin);
}

NFCReader::~NFCReader() {
    delete nfc;
}

bool NFCReader::authenticateMifareClassic(uint8_t block) {
    Serial.println("Trying to authenticate block 4 with default KEYA value");
    return nfc->mifareclassic_AuthenticateBlock(uid, uidLength, block, 0, const_cast<uint8_t*>(DEFAULT_KEY));
}

void NFCReader::handleMifareClassic() {
    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    
    if (authenticateMifareClassic(4)) {
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];

        if (nfc->mifareclassic_ReadDataBlock(4, data)) {
            Serial.println("Reading Block 4:");
            nfc->PrintHexChar(data, 16);
            Serial.println("");
        } else {
            Serial.println("Unable to read the requested block. Try another key?");
        }
    } else {
        Serial.println("Authentication failed: Try another key?");
    }
}

void NFCReader::handleMifareUltralight() {
    Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
    Serial.println("Reading page 4");
    
    uint8_t data[32];
    if (nfc->mifareultralight_ReadPage(4, data)) {
        nfc->PrintHexChar(data, 4);
        Serial.println("");
    } else {
        Serial.println("Unable to read the requested page!");
    }
}

bool NFCReader::begin() {
    nfc->begin();

    uint32_t versiondata = nfc->getFirmwareVersion();
    if (!versiondata) {
        Serial.println("Didn't find PN53x board");
        return false;
    }

    // Display device information
    Serial.print("Found chip PN5"); 
    Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); 
    Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); 
    Serial.println((versiondata >> 8) & 0xFF, DEC);

    Serial.println("Waiting for an ISO14443A Card ...");
    return true;
}

void NFCReader::update() {
    if (nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
        // Display card information
        Serial.println("Found an ISO14443A card");
        Serial.print("  UID Length: ");
        Serial.print(uidLength, DEC);
        Serial.println(" bytes");
        Serial.print("  UID Value: ");
        nfc->PrintHex(uid, uidLength);
        Serial.println("");

        // Handle different card types
        if (uidLength == 4) {
            handleMifareClassic();
        } else if (uidLength == 7) {
            handleMifareUltralight();
        }

        delay(1000); // Wait before next read
    }
}