#include "MenuSystem.h"
#include "BitmapIcon.h"

MenuSystem::MenuSystem(U8G2 &display, IRTools &irTools, SDTools &sdTools) :
                                        display(display),
                                        irTools(irTools),
                                        sdTools(sdTools),
                                        currentMenuState(currentMenuState),
                                        itemCount(0),
                                        menuItems({}),
                                        currentIndex(0),
                                        currentIndexP1(0),
                                        currentIndexP2(0),
                                        currentIndexP3(0),
                                        previousIndex(0),
                                        nextIndex(0),
                                        topIndex(0)
{
  itemsPerPage = display.getDisplayHeight() / 10; // Assuming 10px per item row height
}

int MenuSystem::getCurrentIndex() const {
  return currentIndex;
}

void MenuSystem::addMenu(char menuItems[MENU_NUM_ITEMS][MENU_ITEM_LENGTH], size_t itemCount)
{
  for (int i = 0; i < MENU_NUM_ITEMS; ++i) {
    strncpy(this->menuItems[i], menuItems[i], MENU_ITEM_LENGTH - 1); // Copy string safely
    this->menuItems[i][MENU_ITEM_LENGTH - 1] = '\0'; // Ensure null-termination
  }
  this->itemCount = itemCount;
  currentIndex = 0;
  currentIndexP1 = 0;
  currentIndexP2 = 0;
  currentIndexP3 = 0;
  previousIndex = 0;
  nextIndex = 0;
  topIndex = 0;
}

void MenuSystem::navigateUp(MenuState currentMenuState)
{
  switch(currentMenuState) {
    case INFRARED_MENU_SEND_LIST:
    case INFRARED_MENU_LIST_DONE:
      if (currentIndex > 0) {
        currentIndex--;
      }
      break;
    default: 
      currentIndex--;
      if (currentIndex < 0) {
        currentIndex = itemCount - 1;
      }
      break;
  }
}

void MenuSystem::navigateDown(MenuState currentMenuState)
{
  switch(currentMenuState) {
    case INFRARED_MENU_SEND_LIST:
    case INFRARED_MENU_LIST_DONE:
      if (currentIndex < itemCount - 1) {
        currentIndex++;
      }
      break;
    default: 
      currentIndex++;
      if (currentIndex >= itemCount) {
        currentIndex = 0;
      }
      break;
  }
}

void MenuSystem::select()
{
  // do nothing
}

void MenuSystem::back()
{
  // do nothing
}

void MenuSystem::render(MenuState currentMenuState)
{
  switch(currentMenuState) {
    case INFRARED_MENU_SEND_LIST:
    case INFRARED_MENU_LIST_DONE:
      currentIndexP1 = (currentIndex / 3) * 3;
      currentIndexP2 = currentIndexP1 + 1;
      currentIndexP3 = currentIndexP1 + 2;
      break;
    default: 
      // set correct values for previous and next items
      previousIndex = currentIndex - 1;
      if (previousIndex < 0) {
        previousIndex = itemCount - 1;
      }
      nextIndex = currentIndex + 1;  
      if (nextIndex >= itemCount) {
        nextIndex = 0;
      } // next item would be after last = make it the first
      break;
  }

  display.firstPage();
  do {
    switch(currentMenuState) {
      case INFRARED_MENU: 
        drawIRMenu();
        break;
      case INFRARED_MENU_READING:
        infraredMenuReadingScreen();
        break;
      case INFRARED_MENU_READING_DONE:
        char cmdBuffer[6]; sprintf(cmdBuffer, "%hx", irTools.getCurrentIrData().command);
        char addrBuffer[6]; sprintf(addrBuffer, "%hx", irTools.getCurrentIrData().address);
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
        infraredMenuReadingDoneSavingScreen();
        break;
      case INFRARED_MENU_LIST_DONE:
        infraredMenuListDoneScreen();
        break;
      case INFRARED_MENU_READING_ERROR:
      case INFRARED_MENU_SEND_SENDING:
      case INFRARED_MENU_LIST:
      case INFRARED_MENU_LIST_DELETE:
      case INFRARED_MENU_LIST_DELETE_SUCCESS:
      case WIFI_MENU_SCAN:
      case WIFI_MENU_SCAN_SCANNING:
      case WIFI_MENU_SCAN_LIST:
      case WIFI_MENU_DEAUTH:
      case WIFI_MENU_DEAUTH_SCANNING:
      case WIFI_MENU_DEAUTH_LIST:
      case WIFI_MENU_DEAUTH_ATTACKING:
      case WIFI_MENU_STATION:
      case BLE_MENU_SCAN:
      case BLE_MENU_RECEIVE:
      case BLE_MENU_SEND:
      case LORA_MENU_SEND:
      case LORA_MENU_RECEIVE:
      case LORA_MENU_INFO:
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
      case MICROSD_MENU_INFO:
      case MICROSD_MENU_FORMAT:
      case MICROSD_MENU_FORMAT_DONE:
      case MICROSD_MENU_FORMAT_ERROR:
        inDevelopment();
        break;
      case MAIN_MENU: 
      default:
        drawMenu();
    }
    
    // display.setFont(u8g2_font_ncenB14_tr);
    // display.drawStr(0,15,"Hello World!");
  } while (display.nextPage());
}

