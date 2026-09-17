#include <Arduino.h>
#include <FlexCAN_T4.h>

#include "constants.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;


int setup() {

}

int loop() {

}

float readAPPS() {
  int16_t apps1Raw = analogRead(APPS1_PIN);
  int16_t apps2Raw = analogRead(APPS2_PIN);

  float pct1 = (float)(APPS1_REST - apps1Raw) * 100.0f / (float)(APPS1_REST - APPS1_FULL);
  float pct2 = (float)(APPS2_REST - apps2Raw) * 100.0f / (float)(APPS2_REST - APPS2_FULL);

  float avg = pct1 + pct2 * 0.5f;
  return std::clamp(avg, 0.0f, 100.0f);
}

float readBPS() {
  int16_t bpsRaw = analogRead(BPS_PIN);
  int16_t delta = BPS_REST - bpsRaw;
  // TODO: calibrate bps against reference gauge and implement adcToBar() like in TEENSY repo

  // placeholder
  return 0.0f;
}

// readSuspendion(leftOrRightInput)

// readWheelSpeed(leftOrRightInput)

// readSteeringAngle()

void sendCanFrame() {
  // Construct custom CAN frame then send it
}