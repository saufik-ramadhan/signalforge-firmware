#ifndef IR_TOOLS_H
#define IR_TOOLS_H

#include <Arduino.h>

typedef void (*IRReceiveCallback)();

struct DecodedIRData {
    char name[32];
    uint16_t command;
    uint16_t address;
    const char *protocol;
    char *protocolName;
};
class IRTools {
public:
    IRTools(IRReceiveCallback callback);
    void begin(); // Initialize IR receive and send
    void receive(); // Receive IR signal
    void setListSaved(DecodedIRData *listIrData, size_t size); // list save IR signal to be emulated
    void resumeReceive(); // Resume Receiver
    void saveReceived(); // Save Received Signal
    void discardReceived(); // Discard received signal
    void send(uint8_t index); // Send IR signal

    // Getter for the current irData
    DecodedIRData * getListSaved() const; // storage for data from sdcard
    const char ** getListSavedNames();
    size_t getListSavedSize() const;
    DecodedIRData getCurrentIrData() const; // currently only for IR Read

private:
    int _recvPin;  // Pin for IR receiver
    int _sendPin;  // Pin for IR LED
    DecodedIRData irData;
    DecodedIRData * listIrData;
    size_t listIrDataSize;
    
    IRReceiveCallback callback;
};

#endif
