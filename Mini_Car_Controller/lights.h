// All light states & functions

#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>

// Public control functions
void lightsInit();
void updateLights();

// Light controls
void setHeadLights(bool state);
void setBackLights(bool state);
void setIndicatorLeft(bool state);
void setIndicatorRight(bool state);
void setHazard(bool state);
void triggerCannonFlash();

// Query states (for sound sync )
bool isIndicatorActive();

#endif
