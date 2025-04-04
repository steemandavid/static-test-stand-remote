#ifndef RTOS_H
#define RTOS_H

#define THISFILENAME "RTOS.h"


// define priorities for RTOS tasks 
#define BUZZERBEEPTASKPRIORITY 5
#define MAINLOOPTASKPRIORITY 10
#define BUTTONTASKPRIORITY 4
#define TOGGLETASKPRIORITY 5
#define UPDATEDISPLAYTASKPRIORITY 5
#define BATTERYTASKPRIORITY 3


// define on which core tasks run
// Core 0 = wifi stack, core 1 = free (critical tasks should run here)
// if the core doesn't matter, use tskNO_AFFINITY
#define BUZZERBEEPTASKCORE 1
#define MAINLOOPTASKCORE 1
#define BUTTONTASKCORE 1
#define TOGGLETASKCORE 1
#define UPDATEDISPLAYTASKCORE 0
#define BATTERYTASKCORE 0



// Declare the task handles as extern to use them in other files
extern TaskHandle_t xHandleMainLoop;
extern TaskHandle_t xHandleUpdateDisplay;
extern TaskHandle_t xHandleIOhandler;
extern TaskHandle_t xHandleButton_handler;
extern TaskHandle_t xHandleUpdateDisplay;


void MainLoop(void *parameter);
void SetupRTOSTasks();

#endif