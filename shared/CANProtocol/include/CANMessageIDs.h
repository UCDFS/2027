#pragma once

#include "CANProtocol.h"

namespace CAN_ID {
  // SENSOR NETWORK IDs
  constexpr uint16_t APPS_DATA        = 0x010;
  constexpr uint16_t FRONT_SENSORS    = 0x020;
  constexpr uint16_t REAR_SENSORS     = 0x028;
  constexpr uint16_t IMU_ACCEL        = 0x030;
  constexpr uint16_t IMU_GYRO_TEMP    = 0x031;

  constexpr uint16_t FRONT_HEARTBEAT  = 0x005;
  constexpr uint16_t REAR_HEARTBEAT   = 0x006;

  // POWERTRAIN NETWORK IDs
  constexpr uint16_t INVERTER_REPLY   = 0x181;
  constexpr uint16_t INVERTER_COMMAND = 0x201;
  constexpr uint16_t BMS_PACK_STATUS  = 0x500;
  constexpr uint16_t BMS_FAULTS       = 0x501;

  constexpr uint16_t BMS_CELL_VOLTAGES_0  = 0x510;
  constexpr uint16_t BMS_CELL_VOLTAGES_1  = 0x511;
  constexpr uint16_t BMS_CELL_VOLTAGES_2  = 0x512;
  constexpr uint16_t BMS_CELL_VOLTAGES_3  = 0x513;
  constexpr uint16_t BMS_CELL_VOLTAGES_4  = 0x514;
  constexpr uint16_t BMS_CELL_VOLTAGES_5  = 0x515;
  constexpr uint16_t BMS_CELL_VOLTAGES_6  = 0x516;
  constexpr uint16_t BMS_CELL_VOLTAGES_7  = 0x517;
  constexpr uint16_t BMS_CELL_VOLTAGES_8  = 0x518;
  constexpr uint16_t BMS_CELL_VOLTAGES_9  = 0x519;
  constexpr uint16_t BMS_CELL_VOLTAGES_10 = 0x51A;
  constexpr uint16_t BMS_CELL_VOLTAGES_11 = 0x51B;
  constexpr uint16_t BMS_CELL_VOLTAGES_12 = 0x51C;
  constexpr uint16_t BMS_CELL_VOLTAGES_13 = 0x51D;
  constexpr uint16_t BMS_CELL_VOLTAGES_14 = 0x51E;
  constexpr uint16_t BMS_CELL_VOLTAGES_15 = 0x51F;
  constexpr uint16_t BMS_CELL_VOLTAGES_16 = 0x520;
  constexpr uint16_t BMS_CELL_VOLTAGES_17 = 0x521;
  constexpr uint16_t BMS_CELL_VOLTAGES_18 = 0x522;

  constexpr uint16_t BMS_CELL_TEMPS_0  = 0x530;
  constexpr uint16_t BMS_CELL_TEMPS_1  = 0x531;
  constexpr uint16_t BMS_CELL_TEMPS_2  = 0x532;
  constexpr uint16_t BMS_CELL_TEMPS_3  = 0x533;
  constexpr uint16_t BMS_CELL_TEMPS_4  = 0x534;
  constexpr uint16_t BMS_CELL_TEMPS_5  = 0x535;
  constexpr uint16_t BMS_CELL_TEMPS_6  = 0x536;
  constexpr uint16_t BMS_CELL_TEMPS_7  = 0x537;
  constexpr uint16_t BMS_CELL_TEMPS_8  = 0x538;
  constexpr uint16_t BMS_CELL_TEMPS_9  = 0x539;
  constexpr uint16_t BMS_CELL_TEMPS_10 = 0x53A;
  constexpr uint16_t BMS_CELL_TEMPS_11 = 0x53B;
  constexpr uint16_t BMS_CELL_TEMPS_12 = 0x53C;
  constexpr uint16_t BMS_CELL_TEMPS_13 = 0x53D;
  constexpr uint16_t BMS_CELL_TEMPS_14 = 0x53E;
  constexpr uint16_t BMS_CELL_TEMPS_15 = 0x53F;
  constexpr uint16_t BMS_CELL_TEMPS_16 = 0x540;
  constexpr uint16_t BMS_CELL_TEMPS_17 = 0x541;
  constexpr uint16_t BMS_CELL_TEMPS_18 = 0x542;
}