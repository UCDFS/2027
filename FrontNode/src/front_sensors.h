
#ifndef FRONT_SENSORS_H
#define FRONT_SENSORS_H

#include <Arduino.h>

// APPS pins
constexpr uint8_t PIN_APPS1 = A0;
constexpr uint8_t PIN_APPS2 = A1;

// Brake pressure sensor
constexpr uint8_t PIN_FRONT_BP = A2;

// Front suspension sensors
constexpr uint8_t PIN_SUSP_FL = A3;
constexpr uint8_t PIN_SUSP_FR = A4;


// Sensor readings
struct FrontSensorsData {

    float appsPct;
    bool appsPlausible;

    float brakePressureBar;

    float suspTravelFL_mm;
    float suspTravelFR_mm;
};


// Function declarations
void initFrontSensors();

FrontSensorsData readFrontSensors();

float readAPPS();

float readBPS();

#endif
