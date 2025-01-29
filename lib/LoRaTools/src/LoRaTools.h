#ifndef LORA_TOOLS_H
#define LORA_TOOLS_H

#include <SPI.h>
#include <LoRa.h>

class LoRaHandler {
private:
    // Common components
    const int ss_pin;
    const int rst_pin;
    const int dio0_pin;
    SPIClass* hspi;
    
    // Sender-specific
    int counter;
    
    // Receiver-specific
    String receivedData;

    void initSPI();

public:
    LoRaHandler(int ss = 15, int rst = 4, int dio0 = -1);
    ~LoRaHandler();

    bool begin();
    void sendPacket();
    void processIncomingPackets();
    void transceive();
};


// class LoRaTools {
//     public:
//         LoRaTools();
//         void receive();
//         void send();
// };

#endif