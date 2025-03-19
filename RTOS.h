#ifndef RTOS_H
#define RTOS_H


// Declare the task handles as extern to use them in other files
extern TaskHandle_t xHandleMainLoop;
extern TaskHandle_t xHandleUpdateDisplay;
extern TaskHandle_t xHandleIOhandler;
extern TaskHandle_t xHandleButton_handler;


void MainLoop(void *parameter);
void SetupRTOSTasks();

#endif