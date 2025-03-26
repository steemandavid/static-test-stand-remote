#define THISFILENAME "RTOS.cpp"
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <Arduino.h>
#include <esp_log.h>
#include "RTOS.h"
#include "SSD1306.h"
#include "espnow.h"
#include "handlemessage.h"

static const char *TAG = THISFILENAME;

// RTOS task handles
TaskHandle_t xHandleMainLoop = NULL;
TaskHandle_t xHandleUpdateDisplay = NULL;
TaskHandle_t xHandleIOhandler = NULL;
TaskHandle_t xHandleButton_handler = NULL;
TaskHandle_t xUpdateDisplayTaskHandle = NULL;

void MainLoopTask(void *parameter) {
  ESP_LOGI(TAG, "Task MainLoop started.");

  while(1) {
//    ESP_LOGI(TAG, "%%");
    if (MessageReceivedFlag) { // new ESPNOW message has been received and is available
      handleMessage(MessageReceived);
      MessageReceivedFlag = false; // reset flag
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);   // xxx debug
  }
}

void SetupRTOSTasks() {
  ESP_LOGI(TAG, "Starting SetupRTOSTasks()...");
  // Function name of the task, Name of the task, Stack size (bytes), Parameter to pass, Task priority, Task handle, pin to a specific Core

  // Create MainLoop task
  BaseType_t xReturned;
  xReturned = xTaskCreatePinnedToCore(MainLoopTask, "MainLoopTask", 4096, NULL, MAINLOOPTASKPRIORITY, &xHandleMainLoop, MAINLOOPTASKCORE);
  if (xReturned != pdPASS) {
    ESP_LOGE(TAG, "Error: MainLoop task creation failed.");
  } else {
    ESP_LOGI(TAG, "Pass: MainLoop task creation successful.");
  }
  ESP_LOGI(TAG, "Free heap after creating Task: %d", xPortGetFreeHeapSize());

  // Create display handler task
  xReturned = xTaskCreatePinnedToCore(UpdateDisplayTask, "UpdateDisplayTask", 4096, NULL, UPDATEDISPLAYTASKPRIORITY, &xHandleUpdateDisplay, UPDATEDISPLAYTASKCORE);
  if (xReturned != pdPASS) {
    ESP_LOGE(TAG, "Error: UpdateDisplay task creation failed.");
  } else {
    ESP_LOGI(TAG, "Pass: UpdateDisplay task creation successful.");
  }
  ESP_LOGI(TAG, "Free heap after creating Task: %d", xPortGetFreeHeapSize());

  // vTaskSuspend(xHandleUpdateDisplay);
  // vTaskSuspend(xHandleButton_handler);
  // vTaskSuspend(xHandleIOhandler);
  // vTaskSuspend(xHandleMainLoopTask);
  ESP_LOGI(TAG, "SetupRTOSTasks(): vTaskSuspend completed.");
  // ESP_LOGI(TAG, "SetupRTOSTasks(): Starting scheduler.");
  // vTaskStartScheduler();
  // ESP_LOGI(TAG, "SetupRTOSTasks(): Started scheduler.");
}
