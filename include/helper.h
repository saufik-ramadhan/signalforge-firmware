#include <cstdint>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void msDelay(uint16_t miliseconds) {
  vTaskDelay(miliseconds / portTICK_PERIOD_MS);
}

void sDelay(uint8_t seconds) {
  uint16_t ms = seconds * 1000;
  vTaskDelay(ms / portTICK_PERIOD_MS);
}