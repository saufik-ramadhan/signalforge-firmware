#include "MenuSystem.h"
#include "BitmapIcon.h"
#include "global-configs.h"

MenuSystem::MenuSystem(U8G2& display, IRTools& irTools, SDTools& sdTools) :
    display(display),
    irTools(irTools),
    sdTools(sdTools),
    currentMenuState(currentMenuState),
    itemCount(0),
    menuItems({}),
    currentIndex(0),
    previousIndex(0),
    nextIndex(0),
    topIndex(0) {
    itemsPerPage = display.getDisplayHeight() / 10; // Assuming 10px per item row height
}

int MenuSystem::getCurrentIndex() const {
    return currentIndex;
}

void MenuSystem::addMenu(char menuItems[MAX_NUM_ITEMS][MAX_ITEM_LENGTH], size_t itemCount) {
    for (int i = 0; i < MAX_NUM_ITEMS; ++i) {
        strncpy(this->menuItems[i], menuItems[i], MAX_ITEM_LENGTH - 1); // Copy string safely
        this->menuItems[i][MAX_ITEM_LENGTH - 1] = '\0'; // Ensure null-termination
    }
    this->itemCount = itemCount;
    currentIndex = 0;
    previousIndex = 0;
    nextIndex = 0;
    topIndex = 0;
}

void MenuSystem::addMenu(std::vector<char*> menuItems, size_t itemCount) {
    for (int i = 0; i < MAX_NUM_ITEMS; ++i) {
        strncpy(this->menuItems[i], menuItems[i], MAX_ITEM_LENGTH - 1); // Copy string safely
        this->menuItems[i][MAX_ITEM_LENGTH - 1] = '\0'; // Ensure null-termination
    }
    this->itemCount = itemCount;
    currentIndex = 0;
    previousIndex = 0;
    nextIndex = 0;
    topIndex = 0;
}

void MenuSystem::navigateUp() {
    currentIndex--;
    if (currentIndex < 0) {
        currentIndex = itemCount - 1;
    }
}

void MenuSystem::navigateDown() {
    currentIndex++;
    if (currentIndex >= itemCount) {
        currentIndex = 0;
    }
}

void MenuSystem::select() {
    // do nothing
}

void MenuSystem::back() {
    // do nothing
}

void MenuSystem::render(MenuState currentMenuState) {
    // set correct values for previous and next items
    previousIndex = currentIndex - 1;
    if (previousIndex < 0) {
        previousIndex = itemCount - 1;
    }
    nextIndex = currentIndex + 1;
    if (nextIndex >= itemCount) {
        nextIndex = 0;
    } // next item would be after last = make it the first

    display.firstPage();
    do {
        switch (currentMenuState) {
            case MAIN_MENU:
                drawMenu();
            case INFRARED_MENU:
                drawIRMenu();
                break;
            case INFRARED_MENU_READING:
                infraredMenuReadingScreen();
                break;
            case INFRARED_MENU_READING_DONE:
                char cmdBuffer[6];
                sprintf(cmdBuffer, "%hx", irTools.getCurrentIrData().command);
                char addrBuffer[6];
                sprintf(addrBuffer, "%hx", irTools.getCurrentIrData().address);
                infraredMenuReadingDoneScreen(cmdBuffer, addrBuffer, irTools.getCurrentIrData().protocol);
                break;
            case INFRARED_MENU_SEND:
                break;
            case INFRARED_MENU_SEND_LIST:
                infraredMenuSendListScreen();
                break;
            case INFRARED_MENU_SEND_LIST_FAILED:
                infraredMenuSendListFailedScreen();
                break;

            case INFRARED_MENU_READING_DONE_SAVING:
            case INFRARED_MENU_READING_ERROR:
            case INFRARED_MENU_SEND_SENDING:
            case INFRARED_MENU_LIST:
            case INFRARED_MENU_LIST_DELETE:
            case INFRARED_MENU_LIST_DELETE_SUCCESS:
                inDevelopment();
                break;
            


            case MenuState::WIFI_MENU:
                break;
                case WIFI_MENU_SCAN:
                    break;
                    case WIFI_MENU_SCAN_DONE:
                        break;
                case WIFI_MENU_CONNECTING:
                    break;
                    case WIFI_MENU_CONNECTING_DONE:
                        break;
                    case WIFI_MENU_CONNECTING_FAIL:
                        break;
                case WIFI_MENU_SNIFFER:
                    break;
                case WIFI_MENU_AP_MODE:
                    break;
                    case WIFI_MENU_AP_MODE_DONE:
                        break;

            case MenuState::BLE_MENU:
                break;
                case BLE_MENU_SCAN:
                    break;
                    case BLE_MENU_SCAN_RESULT:
                        break;
                case BLE_MENU_RECEIVE:
                    break;
                    case BLE_MENU_RECEIVE_RESULT:
                        break;
                case BLE_MENU_SEND:
                    break;
                    case BLE_MENU_SEND_RESEND:
                        break;
            case LORA_MENU:
                break;
                case LORA_MENU_SEND:
                    break;
                case LORA_MENU_RECEIVE:
                    break;


            case MenuState::NFC_MENU:
                break;
                case NFC_MENU_READING:
                    break;


            case MICROSD_MENU:
                break;
                case MICROSD_MENU_LIST:
                    break;
                    case MICROSD_MENU_LIST_DONE:
                        break;
                        case MICROSD_MENU_FILE_MENU:
                            break;
                            case MICROSD_MENU_FILE_MENU_CAT:
                                break;
                                case MICROSD_MENU_FILE_MENU_CAT_RESULT:
                                    break;
                            case MICROSD_MENU_FILE_MENU_DELETE_FILE:
                                break;
                                case MICROSD_MENU_FILE_MENU_DELETE_FILE_DONE:
                                    break;
                            case MICROSD_MENU_FILE_MENU_DELETE_FOLDER:
                                break;
                                case MICROSD_MENU_FILE_MENU_DELETE_FOLDER_DONE:
                                    break;
                            case MICROSD_MENU_FILE_MENU_INFO_FILE:
                                break;
                            case MICROSD_MENU_FILE_MENU_LOAD_PROGRAM:
                                break;


            default:
                inDevelopment();
        }

        // display.setFont(u8g2_font_ncenB14_tr);
        // display.drawStr(0,15,"Hello World!");
    } while (display.nextPage());
}

