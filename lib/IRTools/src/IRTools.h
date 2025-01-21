#ifndef IR_TOOLS_H
#define IR_TOOLS_H

#include <IRremote.hpp>

class IRTools {
public:
    IRTools(int recvPin, int sendPin);
    void begin(); // Initialize IR receive and send
    void receive(); // Receive IR signal
    void send(unsigned long data, int bits); // Send IR signal

private:
    int _recvPin;  // Pin for IR receiver
    int _sendPin;  // Pin for IR LED
    IRrecv* irrecv;
    IRsend* irSender;
    decode_results results;
};

#endif
