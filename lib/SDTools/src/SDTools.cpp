#include "SDTools.h"

SDTools::SDTools(int chipSelectPin) : _chipSelectPin(chipSelectPin) {}

bool SDTools::begin() {
    return SD.begin(_chipSelectPin);
}

char * SDTools::getCardType() {
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

File SDTools::openFile(const char *filename) {
    return SD.open(filename);
}

bool SDTools::readLine(File &file, char *buffer, size_t maxLen) {
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