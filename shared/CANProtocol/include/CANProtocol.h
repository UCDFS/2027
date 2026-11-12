// Single header file for CAN Protocol.
// All source files need only include this
#pragma once

#include <cstdint>
#include <FlexCAN_T4.h>

#include "CANMessageIDs.h"
#include "CANSignals.h"

namespace CAN
{
  uint16_t floatToUInt16(float value, float min, float max);
  float uint16ToFloat(uint16_t value, float min, float max);
}