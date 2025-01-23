#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(int upPin, int downPin, int leftPin, int rightPin, int backPin, int okPin, ButtonCallback callback) {
    buttons[BUTTON_UP] = {upPin, 0, 0, false, false, false};
    buttons[BUTTON_DOWN] = {downPin, 0, 0, false, false, false};
    buttons[BUTTON_LEFT] = {leftPin, 0, 0, false, false, false};
    buttons[BUTTON_RIGHT] = {rightPin, 0, 0, false, false, false};
    buttons[BUTTON_BACK] = {backPin, 0, 0, false, false, false};
    buttons[BUTTON_OK] = {okPin, 0, 0, false, false, false};
    this->callback = callback;

    for (int i = 0; i < 6; ++i) {
        if(buttons[i].pin != -1) pinMode(buttons[i].pin, INPUT_PULLUP);
    }
}

void ButtonHandler::update() {
    unsigned long currentTime = millis();

    for (int i = 0; i < 6; ++i) {
        handleButton(buttons[i], static_cast<ButtonType>(i));
    }
}

void ButtonHandler::handleButton(Button& button, ButtonType type) {
    unsigned long currentTime = millis();
    bool isPressed = digitalRead(button.pin) == LOW;

    if (isPressed && !button.debounce) {
        button.debounce = true;
        button.pressStartTime = currentTime;

        if (currentTime - button.lastPressTime <= DOUBLE_PRESS_DELAY) {
            callback(type, DOUBLE_PRESS);
            button.singlePress = false;
        } else {
            button.singlePress = true;
        }

        button.lastPressTime = currentTime;
    }

    if (!isPressed && button.debounce) {
        button.debounce = false;
        if (button.singlePress && currentTime - button.pressStartTime < LONG_PRESS_DELAY) {
            callback(type, SHORT_PRESS);
        }
        button.singlePress = false;
    }

    if (isPressed && (currentTime - button.pressStartTime >= LONG_PRESS_DELAY) && button.singlePress) {
        callback(type, LONG_PRESS);
        button.singlePress = false;
    }
}