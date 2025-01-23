#ifndef IR_TOOLS_H
#define IR_TOOLS_H

#include <Arduino.h>

typedef void (*IRReceiveCallback)();

struct DecodedIRData {
    uint16_t command;
    uint16_t address;
    const char* protocol;
};
class IRTools {
public:
    IRTools(IRReceiveCallback callback);
    void begin(); // Initialize IR receive and send
    void receive(); // Receive IR signal
    void listSaved(); // list save IR signal to be emulated
    void resumeReceive(); // Resume Receiver
    void saveReceived(); // Save Received Signal
    void discardReceived(); // Discard received signal
    void send(unsigned long data, int bits); // Send IR signal

    // Getter for the current irData
    DecodedIRData getCurrentIrData() const;

private:
    int _recvPin;  // Pin for IR receiver
    int _sendPin;  // Pin for IR LED
    DecodedIRData irData;
    
    IRReceiveCallback callback;
};

#endif
