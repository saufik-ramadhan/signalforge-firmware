#include "MenuSystem.h"

MenuSystem::MenuSystem(U8G2 &display) : display(display), menuItems(nullptr), itemCount(0), currentIndex(0), topIndex(0) {
    itemsPerPage = display.getDisplayHeight() / 10; // Assuming 10px per item row height
}

void MenuSystem::addMenu(const char **menuItems, uint8_t itemCount) {
    this->menuItems = menuItems;
    this->itemCount = itemCount;
    currentIndex = 0;
    topIndex = 0;
}

void MenuSystem::navigateUp() {
    if (currentIndex > 0) {
        currentIndex--;
        if (currentIndex < topIndex) {
            topIndex = currentIndex;
        }
    }
}

void MenuSystem::navigateDown() {
    if (currentIndex < itemCount - 1) {
        currentIndex++;
        if (currentIndex >= topIndex + itemsPerPage) {
            topIndex++;
        }
    }
}

void MenuSystem::select() {
    // Add custom functionality for when an item is selected
    Serial.println(menuItems[currentIndex]);
}

void MenuSystem::render() {
    display.clearBuffer();
    drawMenu();
    display.sendBuffer();
}

void MenuSystem::drawMenu() {
    uint8_t endIndex = min(topIndex + itemsPerPage, itemCount);
    for (uint8_t i = topIndex; i < endIndex; i++) {
        if (i == currentIndex) {
            display.drawBox(0, (i - topIndex) * 10, display.getDisplayWidth(), 10);
            display.setDrawColor(0); // Invert text color for selected item
        } else {
            display.setDrawColor(1);
        }
        display.setCursor(2, (i - topIndex) * 10 + 9);
        display.print(menuItems[i]);
    }
}
