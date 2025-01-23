#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <ButtonHandler.h>
#include <MenuList.h>
#include <IRTools.h>

class MenuSystem
{
public:
  MenuSystem(U8G2 &display, IRTools &irTool);

  void addMenu(const char **menuItems, uint8_t itemCount);
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
  MenuState currentMenuState;
  const char **menuItems;
  uint8_t itemCount;
  int pageState;
  int currentIndex;
  int previousIndex;
  int nextIndex;
  uint8_t topIndex;
  uint8_t itemsPerPage;

  void drawMenu();
  void drawIRMenu();

  // Screen
  void inDevelopment();

  void infraredMenuReadingScreen();
  void infraredMenuReadingDoneScreen(char * command, char * address, const char * protocol);
  void infraredMenuReadingDoneSavingScreen();
  void infraredMenuReadingErrorScreen();
  void infraredMenuSendScreen();
  void infraredMenuSendListScreen();
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
