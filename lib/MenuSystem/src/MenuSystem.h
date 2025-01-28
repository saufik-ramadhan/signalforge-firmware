#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <ButtonHandler.h>
#include <MenuList.h>
#include <IRTools.h>
#include <SDTools.h>

#define MENU_NUM_ITEMS 100
#define MENU_ITEM_LENGTH 15

class MenuSystem {
  public:
        MenuSystem(U8G2 &display, IRTools &irTool, SDTools &sdTools);

        void addMenu(char menuItems[MENU_NUM_ITEMS][MENU_ITEM_LENGTH], size_t itemCount);
        void navigateUp();
        void navigateDown();
        void select();
        void back();
        void render(MenuState currentMenuState);

        // Getter for the current index
        int getCurrentIndex() const;

  private:
        U8G2 &display;
        IRTools &irTools;
        SDTools &sdTools;
        MenuState currentMenuState;
        char menuItems[MENU_NUM_ITEMS][MENU_ITEM_LENGTH];
        uint8_t itemCount;
        int pageState;
        int currentIndex;
        int previousIndex;
        int nextIndex;
        uint8_t topIndex;
        uint8_t itemsPerPage;

        void drawMenu();
        void drawIRMenu();
        void drawList();

        // Screen
        void inDevelopment();

        void infraredMenuReadingScreen();
        void infraredMenuReadingDoneScreen(char *command, char *address, const char *protocol);
        void infraredMenuReadingDoneSavingScreen();
        void infraredMenuReadingErrorScreen();
        void infraredMenuSendScreen();
        void infraredMenuSendListScreen();
        void infraredMenuSendListFailedScreen();
        void infraredMenuSendSendingScreen();
        void infraredMenuListScreen();
        void infraredMenuListDeleteScreen();
        void infraredMenuListDeleteSuccessScreen();

        void wifiMenuScanScreen();
        void wifiMenuScanScanningScreen();
        void wifiMenuScanListScreen();
        void wifiMenuDeauthScreen();
        void wifiMenuDeauthScanningScreen();
        void wifiMenuDeauthListScreen();
        void wifiMenuDeauthAttackingScreen();
        void wifiMenuStationScreen();

        void bleMenuScanScreen();
        void bleMenuReceiveScreen();
        void bleMenuSendScreen();

        void loraMenuSendScreen();
        void loraMenuReceiveScreen();
        void loraMenuInfoScreen();

        void nfcMenuReadingScreen();
        void nfcMenuReadingDoneScreen();
        void nfcMenuReadingFailedScreen();
        void nfcMenuSendScreen();
        void nfcMenuSendListScreen();
        void nfcMenuSendSendingScreen();
        void nfcMenuSendDoneScreen();
        void nfcMenuListScreen();
        void nfcMenuListDeleteScreen();
        void nfcMenuListDeleteSuccessScreen();

        void microsdMenuScreen();
        void microsdMenuInfoScreen();
        void microsdMenuFormatScreen();
        void microsdMenuFormatDoneScreen();
        void microsdMenuFormatErrorScreen();
};

#endif
