#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <U8g2lib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <MenuSystem.h>
#include <ButtonHandler.h>
#include <IRTools.h>
#include <SDTools.h>
#include "pin-definition.h"
#include "helper.h"
#include "global-configs.h"

// Top Level Declaration
static const BaseType_t app_cpu = 0;
MenuState currentMenuState = MAIN_MENU;
int menuIdx = 0;

// Top Level Button
QueueHandle_t buttonEventQueue;
void handleButtonEvent(ButtonType button, ButtonState state) {
    ButtonEvent event = {button, state};

    // Send the event to the queue
    if (xQueueSend(buttonEventQueue, &event, 0) != pdPASS) {
        Serial.println("Button Queue Full! event dropped :: FAILED");
    }
}

ButtonHandler buttonHandler(
    BUTTON_UP_PIN,
    BUTTON_DOWN_PIN,
    BUTTON_LEFT_PIN,
    BUTTON_RIGHT_PIN,
    -1,
    -1,
    handleButtonEvent
);

// Top Level IR
void receiverCallback() { currentMenuState = INFRARED_MENU_READING_DONE; }
IRTools irTools(receiverCallback);
File csvFile;
DecodedIRData irDataArray[50];
int irDataCount = 0;
char lineBuffer[128];
char nameBuffer[128];
int lineIndex = 0;

// Top Level SD Card
SDTools sdTools(5);

// Top Level Menu
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE, /* clock=*/SCL_PIN, /* data=*/SDA_PIN); // ESP32 Thing, HW I2C with pin remapping
MenuSystem menu(u8g2, irTools, sdTools);

/** --- Button Handler and Pipeline (queue) --- */
void buttonTask(void *parameter) {
    Serial.println("Task Button handler is active");
    while (1) {
        buttonHandler.update();
        msDelay(10);
    }
}
/** --- END --- */

/** --- Menu Handler and task (OLED --- u8g2 Library) --- */
void menuTask(void *parameter) {
    menu.addMenu(menuItems, 5); // adding menus
    Serial.println("Menu Task Running :: SUCCESS");
    while (1) {
        menu.render(currentMenuState);
    }
}

void menuNavigation(ButtonEvent event) {
    menuIdx = menu.getCurrentIndex();
    switch (currentMenuState) {
        case MAIN_MENU:
            if (event.button == BUTTON_RIGHT) {
                switch (menuIdx) {
                case 0:
                    menu.addMenu(irMenuItems, 3);
                    currentMenuState = INFRARED_MENU;
                    break;
                case 1:
                    menu.addMenu(nfcMenuItems, 3);
                    currentMenuState = NFC_MENU;
                    break;
                case 2:
                    menu.addMenu(wifiMenuItems, 3);
                    currentMenuState = WIFI_MENU;
                    break;
                case 3:
                    menu.addMenu(microsdMenuItems, 2);
                    currentMenuState = MICROSD_MENU;
                    break;
                case 4:
                    menu.addMenu(bleMenuItems, 3);
                    currentMenuState = BLE_MENU; // Fixed from INFRARED_MENU
                    break;
                }
            } else if (event.button == BUTTON_LEFT) {
                menu.addMenu(menuItems, 5);
                currentMenuState = MAIN_MENU;
            }
            break;

        case INFRARED_MENU:
            if (event.button == BUTTON_RIGHT) {
                switch (menuIdx) {
                    case 0:
                        currentMenuState = INFRARED_MENU_SEND;
                        break;
                    case 1:
                        currentMenuState = INFRARED_MENU_READING;
                        break;
                    case 2:
                        currentMenuState = INFRARED_MENU_LIST;
                        break;
                }
            } else if (event.button == BUTTON_LEFT) {
                menu.addMenu(menuItems, 5);
                currentMenuState = MAIN_MENU;
            }
            break;
        case INFRARED_MENU_SEND_LIST:
            if (event.button == BUTTON_LEFT) {
                currentMenuState = INFRARED_MENU;
                menu.addMenu(irMenuItems, 3);
            } else if (event.button == BUTTON_RIGHT) {
                irTools.send(menuIdx);
            }
            break;
        case INFRARED_MENU_SEND_LIST_FAILED:
            if (event.button == BUTTON_LEFT) {
                currentMenuState = INFRARED_MENU;
            }
            break;

        case NFC_MENU:
            if (event.button == BUTTON_RIGHT) {
                switch (menuIdx){
                case 0:
                    currentMenuState = NFC_MENU_READING;
                    break;
                case 1:
                    currentMenuState = NFC_MENU_SEND_LIST;
                    break;
                case 2:
                    currentMenuState = NFC_MENU_LIST;
                    break;
                }
            } else if (event.button == BUTTON_LEFT){
                menu.addMenu(menuItems, 5);
                currentMenuState = MAIN_MENU;
            }
            break;

        case INFRARED_MENU_READING_DONE:
            if (event.button == BUTTON_RIGHT){
                currentMenuState = INFRARED_MENU_READING_DONE_SAVING;
            } else if (event.button == BUTTON_LEFT){
                currentMenuState = INFRARED_MENU_READING;
            }
            break;
        case INFRARED_MENU_READING_DONE_SAVING:
        case INFRARED_MENU_READING:
        case INFRARED_MENU_READING_ERROR:
        case INFRARED_MENU_SEND_SENDING:
        case INFRARED_MENU_LIST:
        case INFRARED_MENU_LIST_DELETE:
        case INFRARED_MENU_LIST_DELETE_SUCCESS:
            if (event.button == BUTTON_LEFT){
                currentMenuState = INFRARED_MENU;
            }
            break;

        case NFC_MENU_READING:
        case NFC_MENU_READING_DONE:
        case NFC_MENU_READING_FAILED:
        case NFC_MENU_SEND:
        case NFC_MENU_SEND_LIST:
        case NFC_MENU_SEND_SENDING:
        case NFC_MENU_SEND_DONE:
        case NFC_MENU_LIST:
        case NFC_MENU_LIST_DELETE:
        case NFC_MENU_LIST_DELETE_SUCCESS:
            if (event.button == BUTTON_LEFT){
                currentMenuState = NFC_MENU;
            }
            break;
    }
}

