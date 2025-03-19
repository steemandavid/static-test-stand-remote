#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "RTOS.h"
#include "tasksettings.h"

// RTOS task handles
TaskHandle_t xHandleMainLoop = NULL;
TaskHandle_t xHandleUpdateDisplay = NULL;
TaskHandle_t xHandleIOhandler = NULL;
TaskHandle_t xHandleButton_handler = NULL;


void MainLoopTask(void *parameter) {
  Serial.println("Task MainLoop started.");

  while(1) {
    Serial.print("%%%");
    vTaskDelay( 500 / portTICK_PERIOD_MS );   //xxx debug
  }
}

void SetupRTOSTasks() {
  Serial.println("Starting SetupRTOSTasks()...");
  // Function name of the task, Name of the task, Stack size (bytes), Parameter to pass, Task priority, Task handle, pin to a specific Core
  BaseType_t xReturned;
  xReturned = xTaskCreatePinnedToCore(MainLoopTask, "MainLoopTask", 4096, NULL, MAINLOOPTASKPRIORITY, &xHandleMainLoop, MAINLOOPTASKCORE);
  if (xReturned != pdPASS) {
    Serial.println(F("Error: MainLoop task creation failed."));
  } else {
    Serial.println(F("Pass: MainLoop task creation succesfull."));
  }
  Serial.print("Free heap after creating Task: ");
  Serial.println(xPortGetFreeHeapSize());

//  vTaskSuspend(xHandleUpdateDisplay);
//  vTaskSuspend(xHandleButton_handler);
//  vTaskSuspend(xHandleIOhandler);
//  vTaskSuspend(xHandleMainLoopTask);
  Serial.println("SetupRTOSTasks(): vTaskSuspend completed.");
  //Serial.println("SetupRTOSTasks(): Starting scheduler.");
  //vTaskStartScheduler();
  //Serial.println("SetupRTOSTasks(): Started scheduler.");
}