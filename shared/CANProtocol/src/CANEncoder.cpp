#include "CANEncoder.h"

namespace CAN
{  
  // ---------- Powertrain Bus ----------
  CAN_message_t encodeInverterCommand(struct InverterCommandData data) {
    CAN_message_t msg = {0};
    msg.id = CAN_ID::INVERTER_COMMAND;
    msg.len = 3;
    msg.buf[0] = data.registerID;
    msg.buf[1] = data.value & 0xFF;
    msg.buf[2] = (data.value >> 8) & 0xFF;
    return msg;
  }

  CAN_message_t encodeInverterRequest(struct InverterRequestData data) {
    CAN_message_t msg = {0};
    msg.id = CAN_ID::INVERTER_COMMAND;
    msg.len = 3;
    msg.buf[0] = data.regRequest;
    msg.buf[1] = data.registerID;
    msg.buf[2] = data.interval_ms;
    return msg;
  }

  // ---------- Sensor Bus ----------
  CAN_message_t encodeAPPSData(struct APPSData data) {
    CAN_message_t msg = {0};
    msg.id = CAN_ID::APPS_DATA;
    msg.len = 4;
    msg.buf[0] = data.apps1 & 0xFF;
    msg.buf[1] = (data.apps1 >> 8) & 0xFF;
    msg.buf[2] = data.apps2 & 0xFF;
    msg.buf[3] = (data.apps2 >> 8) & 0xFF;
    return msg;
  }

  // Remember to attach msg.id after
  CAN_message_t encodeNodeData(struct NodeData data) {
    CAN_message_t msg = {0};
    msg.len = 7;
    msg.buf[0] = data.brakePressure;
    msg.buf[1] = data.leftWheelSpeed & 0xFF;
    msg.buf[2] = (data.leftWheelSpeed >> 8) & 0xFF;
    msg.buf[3] = data.rightWheelSpeed & 0xFF;
    msg.buf[4] = (data.rightWheelSpeed >> 8) & 0xFF;
    msg.buf[5] = data.leftSuspension_cm;
    msg.buf[6] = data.rightSuspension_cm;
    return msg;
  }

  CAN_message_t encodeIMUAccelData(struct IMUAccelData data) {
    CAN_message_t msg = {0};
    msg.id = CAN_ID::IMU_ACCEL;
    msg.len = 6;
    // TODO: choose appropriate min/max values and set constant
    uint16_t xInt = floatToUInt16(data.x, -100.0f, 100.0f);
    uint16_t yInt = floatToUInt16(data.y, -100.0f, 100.0f);
    uint16_t zInt = floatToUInt16(data.z, -100.0f, 100.0f);
    msg.buf[0] = xInt & 0xFF;
    msg.buf[1] = (xInt >> 8) & 0xFF;
    msg.buf[2] = yInt & 0xFF;
    msg.buf[3] = (yInt >> 8) & 0xFF;
    msg.buf[4] = zInt & 0xFF;
    msg.buf[5] = (zInt >> 8) & 0xFF;
    return msg;
  }

  CAN_message_t encodeIMUGyroTempData(struct IMUGyroTempData data) {
    CAN_message_t msg = {0};
    msg.id = CAN_ID::IMU_GYRO_TEMP;
    msg.len = 8;
    // TODO: check imu gyro values
    uint16_t xInt = floatToUInt16(data.x, -180.0f, 180.0f);
    uint16_t yInt = floatToUInt16(data.y, -180.0f, 180.0f);
    uint16_t zInt = floatToUInt16(data.z, -180.0f, 180.0f);
    uint16_t tempInt = floatToUInt16(data.temperature, -50.0f, 150.0f);
    msg.buf[0] = xInt & 0xFF;
    msg.buf[1] = (xInt >> 8) & 0xFF;
    msg.buf[2] = yInt & 0xFF;
    msg.buf[3] = (yInt >> 8) & 0xFF;
    msg.buf[4] = zInt & 0xFF;
    msg.buf[5] = (zInt >> 8) & 0xFF;
    msg.buf[6] = tempInt & 0xFF;
    msg.buf[7] = (tempInt >> 8) & 0xFF;
    return msg;
  }

}