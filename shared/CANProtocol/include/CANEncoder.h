#pragma once

#include "CANProtocol.h"

namespace CAN
{
  CAN_message_t encodeInverterCommand(InverterCommandData data);
  CAN_message_t encodeInverterRequest(InverterRequestData data);

  CAN_message_t encodeAPPSData(APPSData data);
  CAN_message_t encodeNodeData(NodeData data);
  CAN_message_t encodeIMUAccelData(IMUAccelData data);
  CAN_message_t encodeIMUGyroTempData(IMUGyroTempData data);
}
