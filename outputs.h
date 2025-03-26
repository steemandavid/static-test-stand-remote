#ifndef OUTPUTS_H
#define OUTPUTS_H

#define THISFILENAME "outputs.h"

#include <Arduino.h>

void toggleOutput(int pin);
void switchOutputOn(int pin);
void switchOutputOff(int pin);

void startToggleTask(int pin, float frequency);
void stopToggleTask(int pin);

void switchPinOnForSeconds(void *param);
void startSwitchPinOnForSeconds(int pin, int seconds);

#endif
