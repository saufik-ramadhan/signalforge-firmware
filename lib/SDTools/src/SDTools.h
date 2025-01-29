// SDFileReader.h
#ifndef SD_TOOLS_H
#define SD_TOOLS_H

#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include <Update.h>

class SDTools {
    public:
        SDTools(int chipSelectPin);
        bool begin();
        char * getCardType();
        uint64_t getCardSize();
        char * getCardInfo();                                                 // Info
        File openFile(const char *filename);
        bool readLine(File &file, char *buffer, size_t maxLen);               // Cat

        void listDir(fs::FS& fs, const char* dirname, uint8_t levels);        // Dir/ls
        void deleteFile(fs::FS& fs, const char* path);                        // Rm
        void deleteDir(fs::FS& fs, const char* path);                        // Rmdir
        void updateFromFS(fs::FS& fs);                                        // Load function

    private:
        int _chipSelectPin;
};

#endif // SDFILEREADER_H