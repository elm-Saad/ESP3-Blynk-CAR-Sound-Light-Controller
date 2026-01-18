#include "sound.h"
#include "audio_samples.h"
#include "lights.h"

#define DAC_PIN 25
#define AUDIO_RATE 8000
#define SAMPLE_PERIOD_US (1000000 / AUDIO_RATE)

// ---------------- STATES ----------------

// Engine
static bool engineRunning = false;
static bool engineStarting = false;
static bool engineStopping = false;

// Indicator
static bool indicatorSoundEnabled = false;
static bool lastBlinkState = false;

// Reverse
static bool reverseActive = false;

// Priority trigger
static bool hornActive = false;

// Playback state
static const unsigned char* sample = nullptr;
static unsigned int length = 0;
static unsigned int sampleIndex = 0; 
static bool looped = false;

// Timing
static unsigned long lastSampleTime = 0;

// ---------------- LOW LEVEL ----------------

void startSample(const unsigned char* data, unsigned int len, bool loop) {
  sample = data;
  length = len;
  sampleIndex = 0;
  looped = loop;
}

void stopSample() {
  sample = nullptr;
  sampleIndex = 0;
  looped = false;
  dacWrite(DAC_PIN, 0);
}

// ---------------- INIT ----------------

void soundInit() {
  pinMode(DAC_PIN, OUTPUT);
  dacWrite(DAC_PIN, 0);
}

// ---------------- ENGINE ----------------

void engineStart() {
  engineStarting = true;
  engineStopping = false;
  engineRunning = true;
  startSample(engineStartSamples, engineStartLength, false);
}

void engineStop() {
  engineStopping = true;
  engineRunning = false;
  startSample(engineStopSamples, engineStopLength, false);
}

// ---------------- INDICATOR ----------------

void indicatorSoundOn(bool active) {
  indicatorSoundEnabled = active;
}

// ---------------- HORN ----------------

void hornTrigger() {
  hornActive = true;
  startSample(hornSamples, hornSampleLength, false);
}

// ---------------- REVERSE ----------------

void setReverseSound(bool active) {
  reverseActive = active;
}

// ---------------- UPDATE (THE MAGIC) ----------------

void soundUpdate() {
  unsigned long now = micros();

  // --- DAC OUTPUT ---
  if (sample && (now - lastSampleTime >= SAMPLE_PERIOD_US)) {
    lastSampleTime = now;

    dacWrite(DAC_PIN, sample[sampleIndex++]);

    if (sampleIndex >= length) {
      if (looped) {
        sampleIndex = 0;
      } else {
        stopSample();
      }
    }
  }

  // --- PRIORITY LOGIC ---

  
  // --- FORCE STOP REVERSE ---
  if (!reverseActive && sample == reverseBeepSamples) {
    stopSample();
  }

  // Reverse sound (highest priority loop)
  if (!sample && reverseActive) {
    startSample(reverseBeepSamples, reverseBeepLength, true);
    return;
  }

  // Indicator tick synced with blink
  bool blink = isIndicatorActive();
  if (!sample && indicatorSoundEnabled && blink && blink != lastBlinkState) {
    startSample(indicatorSamples, indicatorLength, false);
    lastBlinkState = blink;
    return;
  }

  lastBlinkState = blink;

  // Engine start → engine loop
  if (!sample && engineStarting) {
    engineStarting = false;
    startSample(engineRunSamples, engineRunLength, true);
    return;
  }

  // Engine running loop
  if (!sample && engineRunning && !engineStopping) {
    startSample(engineRunSamples, engineRunLength, true);
    return;
  }

  // Engine stop ends → silence
  if (!sample && engineStopping) {
    engineStopping = false;
    stopSample();
  }
}

