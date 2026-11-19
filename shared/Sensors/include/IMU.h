#pragma once
#include <Adafruit_MPU6050.h>

typedef struct {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;
  float temp;
} IMUReading;

class IMU {
private:
  Adafruit_MPU6050 _mpu;
  float _gyroOffsetX;
  float _gyroOffsetY;
  float _gyroOffsetZ;
  float _accelOffsetX;
  float _accelOffsetY;
  float _accelOffsetZ;

public:
  IMU(Adafruit_MPU6050 mpu);
  bool found;
  void init();
  IMUReading read();
};