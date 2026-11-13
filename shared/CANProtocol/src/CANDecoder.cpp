#include "CANDecoder.h"

namespace CAN
{
  // ---------- Powertrain Bus ----------
  struct InverterReplyData decodeInverterReply(const CAN_message_t &msg) {
    struct InverterReplyData data;
    data.registerID = msg.buf[0];
    data.value = msg.buf[1] | (msg.buf[2] << 8);
    return data;
  }

  // ---------- Sensor Bus ----------
  struct APPSData decodeAPPSData(const CAN_message_t &msg) {
    struct APPSData data;
    data.appsPct1 = msg.buf[0];
    data.appsPct2 = msg.buf[1];
    return data;
  }

  struct NodeData decodeNodeData(const CAN_message_t &msg) {
    struct NodeData data;
    data.brakePressure = msg.buf[0];
    data.leftWheelSpeed = msg.buf[1] | (msg.buf[2] << 8);
    data.rightWheelSpeed = msg.buf[3] | (msg.buf[4] << 8);
    data.leftSuspension_cm = msg.buf[5];
    data.rightSuspension_cm = msg.buf[6];
    return data;
  }

  uint8_t decodeNodeHeartbeat(const CAN_message_t &msg) {
    return msg.buf[0];
  }

  struct IMUAccelData decodeIMUAccelData(const CAN_message_t &msg) {
    struct IMUAccelData data;
    uint16_t xRaw = msg.buf[0] | (msg.buf[1] << 8);
    uint16_t yRaw = msg.buf[2] | (msg.buf[3] << 8);
    uint16_t zRaw = msg.buf[4] | (msg.buf[5] << 8);
    data.x = uint16ToFloat(xRaw, -100.0f, 100.0f);
    data.y = uint16ToFloat(yRaw, -100.0f, 100.0f);
    data.z = uint16ToFloat(zRaw, -100.0f, 100.0f);
    return data;
  }

  struct IMUGyroTempData decodeIMUGyroTempData(const CAN_message_t &msg) {
    struct IMUGyroTempData data;
    uint16_t xRaw = msg.buf[0] | (msg.buf[1] << 8);
    uint16_t yRaw = msg.buf[2] | (msg.buf[3] << 8);
    uint16_t zRaw = msg.buf[4] | (msg.buf[5] << 8);
    uint16_t tempRaw = msg.buf[6] | (msg.buf[7] << 8);
    data.x = uint16ToFloat(xRaw, -180.0f, 180.0f);
    data.y = uint16ToFloat(yRaw, -180.0f, 180.0f);
    data.z = uint16ToFloat(zRaw, -180.0f, 180.0f);
    data.temperature = uint16ToFloat(tempRaw, -50.0f, 150.0f);
    return data;
  }
}