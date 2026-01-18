## FILE TREE (Arduino-style)

```
ESP32_Car_Controller/
│
├── ESP32_Car_Controller.ino   // MAIN FILE
│
├── config.h                   // WiFi, Blynk, pins
├── pins.h                     // GPIO definitions
│
├── lights.h
├── lights.cpp                 // All LED logic
│
├── sound.h
├── sound.cpp                  // DAC sound engine
│
├── blynk_handlers.h

```
---

## CORE SOFTWARE ARCHITECTURE (How everything talks)

```
BLYNK APP
   ↓ WiFi
ESP32
 ├─ Blynk Handlers
 │    ├─ Update States
 │
 ├─ Engine State Machine
 │    ├─ START
 │    ├─ RUN
 │    └─ STOP
 │
 ├─ Light Controller
 │
 └─ Sound Engine (DAC → PAM8403 → Speaker)
```
---


# ESP32 Car Light & Sound Controller (Blynk IoT)

This project simulates a **real car lighting and sound system** using an **ESP32**, controlled remotely via **Blynk**.  
It includes realistic **headlights, indicators, hazard lights, reverse lights**, and a **priority-based sound engine** using a **PAM8403 amplifier + speaker**.

> ⚠️ The car does NOT move — this is a **lighting + sound control system only**, designed for learning automotive electronics and embedded systems.

---

## 📦 Hardware Used

- ESP32
- PAM8403 Audio Amplifier
- 8Ω Speaker
- LEDs (Headlights, Backlights, Indicators, Cannon Flash)
- Resistors
- Power supply (5V)
- Smartphone with **Blynk App**

---

## Project Architecture

The project is split into **three main systems**:

```

ESP32
├── Lights System   (lights.h / lights.cpp)
├── Sound System    (sound.h / sound.cpp)
└── Blynk Control   (ESP32_Car_Controller.ino)

````

Each system is **non-blocking**, **state-based**, and runs inside `loop()`.

---

## Lighting System

### Features
- Headlights (Left + Right)
- Backlights (Left + Right)
- Indicators (Left / Right)
- Hazard lights
- Cannon flash (emergency effect)

### Behavior
- Indicators blink using `millis()` timing
- Hazard overrides left/right indicators
- Cannon flash temporarily overrides indicator output
- All lighting logic runs in:
```
  updateLights();
````

### Indicator State Access

The function below allows **sound synchronization**:

```
bool isIndicatorActive();
```

---

##  Sound System (DAC + PAM8403)

### Audio Output

* ESP32 DAC pin: **GPIO 25**
* Sample rate: **8 kHz**
* Audio format: **Unsigned 8-bit PCM WAV**
* Audio stored in **PROGMEM**

---

## Sound States (Car-Realistic Model)

### Engine

* `engineStart()` → plays once
* `engineRunning()` → loops continuously
* `engineStop()` → plays once and stops engine sound

### Other Sounds

* `hornTrigger()` → momentary (button press)
* `indicatorSoundOn(true/false)` → synced with indicator blink
* `setReverseSound(true/false)` → looping while ON

---

## Sound Priority System (IMPORTANT)

The project uses **priority-based sound playback** (NOT mixing):

### Priority Order (Highest → Lowest)

1. Horn
2. Reverse Beep
3. Indicator Tick
4. Engine Sound

⚠️ Only **one sound plays at a time**.

When a higher-priority sound plays, it **temporarily interrupts** lower-priority sounds.
Once it finishes, the system **resumes** the appropriate lower-priority sound.

---

## ⚠️ Current Known Limitations

### ❌ No Audio Mixing

* The ESP32 DAC outputs **only one audio stream**
* Sounds are **not mixed**
* Only **priority-based playback** is implemented

Example:

* Engine ON + Horn → engine pauses, horn plays, engine resumes
* Engine ON + Indicator → indicator ticks, engine resumes between ticks

---

### Reverse Beep Volume Too Loud

* `reverseBeepSamples` is **significantly louder**
* Needs:

  * Sample normalization **OR**
  * Software volume scaling **OR**
  * External resistor/amplifier gain adjustment

---

### Indicator Sound Behavior

* Indicator sound is **one-shot per blink**
* Works correctly but **cannot overlap with engine sound**
* Engine resumes immediately after indicator tick

---

## 🧪 Tested & Working

✔ Headlights
✔ Backlights
✔ Indicators & Hazard
✔ Cannon Flash
✔ Engine Start / Run / Stop
✔ Horn (momentary)
✔ Reverse Beep (loop ON / stop OFF)
✔ Blynk Button Sync

---

## 📱 Blynk Controls

| Virtual Pin | Function        |
| ----------- | --------------- |
| V0          | Headlights      |
| V1          | Backlights      |
| V2          | Indicator Left  |
| V3          | Indicator Right |
| V4          | Hazard          |
| V5          | Cannon Flash    |
| V6          | Engine ON / OFF |
| V7          | Horn            |
| V8          | Reverse         |

---

## Recommended Future Updates

### HIGH-VALUE UPGRADES

* 🔊 **Software audio mixing** (engine + indicator)
* 🎚 **Per-sound volume control**
* 🔉 Normalize all WAV samples
* 🎛 Engine RPM simulation (pitch modulation)
* ⏱ Indicator sound timing tied exactly to blink state

### ⚙️ HARDWARE UPGRADES

* I2S DAC (MAX98357A / PCM5102)
* Stereo sound
* Higher quality amplifier
* Dedicated audio task on Core 1

### LOGIC IMPROVEMENTS

* Soft fade-in / fade-out for engine
* Reverse sound ducking engine volume
* Engine idle / acceleration sound layers
* Brake light + brake sound logic

---

## 📌 Future Updates

*(Leave space for upcoming improvements)*

* [ ] Audio mixing
* [ ] Engine RPM control
* [ ] Volume scaling
* [ ] Hardware DAC upgrade
* [ ] UI improvements in Blynk
* [ ] Sound optimization

---

## 🏁 Final Notes

This project demonstrates:

* Embedded state machines
* Real automotive logic
* Non-blocking timing
* Priority-based audio scheduling
* IoT control using Blynk


