#include "IMU.h"

IMU::IMU(Adafruit_MPU6050 mpu) : 
  _mpu(mpu), found(false),
  _gyroOffsetX(0), _gyroOffsetY(0), _gyroOffsetZ(0), 
  _accelOffsetX(0), _accelOffsetY(0), _accelOffsetZ(0) {} 


void IMU::init() {
  for (uint8_t i = 0; i < 3; i++) {
    found = _mpu.begin();
    if (found) {
      break;
    }
    delay(100);
  }
  if (!found) return;
  
  _mpu.setAccelerometerRange(MPU_ACCEL_RANGE);
  _mpu.setGyroRange(MPU_GYRO_RANGE);
  _mpu.setFilterBandwidth(MPU_FILTER_BW);
}

IMUReading IMU::read() {
  if (!_mpuFound) return;

  sensors_event_t a, g, t;
  _mpu.getEvent(&a, &g, &t);
  ax = a.acceleration.x - _accelOffsetX;
  ay = a.acceleration.y - _accelOffsetY;
  az = a.acceleration.z - _accelOffsetZ;
  gx = g.gyro.x - _gyroOffsetX;
  gy = g.gyro.y - _gyroOffsetY;
  gz = g.gyro.z - _gyroOffsetZ;
  t = t.temperature;

  return IMUReading{ax, ay, az, gx, gy, gz, temp;} 
}