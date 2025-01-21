#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Arduino.h>
#include <U8g2lib.h>

class MenuSystem {
public:
    MenuSystem(U8G2 &display);

    void addMenu(const char **menuItems, uint8_t itemCount);
    void navigateUp();
    void navigateDown();
    void select();
    void render();

private:
    U8G2 &display;
    const char **menuItems;
    uint8_t itemCount;
    uint8_t currentIndex;
    uint8_t topIndex;
    uint8_t itemsPerPage;

    void drawMenu();
};

#endif
