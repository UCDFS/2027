#include <Arduino.h>
#include <FlexCAN_T4.h>



FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can2;


void setup() {
  // Initialize CAN bus
  Can2.begin();
  Can2.setBaudRate(500000);
  // Initialize sensors
}

void loop() {
  // Read sensors
  // Send CAN frame
}