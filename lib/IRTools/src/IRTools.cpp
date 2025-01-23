#include "IRTools.h"
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include "IRremote.hpp"
// Constructor to initialize pins
IRTools::IRTools(IRReceiveCallback callback) : irData(irData) {
    this->callback = callback;
}

// Begin function to initialize IR components
void IRTools::begin()
{
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start with IR_SEND_PIN
    Serial.print("IR Receiver at pin ");
    Serial.print(IR_RECEIVE_PIN);
    Serial.println(" :: ACTIVE");

    IrSender.begin();
    Serial.print("IR Sender <OK> at pin ");
    Serial.print(IR_SEND_PIN);
    Serial.println(" :: ACTIVE");
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

void IRTools::resumeReceive() {
    IrReceiver.resume();
}

void IRTools::saveReceived() {
    // todo
}

void IRTools::discardReceived() {
    // todo
}

// Function to send IR signal
void IRTools::send(unsigned long data, int bits)
{
    // 
}