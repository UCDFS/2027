#pragma once

#include "CANProtocol.h"

struct APPSData {
  uint8_t appsPct1;
  uint8_t appsPct2;
};

struct NodeData {
  uint8_t brakePressure;
  uint16_t leftWheelSpeed;
  uint16_t rightWheelSpeed;
  uint8_t leftSuspension_cm;
  uint8_t rightSuspension_cm;
};

struct IMUAccelData {
  float x;
  float y;
  float z;
};

struct IMUGyroTempData {
  float x;
  float y;
  float z;
  float temperature;
};

struct InverterCommandData {
  uint8_t registerID;
  uint16_t value;
};

struct InverterRequestData {
  uint8_t regRequest;
  uint8_t registerID;
  uint8_t interval_ms;
};

struct InverterReplyData {
  uint8_t registerID;
  uint16_t value;
};

