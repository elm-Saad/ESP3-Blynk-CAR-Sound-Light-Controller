#include "lights.h"
#include "pins.h"

// -------------------- STATES --------------------
static bool headLightsOn = false;
static bool backLightsOn = false;

static bool indicatorLeft = false;
static bool indicatorRight = false;
static bool hazardOn = false;

static bool cannonActive = false;

// -------------------- TIMING --------------------
static unsigned long lastBlinkTime = 0;
static const unsigned long BLINK_INTERVAL = 500; //indicators to blink at 500 ms (car-realistic) or faster (300 ms)
static bool blinkState = false;

// Cannon flash timing
static unsigned long cannonStartTime = 0;
static const unsigned long CANNON_DURATION = 3500; //3.5s

// -------------------- INIT --------------------
void lightsInit() {
  pinMode(PIN_HEADLIGHT_L, OUTPUT);
  pinMode(PIN_HEADLIGHT_R, OUTPUT);

  pinMode(PIN_BACKLIGHT_L, OUTPUT);
  pinMode(PIN_BACKLIGHT_R, OUTPUT);

  pinMode(PIN_INDICATOR_F_L, OUTPUT);
  pinMode(PIN_INDICATOR_B_L, OUTPUT);
  pinMode(PIN_INDICATOR_F_R, OUTPUT);
  pinMode(PIN_INDICATOR_B_R, OUTPUT);

  pinMode(PIN_CANNON_L, OUTPUT);
  pinMode(PIN_CANNON_R, OUTPUT);

  digitalWrite(PIN_HEADLIGHT_L, LOW);
  digitalWrite(PIN_HEADLIGHT_R, LOW);

  digitalWrite(PIN_BACKLIGHT_L, LOW);
  digitalWrite(PIN_BACKLIGHT_R, LOW);

  digitalWrite(PIN_INDICATOR_F_L, LOW);
  digitalWrite(PIN_INDICATOR_B_L, LOW);
  digitalWrite(PIN_INDICATOR_F_R, LOW);
  digitalWrite(PIN_INDICATOR_B_R, LOW);

  digitalWrite(PIN_CANNON_L, LOW);
  digitalWrite(PIN_CANNON_R, LOW);
}

// -------------------- SETTERS --------------------
void setHeadLights(bool state) {
  headLightsOn = state;
}

void setBackLights(bool state) {
  backLightsOn = state;
}

void setIndicatorLeft(bool state) {
  indicatorLeft = state;
  if (state) {
    indicatorRight = false;
    hazardOn = false;
  }
}

void setIndicatorRight(bool state) {
  indicatorRight = state;
  if (state) {
    indicatorLeft = false;
    hazardOn = false;
  }
}

void setHazard(bool state) {
  hazardOn = state;
  if (state) {
    indicatorLeft = false;
    indicatorRight = false;
  }
}

void triggerCannonFlash() {
  cannonActive = true;
  cannonStartTime = millis();
}

// -------------------- UPDATE --------------------
void updateLights() {
  unsigned long currentMillis = millis();

  // Blink logic
  if (currentMillis - lastBlinkTime >= BLINK_INTERVAL) {
    lastBlinkTime = currentMillis;
    blinkState = !blinkState;
  }

  // Head & back lights
  digitalWrite(PIN_HEADLIGHT_L, headLightsOn ? HIGH : LOW);
  digitalWrite(PIN_HEADLIGHT_R, headLightsOn ? HIGH : LOW);

  digitalWrite(PIN_BACKLIGHT_L, backLightsOn ? HIGH : LOW);
  digitalWrite(PIN_BACKLIGHT_R, backLightsOn ? HIGH : LOW);

  // Indicators
  bool indicatorOutput = blinkState;

  if (hazardOn) {
    digitalWrite(PIN_INDICATOR_F_L, indicatorOutput);
    digitalWrite(PIN_INDICATOR_B_L, indicatorOutput);
    digitalWrite(PIN_INDICATOR_F_R, indicatorOutput);
    digitalWrite(PIN_INDICATOR_B_R, indicatorOutput);
  } else {
    digitalWrite(PIN_INDICATOR_F_L, indicatorLeft ? indicatorOutput : LOW);
    digitalWrite(PIN_INDICATOR_B_L, indicatorLeft ? indicatorOutput : LOW);

    digitalWrite(PIN_INDICATOR_F_R, indicatorRight ? indicatorOutput : LOW);
    digitalWrite(PIN_INDICATOR_B_R, indicatorRight ? indicatorOutput : LOW);
  }

  // Cannon flash (override)
  if (cannonActive) {
    digitalWrite(PIN_CANNON_L, blinkState ? HIGH : LOW);
    digitalWrite(PIN_CANNON_R, blinkState ? HIGH : LOW);

    if (currentMillis - cannonStartTime >= CANNON_DURATION) {
      cannonActive = false;
      digitalWrite(PIN_CANNON_L, LOW);
      digitalWrite(PIN_CANNON_R, LOW);
    }
  }


}

// -------------------- QUERY --------------------
bool isIndicatorActive() {
  return indicatorLeft || indicatorRight || hazardOn;
}
