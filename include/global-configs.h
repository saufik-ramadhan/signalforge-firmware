// const int MAX_NUM_ITEMS = 10;
// const int MAX_ITEM_LENGTH = 20;

// #define MENU_NUM_ITEMS 10
// #define MENU_ITEM_LENGTH 29

#ifndef GLOBAL_CONFIGS_H
#define GLOBAL_CONFIGS_H

#define MAX_NUM_ITEMS 10
#define MAX_ITEM_LENGTH 30

char irMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"IR Send"},
    {"IR Read"},
    {"IR List"}  
};

char menuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Infrared"},
    {"NFC"},
    {"Wifi"},
    {"MicroSD"},
    {"BLE"},
    {"LoRa"}
};







char wifiMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"WIFI Scan"},
    {"WIFI Deauth"},
    {"WIFI Station"}
};

char bleMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"BLE Start"},
    {"BLE Scan"},
    {"BLE Receive"}
};

char loraMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"LoRa Send"},
    {"LoRa Receive"},
    {"LoRa Info"}
};

char nfcMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"NFC Read"},
    {"NFC Write"}
};

char microsdMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
    {"Print as text"},
    {"Info"},
    {"List files"},
    {"Delete files"},
    {"Load Program"}
};


#endif