
// define priorities for RTOS tasks 
#define BUZZERBEEPTASKPRIORITY 5
#define MAINLOOPTASKPRIORITY 10
#define BUTTONTASKPRIORITY 4
#define TOGGLETASKPRIORITY 5

// define on which core tasks run
// Core 0 = wifi stack, core 1 = free (critical tasks should run here)
// if the core doesn't matter, use tskNO_AFFINITY
#define BUZZERBEEPTASKCORE 1
#define MAINLOOPTASKCORE 1
#define BUTTONTASKCORE 1
#define TOGGLETASKCORE 1