void MenuSystem::drawMenu()
{
  // selected item background
  display.setFlipMode(0);

  display.drawBitmap(0, 22, 128/8, 21, bitmap_item_sel_outline);

  // draw previous item as icon + label
  display.setFont(u8g_font_7x14);
  display.drawStr(25, 15, menuItems[previousIndex]); 
  display.drawBitmap( 4, 2, 16/8, 16, bitmap_icons[previousIndex]);          

  // draw selected item as icon + label in bold font
  display.setFont(u8g_font_7x14B);    
  display.drawStr(25, 15+20+2, menuItems[currentIndex]);   
  display.setDrawColor(0); // Invert text color for selected item
  display.drawBitmap( 4, 24, 16/8, 16, bitmap_icons[currentIndex]);     
  display.setDrawColor(1); // Invert text color for selected item

  // draw next item as icon + label
  display.setFont(u8g_font_7x14);     
  display.drawStr(25, 15+20+20+2+2, menuItems[nextIndex]);   
  display.drawBitmap( 4, 46, 16/8, 16, bitmap_icons[nextIndex]);        

  // draw scrollbar handle
  display.drawBox(125, 64/itemCount * currentIndex, 3, 64/itemCount);
}
void MenuSystem::drawIRMenu()
{
  // selected item background
  display.drawBitmap(0, 22, 128/8, 21, bitmap_item_sel_outline);

  // draw previous item as icon + label
  display.setFont(u8g_font_7x14);
  display.drawStr(25, 15, menuItems[previousIndex]); 
  display.drawBitmap( 4, 2, 16/8, 16, bitmap_icons[0]);          

  // draw selected item as icon + label in bold font
  display.setFont(u8g_font_7x14B);    
  display.drawStr(25, 15+20+2, menuItems[currentIndex]);   
  display.setDrawColor(0); // Invert text color for selected item
  display.drawBitmap( 4, 24, 16/8, 16, bitmap_icons[0]);     
  display.setDrawColor(1); // Invert text color for selected item

  // draw next item as icon + label
  display.setFont(u8g_font_7x14);     
  display.drawStr(25, 15+20+20+2+2, menuItems[nextIndex]);   
  display.drawBitmap( 4, 46, 16/8, 16, bitmap_icons[0]);        

  // draw scrollbar handle
  display.drawBox(125, 64/itemCount * currentIndex, 3, 64/itemCount);
}
void MenuSystem::drawList()
{
  // selected item background
  int sel = currentIndex % 3;
  if(sel == 0) {
    display.drawBitmap(0, 0, 128/8, 21, bitmap_item_sel_outline);
  } else if (sel == 1) {
    display.drawBitmap(0, 22, 128/8, 21, bitmap_item_sel_outline);
  } else if (sel == 2) {
    display.drawBitmap(0, 44, 128/8, 21, bitmap_item_sel_outline);
  }

  // draw previous item as icon + label
  display.setFont(u8g_font_7x14);
  display.drawStr(7, 15, menuItems[currentIndexP1]); 

  // draw selected item as icon + label in bold font
  display.setFont(u8g_font_7x14B);
  display.drawStr(7, 15+20+2, menuItems[currentIndexP2]);     

  // draw next item as icon + label
  display.setFont(u8g_font_7x14);
  display.drawStr(7, 15+20+20+2+2, menuItems[currentIndexP3]);

  // draw scrollbar handle
  // display.drawBox(125, 64/itemCount * currentIndex, 3, 64/itemCount);
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
void MenuSystem::infraredMenuReadingDoneScreen(char *command, char *address, const char *protocol) {
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
  display.drawStr(0, 14, "IR Code Saving");
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
void MenuSystem::infraredMenuListDoneScreen() {
  drawList();
}
void MenuSystem::infraredMenuListDeleteScreen() {}
void MenuSystem::infraredMenuListDeleteSuccessScreen() {}

void MenuSystem::wifiMenuScanScreen() {}
void MenuSystem::wifiMenuScanScanningScreen() {}
void MenuSystem::wifiMenuScanListScreen() {}
void MenuSystem::wifiMenuDeauthScreen() {}
void MenuSystem::wifiMenuDeauthScanningScreen() {}
void MenuSystem::wifiMenuDeauthListScreen() {}
void MenuSystem::wifiMenuDeauthAttackingScreen() {}
void MenuSystem::wifiMenuStationScreen() {}

void MenuSystem::bleMenuScanScreen() {}
void MenuSystem::bleMenuReceiveScreen() {}
void MenuSystem::bleMenuSendScreen() {}

void MenuSystem::loraMenuSendScreen() {}
void MenuSystem::loraMenuReceiveScreen() {}
void MenuSystem::loraMenuInfoScreen() {}

void MenuSystem::nfcMenuReadingScreen() {}
void MenuSystem::nfcMenuReadingDoneScreen() {}
void MenuSystem::nfcMenuReadingFailedScreen() {}
void MenuSystem::nfcMenuSendScreen() {}
void MenuSystem::nfcMenuSendListScreen() {}
void MenuSystem::nfcMenuSendSendingScreen() {}
void MenuSystem::nfcMenuSendDoneScreen() {}
void MenuSystem::nfcMenuListScreen() {}
void MenuSystem::nfcMenuListDeleteScreen() {}
void MenuSystem::nfcMenuListDeleteSuccessScreen() {}

void MenuSystem::microsdMenuScreen() {}
void MenuSystem::microsdMenuInfoScreen() {}
void MenuSystem::microsdMenuFormatScreen() {}
void MenuSystem::microsdMenuFormatDoneScreen() {}
void MenuSystem::microsdMenuFormatErrorScreen() {}
