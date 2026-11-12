
#include <Arduino.h>
#include <algorithm>

#include "front_sensors.h"

// ---------------------------------
// APPS calibration
// ---------------------------------

constexpr int APPS1_REST = 2900;
constexpr int APPS1_FULL = 1800;

constexpr int APPS2_REST = 2900;
constexpr int APPS2_FULL = 1800;


// ---------------------------------
// Initialize front sensors
// ---------------------------------

void initFrontSensors() {

    pinMode(PIN_APPS1, INPUT);
    pinMode(PIN_APPS2, INPUT);

    pinMode(PIN_FRONT_BP, INPUT);

    pinMode(PIN_SUSP_FL, INPUT);
    pinMode(PIN_SUSP_FR, INPUT);

}


// ---------------------------------
// Read APPS
// ---------------------------------

float readAPPS() {

    int16_t apps1Raw = analogRead(PIN_APPS1);
    int16_t apps2Raw = analogRead(PIN_APPS2);

    float pct1 =
        (float)(APPS1_REST - apps1Raw) * 100.0f /
        (float)(APPS1_REST - APPS1_FULL);

    float pct2 =
        (float)(APPS2_REST - apps2Raw) * 100.0f /
        (float)(APPS2_REST - APPS2_FULL);

    float avg = (pct1 + pct2) * 0.5f;

    return std::clamp(avg, 0.0f, 100.0f);
}


// ---------------------------------
// Read brake pressure
// ---------------------------------

float readBPS() {

    int16_t bpsRaw = analogRead(PIN_FRONT_BP);

    // TODO: Calibrate against reference pressure gauge.
    // Convert ADC reading to bar.

    return 0.0f;
}


// ---------------------------------
// Read all front sensors
// ---------------------------------

FrontSensorsData readFrontSensors() {

    FrontSensorsData data;

    // Read APPS sensors
    int16_t apps1Raw = analogRead(PIN_APPS1);
    int16_t apps2Raw = analogRead(PIN_APPS2);

    float pct1 =
        (float)(APPS1_REST - apps1Raw) * 100.0f /
        (float)(APPS1_REST - APPS1_FULL);

    float pct2 =
        (float)(APPS2_REST - apps2Raw) * 100.0f /
        (float)(APPS2_REST - APPS2_FULL);

    // APPS plausibility check
    data.appsPlausible = abs(pct1 - pct2) <= 10.0f;

    // Calculate APPS percentage
    data.appsPct = std::clamp(
        (pct1 + pct2) * 0.5f,
        0.0f,
        100.0f
    );

    // Read brake pressure
    data.brakePressureBar = readBPS();

    // Read suspension sensors
    int16_t suspFLRaw = analogRead(PIN_SUSP_FL);
    int16_t suspFRRaw = analogRead(PIN_SUSP_FR);

    // TODO: Convert ADC values to mm using calibration.
    data.suspTravelFL_mm = 0.0f;
    data.suspTravelFR_mm = 0.0f;

    return data;
}