void buttonMenuTask(void *parameter){
    ButtonEvent event;
    while (true){
        // Wait for an event to arrive in the queue
        if (xQueueReceive(buttonEventQueue, &event, portMAX_DELAY) == pdPASS){
            // Process the event
            switch (event.button){
                case BUTTON_UP:
                    menu.navigateUp();
                    break;
                case BUTTON_DOWN:
                    menu.navigateDown();
                    break;
                case BUTTON_RIGHT:
                    menuNavigation(event);
                    break;
                case BUTTON_LEFT:
                    menuNavigation(event);
                    break;
                default:
                    break;
            }
        }
        msDelay(10);
    }
}
/** END ---- */

/** --- IR Transceiver Handler and Tasks --- */
void irTask(void *parameter){
    irTools.begin();
    while (1){
        switch (currentMenuState){
            case INFRARED_MENU_READING:
                irTools.receive();
                break;
            case INFRARED_MENU_READING_DONE:
                break;
            case INFRARED_MENU_READING_DONE_SAVING:
                csvFile = SD.open("/ircommand.csv");
                if (!csvFile) {
                    Serial.println("Failed to open file!");
                    currentMenuState = INFRARED_MENU_SEND_LIST_FAILED;
                } else {
                    irDataCount = 0;

                    // read data rows + current row
                    while (sdTools.readLine(csvFile, lineBuffer, sizeof(lineBuffer))){
                        char name[32], protocol[16];
                        uint16_t command, address;
                        if (sscanf(lineBuffer, "%31[^;];0x%hx;0x%hx;%s", name, &command, &address, protocol) == 4){
                            strcpy(irDataArray[irDataCount].name, name);
                            irDataArray[irDataCount].command = command;
                            irDataArray[irDataCount].address = address;
                            irDataArray[irDataCount].protocol = protocol;
                            irDataCount++;
                        }
                        Serial.println(lineBuffer);
                    }

                    DecodedIRData currentIrData = irTools.getCurrentIrData();
                    sprintf(nameBuffer, "%02x_%02x_%s", currentIrData.address, currentIrData.command, currentIrData.protocol);
                    strcpy(irDataArray[irDataCount].name, nameBuffer);
                    irDataArray[irDataCount].address = irTools.getCurrentIrData().address;
                    irDataArray[irDataCount].command = irTools.getCurrentIrData().command;
                    irDataArray[irDataCount].protocol = irTools.getCurrentIrData().protocol;
                    irDataCount++;
                    // --- end
                    csvFile.close();

                    csvFile = SD.open("/ircommand.csv", FILE_WRITE, false);
                    Serial.println("writing");
                    if (!csvFile){
                        Serial.println("Failed to open file for reading");
                    } else{
                        // Writing New File
                        // Serial.println("Writing File!");
                        // csvFile.println("name;command;address;protocol");
                        for (int i = 0; i < irDataCount; i++){
                            Serial.printf("%s;0x%02X;0x%02X;%s\n",
                                        irDataArray[i].name,
                                        irDataArray[i].command,
                                        irDataArray[i].address,
                                        irDataArray[i].protocol);
                            // snprintf(lineBuffer, sizeof(lineBuffer), "%s;0x%02X;0x%02X;%s\n",
                            // irDataArray[i].name,
                            // irDataArray[i].command,
                            // irDataArray[i].address,
                            // irDataArray[i].protocol);
                            // if(csvFile.print(lineBuffer)) {
                            //   Serial.println("Row Added !");
                            // } else {
                            //   Serial.println("Row add failed !");
                            // }
                        }
                        // Serial.println("Writing File DONE!");
                    }
                    csvFile.close();
                    msDelay(10);
                    currentMenuState = INFRARED_MENU_READING;
                }
                break;
            case INFRARED_MENU_SEND:
                csvFile = sdTools.openFile("/ircommand.csv");
                if (!csvFile) {
                    Serial.println("Failed to open file!");
                    currentMenuState = INFRARED_MENU_SEND_LIST_FAILED;
                } else {
                    irDataCount = 0;
                    // Read the first row (header)
                    if (sdTools.readLine(csvFile, lineBuffer, sizeof(lineBuffer))) {
                        Serial.println(lineBuffer); // Print header to Serial (optional)
                    }
                    // Display data rows
                    while (sdTools.readLine(csvFile, lineBuffer, sizeof(lineBuffer))) {
                        Serial.println(lineBuffer); // Print each row to Serial (optional)
                        char name[32], protocol[16];
                        uint16_t command, address;

                        if (sscanf(lineBuffer, "%31[^;];0x%hx;0x%hx;%15[^;]", name, &command, &address, protocol) == 4) {
                            strcpy(irDataArray[irDataCount].name, name);
                            irDataArray[irDataCount].command = command;
                            irDataArray[irDataCount].address = address;
                            irDataArray[irDataCount].protocol = protocol;
                            irDataCount++;
                        }
                        Serial.println("SUCCESS");
                    }
                    irTools.setListSaved(irDataArray, irDataCount);
                    csvFile.close();
                    menu.addMenu(irTools.getListSavedNames(), irTools.getListSavedSize());
                    msDelay(10);
                    currentMenuState = INFRARED_MENU_SEND_LIST;
                }
                break;
            case INFRARED_MENU_SEND_LIST:
                // Listen two an event from queue
                // if (xQueueReceive(buttonEventQueue, &event, portMAX_DELAY) == pdPASS)
                // {
                //   if(event.button == BUTTON_RIGHT) {
                //     Serial.print("Hoyaaaa");
                //     irTools.send(menuIdx);
                //   }
                // }
                // msDelay(10);
                break;
            case INFRARED_MENU_SEND_LIST_FAILED:
                break;
            default:
                break;
        }
        msDelay(10);
    }
}
/** END --- */


