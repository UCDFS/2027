#pragma once

#include "CANProtocol.h"

namespace CAN
{
  InverterReplyData decodeInverterReply(const CAN_message_t &msg);

  APPSData decodeAPPSData(const CAN_message_t &msg);
  NodeData decodeNodeData(const CAN_message_t &msg);
  IMUAccelData decodeIMUAccelData(const CAN_message_t &msg);
  IMUGyroTempData decodeIMUGyroTempData(const CAN_message_t &msg);
}
