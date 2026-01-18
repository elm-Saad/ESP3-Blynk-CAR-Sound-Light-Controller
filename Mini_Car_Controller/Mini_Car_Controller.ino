#include "blynk_config.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include "lights.h"
#include "sound.h"

// Wi-Fi credentials
#define ssid "your_wifi_name"
#define pass "your_password"

// ===== BLYNK CALLBACK (GLOBAL SCOPE) =====

BLYNK_WRITE(V0) {  // Headlights
  setHeadLights(param.asInt());
}

BLYNK_WRITE(V1) {  // Backlights
  setBackLights(param.asInt());
}

BLYNK_WRITE(V2) {  // Indicator LEFT
  if (param.asInt()) {
    setIndicatorLeft(true);
    indicatorSoundOn(true);        //  START indicator sound

    Blynk.virtualWrite(V3, 0); // turn OFF right button
    Blynk.virtualWrite(V4, 0); // turn OFF hazard button
  } else {
    setIndicatorLeft(false);
    indicatorSoundOn(false);       // STOP indicator sound
  }
}

BLYNK_WRITE(V3) {  // Indicator RIGHT
  if (param.asInt()) {
    setIndicatorRight(true);
    indicatorSoundOn(true);        //  START indicator sound

    Blynk.virtualWrite(V2, 0); // turn OFF left button
    Blynk.virtualWrite(V4, 0); // turn OFF hazard button
  } else {
    setIndicatorRight(false);
    indicatorSoundOn(false);       //  STOP indicator sound
  }
}

BLYNK_WRITE(V4) {  // Hazard
  if (param.asInt()) {
    setHazard(true);
    indicatorSoundOn(true);        // START indicator sound

    Blynk.virtualWrite(V2, 0); // turn OFF left
    Blynk.virtualWrite(V3, 0); // turn OFF right
  } else {
    setHazard(false);
    indicatorSoundOn(false);       //  STOP indicator sound
  }
}

BLYNK_WRITE(V5) {  // Cannon
  if (param.asInt()) {
    triggerCannonFlash();
  }
}

// ENGINE ON / OFF
BLYNK_WRITE(V6) {
  if (param.asInt()) {
    engineStart();
  } else {
    engineStop();
  }
}

// HORN
BLYNK_WRITE(V7) {
  if (param.asInt()) hornTrigger();
}

// REVERSE
BLYNK_WRITE(V8) {
  if (param.asInt()) {
    setBackLights(true);
    setReverseSound(true);
  } else {
    setBackLights(false);
    setReverseSound(false);
  }
}


void setup() {
  Serial.begin(115200);
  lightsInit();             // LED pin setup
  soundInit();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

}

void loop() {
  Blynk.run(); // Handle Blynk
  updateLights();  // blinking logic, etc
  soundUpdate();
}
