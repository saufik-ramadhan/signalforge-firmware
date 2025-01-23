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
#include "pin-definition.h"
#include "helper.h"
#include "global-configs.h"

// Top Level Declaration
static const BaseType_t app_cpu = 0;
MenuState currentMenuState = MAIN_MENU;
int menuIdx = 0;


/** --- Button Handler and Pipeline (queue) --- */
QueueHandle_t buttonEventQueue;
void handleButtonEvent(ButtonType button, ButtonState state)
{
  ButtonEvent event = {button, state};

  // Send the event to the queue
  if (xQueueSend(buttonEventQueue, &event, 0) != pdPASS)
  {
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
    handleButtonEvent);
void buttonTask(void *parameter)
{
  Serial.println("Task Button handler is active");
  while (1)
  {
    buttonHandler.update();
    msDelay(10);
  }
}
/** --- END --- */

/** --- IR Transceiver Handler and Tasks --- */
void receiverCallback() { currentMenuState = INFRARED_MENU_READING_DONE; }
IRTools irTools(receiverCallback);
void irTask(void *parameter) {
  irTools.begin();
  while(1) {
    switch(currentMenuState) {
      case INFRARED_MENU_READING:
        irTools.receive();
        break;
      case INFRARED_MENU_READING_DONE:
        // irTools.resumeReceive();
        break;
      default:
        break;
    }
    msDelay(10);
  }
}
/** END --- */

/** --- Menu Handler and task (OLED --- u8g2 Library) --- */
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE, /* clock=*/SCL_PIN, /* data=*/SDA_PIN); // ESP32 Thing, HW I2C with pin remapping
MenuSystem menu(u8g2, irTools);
void menuTask(void *parameter)
{
  menu.addMenu(menuItems, 5); // adding menus
  Serial.println("Menu Task Running :: SUCCESS");
  while (1)
  {
    menu.render(currentMenuState);
  }
}
void menuNavigation(ButtonEvent event)
{
  menuIdx = menu.getCurrentIndex();
  switch (currentMenuState)
  {
  case MAIN_MENU:
    if (event.button == BUTTON_RIGHT)
    {
      switch (menuIdx)
      {
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
    }
    else if (event.button == BUTTON_LEFT)
    {
      menu.addMenu(menuItems, 5);
      currentMenuState = MAIN_MENU;
    }
    break;

  case INFRARED_MENU:
    if (event.button == BUTTON_RIGHT)
    {
      switch (menuIdx)
      {
      case 0:
        currentMenuState = INFRARED_MENU_SEND_LIST;
        break;
      case 1:
        currentMenuState = INFRARED_MENU_READING;
        break;
      case 2:
        currentMenuState = INFRARED_MENU_LIST;
        break;
      }
    }
    else if (event.button == BUTTON_LEFT)
    {
      menu.addMenu(menuItems, 5);
      currentMenuState = MAIN_MENU;
    }
    break;

  case NFC_MENU:
    if (event.button == BUTTON_RIGHT)
    {
      switch (menuIdx)
      {
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
    }
    else if (event.button == BUTTON_LEFT)
    {
      menu.addMenu(menuItems, 5);
      currentMenuState = MAIN_MENU;
    }
    break;

  case INFRARED_MENU_READING:
  case INFRARED_MENU_READING_DONE:
  case INFRARED_MENU_READING_DONE_SAVING:
  case INFRARED_MENU_READING_ERROR:
  case INFRARED_MENU_SEND:
  case INFRARED_MENU_SEND_LIST:
  case INFRARED_MENU_SEND_SENDING:
  case INFRARED_MENU_LIST:
  case INFRARED_MENU_LIST_DELETE:
  case INFRARED_MENU_LIST_DELETE_SUCCESS:
    if (event.button == BUTTON_LEFT)
    {
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
    if (event.button == BUTTON_LEFT)
    {
      currentMenuState = NFC_MENU;
    }
    break;
  }
}
void buttonMenuTask(void *parameter)
{
  ButtonEvent event;
  while (true)
  {
    // Wait for an event to arrive in the queue
    if (xQueueReceive(buttonEventQueue, &event, portMAX_DELAY) == pdPASS)
    {
      // Process the event
      switch (event.button)
      {
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

void setup()
{
  Serial.begin(115200);
  u8g2.begin();                                             // define oled
  buttonEventQueue = xQueueCreate(10, sizeof(ButtonEvent)); // define queuer for button
  if (buttonEventQueue == NULL)
  {
    Serial.println("Failed to create queue!");
  }

  /* --- Tasks --- */
  //--- 1 Button Handler Task
  xTaskCreatePinnedToCore(buttonTask, "Button Task", 1024, NULL, 0, NULL, app_cpu);
  xTaskCreatePinnedToCore(buttonMenuTask, "Button Menu Task", 1024, NULL, 1, NULL, app_cpu);
  //--- 2 Button Menu Task Task (OLED1306)
  xTaskCreatePinnedToCore(menuTask, "Menu Task", 2048, NULL, 1, NULL, app_cpu);
  //--- 3 Ir Tools Task ()
  xTaskCreatePinnedToCore(irTask, "IR Transceiver Task", 2048, NULL, 1, NULL, 1);
  //--- 4 Wifi Tools Task ()
  // xTaskCreatePinnedToCore(wifiTask, "Wifi Tools Task", 1024, NULL, 1, NULL, app_cpu);
  //--- 5 NFC Tools Task ()
  // xTaskCreatePinnedToCore(nfcTask, "NFC Tools Task", 1024, NULL, 1, NULL, app_cpu);
}

void loop()
{
}