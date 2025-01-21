#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Arduino.h>

#define DEBOUNCE_DELAY 50 // ms
#define LONG_PRESS_DELAY 1000 // ms
#define DOUBLE_PRESS_DELAY 500 // ms

enum ButtonState {
    NONE,
    SHORT_PRESS,
    LONG_PRESS,
    DOUBLE_PRESS
};

enum ButtonType {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_BACK,
    BUTTON_OK
};

typedef void (*ButtonCallback)(ButtonType button, ButtonState state);

class ButtonHandler {
public:
    ButtonHandler(int upPin, int downPin, int leftPin, int rightPin, int backPin, int okPin, ButtonCallback callback);
    void update();

private:
    struct Button {
        int pin;
        unsigned long lastPressTime;
        unsigned long pressStartTime;
        bool pressed;
        bool debounce;
        bool singlePress;
    };

    Button buttons[6];
    ButtonCallback callback;

    void handleButton(Button& button, ButtonType type);
};

#endif