// SDFileReader.h
#ifndef SD_TOOLS_H
#define SD_TOOLS_H

#include <Arduino.h>
#include <SD.h>
#include <FS.h>
#include <Update.h>
#include <vector>

class SDTools {
    public:
        SDTools(int chipSelectPin);
        bool begin();
        char * getCardType();
        uint64_t getCardSize();
        char * getCardInfo();                                                 // Info
        File openFile(const char *filename);
        bool readLine(File &file, char *buffer, size_t maxLen);

        std::vector<String> head(const char* dirname, int numLines);                  // Head
        std::vector<String> listDir(const char* dirname);                   // Dir/ls
        void deleteFile(fs::FS& fs, const char* path);                        // Rm
        void deleteDir(fs::FS& fs, const char* path);                        // Rmdir
        void updateFromFS(fs::FS& fs, const char* path);                                        // Load function

    private:
        int _chipSelectPin;

};

#endif // SDFILEREADER_H