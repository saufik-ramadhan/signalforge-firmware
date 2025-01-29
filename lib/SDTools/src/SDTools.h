// SDFileReader.h
#ifndef SD_TOOLS_H
#define SD_TOOLS_H

#include <Arduino.h>
#include <SD.h>
#include <FS.h>

class SDTools {
public:
    SDTools(int chipSelectPin);
    bool begin();
    char * getCardType();
    uint64_t getCardSize();
    void listFileInDir(File dir);
    char * getCardInfo();
    File openFile(const char *filename);
    bool readLine(File &file, char *buffer, size_t maxLen);
    char ** readChunk(File &file, char ** chunk, char * format, size_t maxLen, size_t maxChunk);

private:
    int _chipSelectPin;
};

#endif // SDFILEREADER_H