#include <Arduino.h>
#include <FlexCAN_T4.h>


FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;

void setup() {
  // Initialize CAN bus
  Can1.begin();
  Can1.setBaudRate(500000);

  // Initialize sensors

}

void loop() {
  uint8_t heartbeatStatus = 0x00; // Placeholder
  Can1.write(CAN::encodeFrontNodeHeartbeat(heartbeatStatus));
  // Read sensors
  // Send CAN frame
}