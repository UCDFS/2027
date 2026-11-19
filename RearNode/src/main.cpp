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

void setup() {
  // Initialize CAN bus
  Can1.begin();
  Can1.setBaudRate(CAN_BAUD_RATE);

  // Initialize sensors
  pinMode(RL_WHEEL_SPEED_PIN, INPUT);
  pinMode(RR_WHEEL_SPEED_PIN, INPUT);
  pinMode(BRAKE_LIGHT_PIN, OUTPUT);
  analogReadResolution(12);

  Wire.setSDA(REAR_IMU_SDA_PIN);
  Wire.setSCL(REAR_IMU_SCL_PIN);
  Wire.begin();
  imu.init();
}

void loop() {
  // Update brake light
  
  // Read sensors

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