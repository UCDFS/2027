#include <Arduino.h>
#include <algorithm>
#include "sensors.h"

// Returns false if apps fails plausibility check, true otherwise
bool readApps(uint8_t* appsPct) {
  int raw1 = analogRead(APPS1_PIN);
  int raw2 = analogRead(APPS2_PIN);

  if (abs(raw1 - raw2) / std::max({raw1, raw2, 1}) >= 0.1f) {
    return false;
  }

  float pct1 = (float)(APPS1_REST - raw1) * 100.0f / (float)(APPS1_REST - APPS1_FULL);
  float pct2 = (float)(APPS2_REST - raw2) * 100.0f / (float)(APPS2_REST - APPS2_FULL);

  pct1 = std::clamp(pct1, 0.0f, 100.0f);
  pct2 = std::clamp(pct2, 0.0f, 100.0f);

  *appsPct = (pct1 + pct2) * 0.5f;

  return true;
}

// readBPS()

// readSuspendion()

// readWheelSpeed()

// readSteeringAngle()