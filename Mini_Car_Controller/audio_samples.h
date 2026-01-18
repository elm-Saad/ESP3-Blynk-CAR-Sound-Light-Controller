#ifndef AUDIO_SAMPLES_H
#define AUDIO_SAMPLES_H

#include <Arduino.h>

// ===== HORN =====
extern const unsigned char hornSamples[];
extern const unsigned int hornSampleLength;

// ===== ENGINE =====
extern const unsigned char engineStartSamples[];
extern const unsigned int engineStartLength;

extern const unsigned char engineRunSamples[];
extern const unsigned int engineRunLength;

extern const unsigned char engineStopSamples[];
extern const unsigned int engineStopLength;

// ===== INDICATOR =====
extern const unsigned char indicatorSamples[];
extern const unsigned int indicatorLength;

// ===== REVERSE =====
extern const unsigned char reverseBeepSamples[];
extern const unsigned int reverseBeepLength;

#endif
