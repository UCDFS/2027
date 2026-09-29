#include <Arduino.h>
#include <FlexCAN_T4.h>

#include "constants.h"
#include "sensors.h"
#include "CAN.h"

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;


int setup() {
  // Initialize CAN bus
  // Initialize sensors
}

int loop() {
  // Read sensors
  // Send CAN frame
}