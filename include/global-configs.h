// const int MAX_NUM_ITEMS = 10;
// const int MAX_ITEM_LENGTH = 20;

// #define MENU_NUM_ITEMS 10
// #define MENU_ITEM_LENGTH 29

#ifndef GLOBAL_CONFIGS_H
#define GLOBAL_CONFIGS_H

#include <vector>

#define MAX_NUM_ITEMS 10
#define MAX_ITEM_LENGTH 40

std::vector<char*> menuItems = {
    {"Infrared"},
    {"NFC"},
    {"Wifi"},
    {"MicroSD"},
    {"BLE"},
    {"LoRa"}
};

char irMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"IR Send"},
    {"IR Read"},
    {"IR List"}  
};




std::vector<char*> wifiMenuItems = {
    {"WIFI Scan"},
    {"WIFI Connect"},
    {"WIFI Access Point"},
    {"WIFI Station"}
};

std::vector<char*> bleMenuItems = {
    {"BLE Scanner"},
    {"BLE Sender"},
    {"BLE Receiver"}
};

std::vector<char*> loraMenuItems = {
    {"LoRa Send"},
    {"LoRa Receive"}
};

std::vector<char*> nfcMenuItems = {
    {"NFC Read"}
};

std::vector<char*> microsdMenuItems = {
    {"List files"}
};

std::vector<char*> microsdFileAction = {
    {"Print as text"},
    {"Info"},
    {"Delete files"},
    {"Load Program"}
};

#endif