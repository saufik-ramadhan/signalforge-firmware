enum MenuState {
    MAIN_MENU,

    INFRARED_MENU,
        INFRARED_MENU_READING,
            INFRARED_MENU_READING_DONE,
            INFRARED_MENU_READING_DONE_SAVING,
            INFRARED_MENU_READING_ERROR,
        INFRARED_MENU_SEND,
            INFRARED_MENU_SEND_LIST,
            INFRARED_MENU_SEND_LIST_DONE,
            INFRARED_MENU_SEND_LIST_FAILED,
            INFRARED_MENU_SEND_SENDING,
        INFRARED_MENU_LIST,
            INFRARED_MENU_LIST_DELETE,
            INFRARED_MENU_LIST_DELETE_SUCCESS,






    // Menu / Sensor menu
        // Submenu / Sensor Feature Execute
            // Submenu status / Feature status / success/fail


    WIFI_MENU,
        WIFI_MENU_SCAN,
            WIFI_MENU_SCAN_SCANNING,
            WIFI_MENU_SCAN_LIST,
        WIFI_MENU_DEAUTH,
            WIFI_MENU_DEAUTH_SCANNING,
            WIFI_MENU_DEAUTH_LIST,
            WIFI_MENU_DEAUTH_ATTACKING,
        WIFI_MENU_STATION,
    
    BLE_MENU,
        BLE_MENU_SCAN,
        BLE_MENU_RECEIVE,
        BLE_MENU_SEND,

    LORA_MENU,
        LORA_MENU_SEND,
        LORA_MENU_RECEIVE,
        LORA_MENU_INFO,

    NFC_MENU,
        NFC_MENU_READING,
            NFC_MENU_READING_DONE,
            NFC_MENU_READING_FAILED,
        NFC_MENU_SEND,
            NFC_MENU_SEND_LIST,
            NFC_MENU_SEND_SENDING,
            NFC_MENU_SEND_DONE,
        NFC_MENU_LIST,
            NFC_MENU_LIST_DELETE,
            NFC_MENU_LIST_DELETE_SUCCESS,

    MICROSD_MENU,
        MICROSD_MENU_INFO,
        MICROSD_MENU_FORMAT,
            MICROSD_MENU_FORMAT_DONE,
            MICROSD_MENU_FORMAT_ERROR
};