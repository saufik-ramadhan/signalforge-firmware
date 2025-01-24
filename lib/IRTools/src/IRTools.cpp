#include "IRTools.h"
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include "IRremote.hpp"
// Constructor to initialize pins
IRTools::IRTools(IRReceiveCallback callback) : irData(irData), listIrData(listIrData) {
    this->callback = callback;
}

// Begin function to initialize IR components
void IRTools::begin()
{
    pinMode(IR_RECEIVE_PIN, INPUT);
    pinMode(IR_SEND_PIN, OUTPUT);
    
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start with IR_SEND_PIN
    Serial.print("IR Receiver at pin ");
    Serial.print(IR_RECEIVE_PIN);
    Serial.println(" :: ACTIVE");

    IrSender.begin();
    Serial.print("IR Sender <OK> at pin ");
    Serial.print(IR_SEND_PIN);
    Serial.println(" :: ACTIVE");

    IrSender.enableIROut(38); // Call it with 38 kHz just to initialize the values printed below
}

// Function to receive IR signal
void IRTools::receive()
{
    if (IrReceiver.decode())
    {
        IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        if (IrReceiver.decodedIRData.protocol == UNKNOWN)
        {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            IrReceiver.printIRResultRawFormatted(&Serial, true);
        }
        Serial.println();
        /*
         * !!!Important!!! Enable receiving of the next value,
         * since receiving has stopped after the end of the current received data packet.
         */
        IrReceiver.resume(); // Enable receiving of the next value

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        irData.address = IrReceiver.decodedIRData.address;
        irData.command = IrReceiver.decodedIRData.command;
        irData.protocol = IrReceiver.getProtocolString();
        callback();
    }
}

DecodedIRData IRTools::getCurrentIrData() const {
    return irData;
}

DecodedIRData * IRTools::getListSaved() const {
    return listIrData;
}

const char ** IRTools::getListSavedNames() {
    char** names = new char*[listIrDataSize];
    for (size_t i = 0; i < listIrDataSize; ++i) {
        // Allocate memory for each name and copy it
        names[i] = new char[32];
        strcpy(names[i], listIrData[i].name);
    }
    return const_cast<const char**>(names);
}

size_t IRTools::getListSavedSize() const {
    return listIrDataSize;
}


void IRTools::setListSaved(DecodedIRData * receivedListIrData, size_t size) {
    listIrData = receivedListIrData;
    listIrDataSize = size;
}

void IRTools::resumeReceive() {
    IrReceiver.resume();
}

void IRTools::saveReceived() {
    
}

void IRTools::discardReceived() {
    // todo
}

// Function to send IR signal
void IRTools::send(uint8_t index)
{
    // Serial.print("Sent:");
    // Serial.println(listIrData[index].protocol);
    DecodedIRData irData = listIrData[index];
    
    if (strcmp(irData.protocol, "samsung") == 0 || strcmp(irData.protocol, "Samsung") == 0) {
        IrSender.sendSamsung(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
    else if (strcmp(irData.protocol, "NEC") == 0) {
        IrSender.sendNEC(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
    else if (strcmp(irData.protocol, "Onkyo") == 0) {
        IrSender.sendOnkyo(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
    else if (strcmp(irData.protocol, "Sony") == 0) {
        IrSender.sendSony(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
    else if (strcmp(irData.protocol, "LG") == 0) {
        IrSender.sendLG(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
    else if (strcmp(irData.protocol, "Sharp") == 0) {
        IrSender.sendSharp(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
    else if (strcmp(irData.protocol, "Panasonic") == 0) {
        IrSender.sendPanasonic(irData.address, irData.command, 0);
        Serial.println("IR Sent");
    }
}