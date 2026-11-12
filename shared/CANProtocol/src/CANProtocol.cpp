#include <algorithm>
#include "CANProtocol.h"

namespace CAN
{
  // Read about CAN frame structure in 2027/docs/can-design/new.md

  // ---------- float to int conversions ----------

  // Expresses the float on a scale of 0-65536 between min and max
  // For example, if a value could range between -10 and 10, and the value is
  // 0, then the int value would be 32768, halfway between 0 and 65536.
  uint16_t floatToUInt16(float value, float min, float max) {
    value = std::clamp(value, min, max);
    return static_cast<uint16_t>((value - min) / (max - min) * 65535.0f);
  }

  // Opposite of floatToUInt16, read above for explanation
  float uint16ToFloat(uint16_t value, float min, float max) {
    return static_cast<float>(value) / 65535.0f * (max - min) + min;
  }

}