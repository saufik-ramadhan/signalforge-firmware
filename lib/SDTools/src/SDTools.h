// SDFileReader.h
#ifndef SD_TOOLS_H
#define SD_TOOLS_H

#include <Arduino.h>
#include <SD.h>

class SDTools {
public:
    SDTools(int chipSelectPin);
    bool begin();
    File openFile(const char *filename);
    bool readLine(File &file, char *buffer, size_t maxLen);

private:
    int _chipSelectPin;
};

#endif // SDFILEREADER_H