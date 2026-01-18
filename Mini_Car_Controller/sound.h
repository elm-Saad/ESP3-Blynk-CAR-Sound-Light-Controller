#ifndef SOUND_H
#define SOUND_H

#include <Arduino.h>

// Init
void soundInit();

// Must be called in loop()
void soundUpdate();

// ===== ENGINE =====
void engineStart();
void engineStop();

// ===== INDICATOR =====
void indicatorSoundOn(bool active);

// ===== HORN =====
void hornTrigger();

// ===== REVERSE =====
void setReverseSound(bool active);

#endif