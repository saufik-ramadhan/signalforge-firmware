#include "LoRaTools.h"

LoRaHandler::LoRaHandler(int ss, int rst, int dio0) 
    : ss_pin(ss), rst_pin(rst), dio0_pin(dio0), counter(0) {
    initSPI();
}

LoRaHandler::~LoRaHandler() {
    delete hspi;
}

void LoRaHandler::initSPI() {
    hspi = new SPIClass(HSPI);
    LoRa.setSPI(*hspi);
}

bool LoRaHandler::begin() {
    LoRa.setPins(ss_pin, rst_pin, dio0_pin);
    return LoRa.begin(915E6);
}

void LoRaHandler::sendPacket() {
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();
    Serial.print("Sent packet: ");
    Serial.println(counter);
    counter++;
}

void LoRaHandler::processIncomingPackets() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        receivedData = "";
        while (LoRa.available()) {
            receivedData += (char)LoRa.read();
        }
        Serial.print("Received packet: ");
        Serial.println(receivedData);
    }
}

void LoRaHandler::transceive() {
    sendPacket();
    processIncomingPackets();
}