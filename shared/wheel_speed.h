
#ifndef WHEEL_SPEED_H
#define WHEEL_SPEED_H

#include <Arduino.h>

class WheelSpeed {

public:

    // Specify sensor pin
    WheelSpeed(uint8_t pin);

    // Initialize sensor and interrupt
    void begin();

    // Return speed
    float kmh();
    float rpm();

private:

    uint8_t _pin;
    uint8_t _slot;

    // Sensor settings
    static constexpr uint8_t TEETH = 12;
    static constexpr uint8_t WINDOW_TEETH = 12;

    static constexpr float WHEEL_CIRC_M = 1.45f;

    static constexpr uint32_t MIN_PERIOD_US = 400;
    static constexpr uint32_t TIMEOUT_US = 1000000;
    static constexpr uint32_t STARTUP_MS = 150;

    // Interrupt timing variables
    volatile uint32_t _stamps[TEETH];
    volatile uint8_t _head;
    volatile uint8_t _count;
    volatile uint32_t _lastUs;
    volatile uint32_t _windowUs;

    // Two independent wheel instances
    static WheelSpeed* _left;
    static WheelSpeed* _right;

    // Interrupt handlers
    static void ISR_Left();
    static void ISR_Right();

    // Internal calculations
    void handlePulse();
    float revPerSecond();

};

#endif
