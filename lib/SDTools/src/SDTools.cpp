#include "SDTools.h"

SDTools::SDTools(int chipSelectPin) : _chipSelectPin(chipSelectPin) {}

bool SDTools::begin() {
    return SD.begin(_chipSelectPin);
}

char* SDTools::getCardType() {
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        return "NO CARD";
    }
    if (cardType == CARD_MMC) {
        return "MMC";
    } else if (cardType == CARD_SD) {
        return "SDSC";
    } else if (cardType == CARD_SDHC) {
        return "SDHC";
    } else {
        return "UNKNOWN";
    }
}

uint64_t SDTools::getCardSize() {
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    return cardSize;
    // Serial.printf("SDCARD Size: %lluMB\n", cardSize);
}

File SDTools::openFile(const char* filename) {
    return SD.open(filename);
}

bool SDTools::readLine(File& file, char* buffer, size_t maxLen) {
    size_t i = 0;
    while (file.available() && i < maxLen - 1) {
        char c = file.read();
        if (c == '\n') {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Null-terminate the string
    return i > 0;
}


void SDTools::listDir(fs::FS& fs, const char* dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.path(), levels - 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void SDTools::deleteFile(fs::FS& fs, const char* path) {
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path)) {
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void SDTools::deleteDir(fs::FS& fs, const char* path) {
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path)) {
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void SDTools::updateFromFS(fs::FS& fs) {
    File updateBin = fs.open("/signalforge/main.bin");
    if (updateBin) {
        if (updateBin.isDirectory()) {
            Serial.println("Error, update.bin is not a file");
            updateBin.close();
            return;
        }

        size_t updateSize = updateBin.size();

        if (updateSize > 0) {
            Serial.println("Try to start update");
            performUpdate(updateBin, updateSize);
        } else {
            Serial.println("Error, file is empty");
        }

        updateBin.close();

        // when finished remove the binary from sd card to indicate end of the process
        fs.remove("/signalforge/update.bin");
    } else {
        Serial.println("Could not load update.bin from sd signalforge/ directory");
    }
}

void rebootEspWithReason(String reason) {
    Serial.println(reason);
    delay(1000);
    ESP.restart();
}

// perform the actual update from a given stream
void performUpdate(Stream& updateSource, size_t updateSize) {
    if (Update.begin(updateSize)) {
        size_t written = Update.writeStream(updateSource);
        if (written == updateSize) {
            Serial.println("Written : " + String(written) + " successfully");
        } else {
            Serial.println("Written only : " + String(written) + "/" + String(updateSize) + ". Retry?");
        }
        if (Update.end()) {
            Serial.println("OTA done!");
            if (Update.isFinished()) {
                Serial.println("Update successfully completed. Rebooting.");
            } else {
                Serial.println("Update not finished? Something went wrong!");
            }
        } else {
            Serial.println("Error Occurred. Error #: " + String(Update.getError()));
        }
    } else {
        Serial.println("Not enough space to begin OTA");
    }
}
