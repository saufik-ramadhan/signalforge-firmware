#include <Arduino.h>
#include "WiFi.h"
#include <U8g2lib.h>
#include "pin-definition.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include <Wire.h>
#include "helper.h"
#include <MenuSystem.h>

// Use only core 1
static const BaseType_t app_cpu = 0;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g(U8G2_R2, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL_PIN, /* data=*/ SDA_PIN);   // ESP32 Thing, HW I2C with pin remapping

void toggleLED1(void *parameter) {
  while(1) {
    Serial.println("Task 1 Running ...");
    msDelay(5000);
  }
}

void toggleLED2(void *parameter) {
  while(1) {
    Serial.println("Task 2 Running ...");
    msDelay(3000);
  }
}

void scanI2C(void *parameter) {
  byte error, address;
  int nDevices;

  while(1) {
    Serial.println("Scanning I2C...");
    nDevices = 0;
    for(address = 1; address < 127; address++) {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if(error == 0) {
        Serial.print("I2C device found at adress 0x");
        if (address<16) {
          Serial.print("0");
        }
        Serial.println(address, HEX);
        nDevices++;
      }
      else if (error == 4) {
        Serial.print("Unknown error at address 0x");
        if(address < 16) {
          Serial.print("0");
        }
        Serial.println(address, HEX);
      }
    }

    if (nDevices == 0) {
      Serial.println("No I2C devices found\n");
    } else {
      Serial.println("done\n");
    }

    // delay
    sDelay(3);
  }

}

void setup() {
  uint8_t cardType;
  Wire.begin();
  Serial.begin(115200);

  // GFX
  u8g.begin();

  // Input Buttons
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP); // up button
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP); // down button
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP); // select button
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);

  // Task to run forever
  xTaskCreatePinnedToCore(
    toggleLED1,
    "Toggle LED1",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    toggleLED2,
    "Toggle LED2",
    1024,
    NULL,
    1,
    NULL,
    app_cpu
  );

  xTaskCreatePinnedToCore(
    scanI2C,
    "Scan I2C",
    1024,
    NULL,
    1,
    NULL,
    1
  );
  
}

void loop() {

}