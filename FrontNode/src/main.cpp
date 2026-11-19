#include <Arduino.h>
#include <FlexCAN_T4.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include "sensors.h"
#include "CanProtocol.h"


FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
Adafruit_MPU6050 mpu;
IMU imu(mpu);
IMUReading imuReading;
uint8_t appsPct = 0;

void setup() {
  // Initialize CAN bus
  Can1.begin();
  Can1.setBaudRate(CAN_BAUD_RATE);

  // Initialize sensors
  pinMode(FL_WHEEL_SPEED_PIN, INPUT);
  pinMode(FR_WHEEL_SPEED_PIN, INPUT);
  analogReadResolution(12);

  Wire.setSDA(FRONT_IMU_SDA_PIN);
  Wire.setSCL(FRONT_IMU_SCL_PIN);
  Wire.begin();
  imu.init();
}

void loop() {
  // Read and send APPS
  if (readApps(&appsPct)) {
    Can1.write(CAN::encodeAPPSData({appsPct}));
  } else {
    Can1.write(CAN::encodeAPPSData({APPS_ERROR}));
  }

  // Read IMU
  if (imu.found) {
    imuReading = imu.read();
    IMUAccelData accel = CAN::imuReadingToAccelData(imuReading);
    Can1.write(CAN::encodeIMUAccelData(accel));
    IMUGyroTempData gyro = CAN::imuReadingToGyroData(imuReading);
    Can1.write(CAN::encodeIMUGyroTempData(gyro));
  }

  // Read BPS
  // Read wheel speeds
  // Read suspensions
  // Send CAN frame
}