// TODO:
// Add each task executor
//     SD files
//          cat
//          info
//          dir/ls
//          del format
//          SD Loader
//              Duplicate some of the task 
//                  functionality on arduino
//              Load confirm
//              Add back button on task
//     NFC
//          Read
//          Write
//     RF
//          Send
//          Receive
//          Prepare second item for demo
//     Bluetooth
//          Scan
//          Announce
//     Wifi
//          Scan + Power
//          Auth
//          Deauth
//          Sniff
// Add menu display each
// Remember to add state transition each

// Add RF to menu
// Run menu first to see how this works
// Duplicate button right and ok, left and back


//////////////////////////
// TODO: Hardware
// Remove oled
// Add battery
// Add charger
// Add switch
// Add button
// Reprint 3D
    // Spacer
    // OLED Hole
/////////////////////////////////



/** --- SD Card Handler and Subroutines --- */
void sdCardTask(void *parameter) {
    
}
/** END --- */

/** --- NFC Task */
void nfcTask(void *parameter) {

}
/** END --- */

/** --- RF Task */
void rfTask(void *parameter) {
    
}
/** END --- */

/** --- Bluetooth Task */
void bluetoothTask(void *parameter) {
    
}
/** END --- */

/** --- Wifi Task Task */
void wifiTask(void *parameter) {
    
}
/** END --- */








void setup() {
    Serial.begin(115200);

    /** OLED */
    u8g2.begin(); // define oled

    /** SD CARD */
    if (!sdTools.begin()) {
        Serial.println("SD card initialization :: FAILED!");
        while (1)
            ;
    } // define sdcard tools
    Serial.print("Card Type: ");
    Serial.println(sdTools.getCardType());
    Serial.print("Card Size: ");
    Serial.printf("SDCARD Size: %lluMB\n", sdTools.getCardSize());

    /** BUTTON */
    buttonEventQueue = xQueueCreate(10, sizeof(ButtonEvent)); // define queuer for button
    if (buttonEventQueue == NULL) {
        Serial.println("Failed to create queue!");
    }

    /* --- Tasks --- */
    //--- 1 Button Handler Task
    xTaskCreatePinnedToCore(buttonTask, "Button Task", 1024, NULL, 0, NULL, app_cpu);
    xTaskCreatePinnedToCore(buttonMenuTask, "Button Menu Task", 1024, NULL, 1, NULL, app_cpu);
    //--- 2 Button Menu Task Task (OLED1306)
    xTaskCreatePinnedToCore(menuTask, "Menu Task", 2048, NULL, 1, NULL, 1);
    //--- 3 Ir Tools Task ()
    xTaskCreatePinnedToCore(irTask, "IR Transceiver Task", 4000, NULL, 1, NULL, 1);
    //--- 4 Wifi Tools Task ()
    // xTaskCreatePinnedToCore(wifiTask, "Wifi Tools Task", 1024, NULL, 1, NULL, app_cpu);
    //--- 5 NFC Tools Task ()
    // xTaskCreatePinnedToCore(nfcTask, "NFC Tools Task", 1024, NULL, 1, NULL, app_cpu);
}

void loop() {
    sDelay(1);
}