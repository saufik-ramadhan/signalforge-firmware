const int MAX_NUM_ITEMS = 100;
const int MAX_ITEM_LENGTH = 15;

char irMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"IR Send"},
  {"IR Read"},
  {"IR List"}  
};

char wifiMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"WIFI Scan"},
  {"WIFI Deauth"},
  {"WIFI Station"}
};

char bleMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"BLE Scan"},
  {"BLE Send"},
  {"BLE Receive"}
};

char loraMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"LoRa Send"},
  {"LoRa Receive"},
  {"LoRa Info"}
};

char  nfcMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"NFC Read"},
  {"NFC Send"},
  {"NFC List"}
};

char  microsdMenuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"Info"},
  {"Format"}
};

char menuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH] = {
  {"Infrared"},
  {"NFC"},
  {"Wifi"},
  {"MicroSD"},
  {"BLE"}
};