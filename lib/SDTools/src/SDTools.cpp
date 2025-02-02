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


// Assuming SDTools is a class and readLine is defined as given.
std::vector<String> SDTools::head(const char* dirname, int numLines) {
    fs::FS& fs = SD;
    const size_t maxLineLen = 20;  // Maximum length for each line
    char buffer[maxLineLen];
    std::vector<String> lines; // Vector to store the result

    File file = fs.open(dirname);
    if (!file) {
        return lines;
    }

    // Attempt to read up to 5 lines
    for (int i = 0; i < numLines; i++) {
        if (readLine(file, buffer, maxLineLen)) {
            // Successfully read a line; store it in the vector
            lines.push_back(String(buffer));
        } else {
            // No more data (end-of-file reached or an empty line read)
            break;
        }
    }
    
    return lines;
}

std::vector<String> SDTools::listDir(const char* dirname) {
    fs::FS& fs = SD;
    uint8_t levels = 0;
    Serial.printf("Listing directory: %s\n", dirname);
    std::vector<String> fileList;
    
    File root = fs.open(dirname);
    if (!root) {
        Serial.println("Failed to open directory");
        return fileList;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return fileList;
    }
    
    // Iterate over each entry in the directory (non-recursive).
    File file = root.openNextFile();
    while (file) {
        // Optionally, append a trailing slash for directories.
        if (file.isDirectory()) {
            fileList.push_back(String(file.name()) + "/");
            Serial.print("  DIR : ");
        } else {
            fileList.push_back(String(file.name()));
            Serial.print("  FILE: ");
        }
        Serial.println(file.name());
        file = root.openNextFile();
    }
    
    return fileList;
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

void SDTools::updateFromFS(fs::FS& fs, const char* path) {
    File updateBin = fs.open(path);
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
