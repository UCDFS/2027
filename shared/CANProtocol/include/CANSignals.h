#pragma once

#include "CANProtocol.h"

struct APPSData {
  uint8_t appsPct;
};
constexpr uint8_t APPSDATA_LEN = 1;

struct NodeData {
  uint8_t brakePressure;
  uint16_t leftWheelSpeed;
  uint16_t rightWheelSpeed;
  uint8_t leftSuspension_cm;
  uint8_t rightSuspension_cm;
};
constexpr uint8_t NODEDATA_LEN = 7;

struct IMUAccelData {
  float x;
  float y;
  float z;
};
constexpr uint8_t IMUACCELDATA_LEN = 6;

struct IMUGyroTempData {
  float x;
  float y;
  float z;
  float temperature;
};
constexpr uint8_t IMUGYROTEMPDATA_LEN = 8;

struct InverterCommandData {
  uint8_t registerID;
  uint16_t value;
};
constexpr uint8_t INVERTERCOMMANDDATA_LEN = 3;

struct InverterRequestData {
  uint8_t regRequest;
  uint8_t registerID;
  uint8_t interval_ms;
};
constexpr uint8_t INVERTER_REQUEST_DATA_LEN = 3;

struct InverterReplyData {
  uint8_t registerID;
  uint16_t value;
};
constexpr uint8_t INVERTERREPLYDATA_LEN = 3;