void MenuSystem::drawMenu() {
    // selected item background
    display.setFlipMode(0);

    display.drawBitmap(0, 22, 128 / 8, 21, bitmap_item_sel_outline);

    // draw previous item as icon + label
    display.setFont(u8g_font_7x14);
    display.drawStr(25, 15, menuItems[previousIndex]);
    display.drawBitmap(4, 2, 16 / 8, 16, bitmap_icons[previousIndex]);

    // draw selected item as icon + label in bold font
    display.setFont(u8g_font_7x14B);
    display.drawStr(25, 15 + 20 + 2, menuItems[currentIndex]);
    display.setDrawColor(0); // Invert text color for selected item
    display.drawBitmap(4, 24, 16 / 8, 16, bitmap_icons[currentIndex]);
    display.setDrawColor(1); // Invert text color for selected item

    // draw next item as icon + label
    display.setFont(u8g_font_7x14);
    display.drawStr(25, 15 + 20 + 20 + 2 + 2, menuItems[nextIndex]);
    display.drawBitmap(4, 46, 16 / 8, 16, bitmap_icons[nextIndex]);

    // draw scrollbar handle
    display.drawBox(125, 64 / itemCount * currentIndex, 3, 64 / itemCount);
}

void MenuSystem::drawIRMenu() {
    // selected item background
    display.drawBitmap(0, 22, 128 / 8, 21, bitmap_item_sel_outline);

    // draw previous item as icon + label
    display.setFont(u8g_font_7x14);
    display.drawStr(25, 15, menuItems[previousIndex]);
    display.drawBitmap(4, 2, 16 / 8, 16, bitmap_icons[0]);

    // draw selected item as icon + label in bold font
    display.setFont(u8g_font_7x14B);
    display.drawStr(25, 15 + 20 + 2, menuItems[currentIndex]);
    display.setDrawColor(0); // Invert text color for selected item
    display.drawBitmap(4, 24, 16 / 8, 16, bitmap_icons[0]);
    display.setDrawColor(1); // Invert text color for selected item

    // draw next item as icon + label
    display.setFont(u8g_font_7x14);
    display.drawStr(25, 15 + 20 + 20 + 2 + 2, menuItems[nextIndex]);
    display.drawBitmap(4, 46, 16 / 8, 16, bitmap_icons[0]);

    // draw scrollbar handle
    display.drawBox(125, 64 / itemCount * currentIndex, 3, 64 / itemCount);
}

void MenuSystem::drawList() {
    // selected item background
    display.drawBitmap(0, 22, 128 / 8, 21, bitmap_item_sel_outline);

    // draw previous item as icon + label
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 15, menuItems[previousIndex]);

    // draw selected item as icon + label in bold font
    display.setFont(u8g_font_7x14B);
    display.drawStr(0, 15 + 20 + 2, menuItems[currentIndex]);

    // draw next item as icon + label
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 15 + 20 + 20 + 2 + 2, menuItems[nextIndex]);

    // draw scrollbar handle
    display.drawBox(125, 64 / itemCount * currentIndex, 3, 64 / itemCount);
}

// Screen
void MenuSystem::inDevelopment() {
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 14, "404");
    display.drawStr(0, 28, "In Development");
}

void MenuSystem::infraredMenuReadingScreen() {
    display.drawXBMP(0, 0, 128, 64, screen[0]);
}

void MenuSystem::infraredMenuReadingDoneScreen(char* command, char* address, const char* protocol) {
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 14, "Read Done");
    display.drawStr(0, 28, "cmd: 0x");
    display.drawStr(49, 28, command);
    display.drawStr(0, 42, "adr: 0x");
    display.drawStr(49, 42, address);
    display.drawStr(0, 56, "pro:");
    display.drawStr(35, 56, protocol);
}

void MenuSystem::infraredMenuReadingDoneSavingScreen() {
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 14, "IR Code Saved");
}

void MenuSystem::infraredMenuReadingErrorScreen() {
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 14, "IR Reading Error");
}

void MenuSystem::infraredMenuSendScreen() {}

void MenuSystem::infraredMenuSendListScreen() {
    drawList();
}

void MenuSystem::infraredMenuSendListFailedScreen() {
    display.setFont(u8g_font_7x14);
    display.drawStr(0, 14, "Failed to open file");
    display.drawStr(0, 28, "check ircommand.csv");
    display.drawStr(0, 42, "if is exist !!!");
}

void MenuSystem::infraredMenuSendSendingScreen() {}
void MenuSystem::infraredMenuListScreen() {}
void MenuSystem::infraredMenuListDeleteScreen() {}
void MenuSystem::infraredMenuListDeleteSuccessScreen() {}


void MenuSystem::WIFI_MENU() {

}

void MenuSystem::BLE_MENU() {

}

void MenuSystem::SD_MENU() {

}

void MenuSystem::NFC_MENU() {

}

void MenuSystem::RF_MENU() {

}
