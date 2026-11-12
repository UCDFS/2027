
#include <Arduino.h>

#include "front_sensors.h"
#include "wheel_speed.h"

// Wheel speed sensor pins
constexpr uint8_t PIN_WHEEL_LEFT  = 2;
constexpr uint8_t PIN_WHEEL_RIGHT = 3;

// Create one WheelSpeed object for each front wheel
WheelSpeed wheelLeft(PIN_WHEEL_LEFT);
WheelSpeed wheelRight(PIN_WHEEL_RIGHT);

void setup() {
    Serial.begin(115200);

    // Configure analog inputs
    analogReadResolution(12);

    // Initialize front sensors
    initFrontSensors();

    // Initialize wheel speed sensors
    wheelLeft.begin();
    wheelRight.begin();

    Serial.println("Front Node Initialized");
}

void loop() {

    static uint32_t lastSendMs = 0;

    // Update sensor readings every 20 ms
    if (millis() - lastSendMs >= 20) {
        lastSendMs = millis();

        // Read front sensors
        FrontSensorsData front = readFrontSensors();

        // Read wheel speeds
        float leftKmh  = wheelLeft.kmh();
        float rightKmh = wheelRight.kmh();

        // Temporary serial debugging
        Serial.print("Left Wheel: ");
        Serial.print(leftKmh);

        Serial.print(" km/h | Right Wheel: ");
        Serial.print(rightKmh);

        Serial.print(" km/h | APPS: ");
        Serial.print(front.appsPct);

        Serial.println("%");
    }
}
