#pragma once

#include <cstdint>

bool readApps(float* appsPct);

// APPS
#define APPS1_PIN      A0   // ADC pin for APPS1 sensor
#define APPS2_PIN      A1   // ADC pin for APPS2 sensor
#define APPS1_REST     2900 // Rest position for APPS1 sensor
#define APPS1_FULL     1800 // Fully pressed position for APPS1 sensor
#define APPS2_REST     2900 // Rest position for APPS2 sensor
#define APPS2_FULL     1800 // Fully pressed position for APPS2 sensor
#define APPS_ERROR     255  // Error value for APPS sensor

// BPS
#define FRONT_BPS_PIN  A2   // ADC pin for front BPS sensor
#define REAR_BPS_PIN   A1   // ADC pin for rear BPS sensor

// Suspension
#define FR_SUS_PIN     A4   // ADC pin for front right suspension sensor
#define FL_SUS_PIN     A3   // ADC pin for front left suspension sensor
#define RR_SUS_PIN     A7   // ADC pin for rear right suspension sensor
#define RL_SUS_PIN     A6   // ADC pin for rear left suspension sensor

// Wheel Speed
#define FR_WHEEL_SPEED_PIN 20    // Digital pin for front right wheel speed sensor
#define FL_WHEEL_SPEED_PIN 19    // Digital pin for front left wheel speed sensor
#define RR_WHEEL_SPEED_PIN 17    // Digital pin for rear right wheel speed sensor
#define RL_WHEEL_SPEED_PIN 16    // Digital pin for rear left wheel speed sensor

// Steering Angle
#define STEER_ANGLE_PIN    A0   // ADC pin for steering angle sensor

// Coolant Temperature
#define COOLANT_TEMP1_PIN  A10   // ADC pin for coolant temperature sensor
#define COOLANT_TEMP2_PIN  A11   // ADC pin for coolant temperature sensor
#define COOLANT_TEMP3_PIN  A12   // ADC pin for coolant temperature sensor

// IMUs
#define REAR_IMU_SDA_PIN   18   // I2C SDA pin for rear IMU
#define REAR_IMU_SCL_PIN   19   // I2C SCL pin for rear IMU
#define FRONT_IMU_SDA_PIN  25   // I2C SDA pin for front IMU
#define FRONT_IMU_SCL_PIN  24   // I2C SCL pin for front IMU

// GPS
#define GPS_SERIAL Serial2

// Non-sensor pins
#define BRAKE_LIGHT_PIN    14  // Digital pin for brake light control
#define RTD_SPEAKER_EN_PIN 9   // Digital pin for RTD speaker enable
#define PRECHARGE_EN_PIN   6   // Digital pin for precharge enable
#define AUX1_PIN           3   // Digital pin for AUX1 button
#define RTD_BUTTON_PIN     2   // Digital pin for RTD button


