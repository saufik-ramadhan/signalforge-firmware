#include "IRTools.h"

// Constructor to initialize pins
IRTools::IRTools(int recvPin, int sendPin) {
    _recvPin = recvPin;
    _sendPin = sendPin;
    irrecv = new IRrecv(recvPin); // IR receiver object
    irSender = new IRsend(); // IR sender object
}

// Begin function to initialize IR components
void IRTools::begin() {
    irrecv->enableIRIn(); // Start IR receiver
    irSender->begin(_sendPin); // Start IR sender
    Serial.begin(9600);
    Serial.println("IRTools initialized");
}

// Function to receive IR signal
void IRTools::receive() {
    if (irrecv->decode(&results)) {
        Serial.print("Received: ");
        Serial.println(results.value, HEX);
        irrecv->resume(); // Prepare for next signal
    }
}

// Function to send IR signal
void IRTools::send(unsigned long data, int bits) {
    irSender->sendNEC(data, bits); // Send NEC protocol signal
    Serial.print("Sent: ");
    Serial.println(data, HEX);
}