#include "wheel_speed.h"

// Static pointers to the two wheel objects
WheelSpeed* WheelSpeed::_left = nullptr;
WheelSpeed* WheelSpeed::_right = nullptr;

// Constructor
WheelSpeed::WheelSpeed(uint8_t pin)
    : _pin(pin),
      _head(0),
      _count(0),
      _lastUs(0),
      _windowUs(0),
      _slot(255)
{
    for (uint8_t i = 0; i < TEETH; i++) {
        _stamps[i] = 0;
    }

    // Assign this object to a free interrupt slot
    if (_left == nullptr) {
        _left = this;
        _slot = 0;
    }
    else if (_right == nullptr) {
        _right = this;
        _slot = 1;
    }
}

// Initialize sensor
void WheelSpeed::begin() {

    pinMode(_pin, INPUT);

    if (_slot == 0) {
        attachInterrupt(
            digitalPinToInterrupt(_pin),
            ISR_Left,
            RISING
        );
    }
    else if (_slot == 1) {
        attachInterrupt(
            digitalPinToInterrupt(_pin),
            ISR_Right,
            RISING
        );
    }
}

// Left interrupt
void WheelSpeed::ISR_Left() {

    if (_left != nullptr) {
        _left->handlePulse();
    }
}

// Right interrupt
void WheelSpeed::ISR_Right() {

    if (_right != nullptr) {
        _right->handlePulse();
    }
}

// Process each incoming tooth pulse
void WheelSpeed::handlePulse() {

    if (millis() < STARTUP_MS) return;

    uint32_t now = micros();

    // Reject noise / double triggers
    if ((now - _lastUs) < MIN_PERIOD_US) return;

    _lastUs = now;

    // Calculate time for the last full window
    if (_count >= WINDOW_TEETH) {

        uint8_t idx =
            (_head + TEETH - WINDOW_TEETH) % TEETH;

        _windowUs = now - _stamps[idx];
    }

    // Store timestamp
    _stamps[_head] = now;

    _head = (_head + 1) % TEETH;

    if (_count < TEETH) {
        _count++;
    }
}

// Calculate revolutions per second
float WheelSpeed::revPerSecond() {

    uint32_t window;
    uint32_t last;
    uint8_t count;

    // Safely copy interrupt variables
    noInterrupts();

    window = _windowUs;
    last = _lastUs;
    count = _count;

    interrupts();

    // No pulses recently = stopped
    if ((micros() - last) >= TIMEOUT_US) {

        noInterrupts();

        _count = 0;
        _head = 0;
        _windowUs = 0;

        interrupts();

        return 0.0f;
    }

    // Not enough pulses yet
    if (count < WINDOW_TEETH || window == 0) {
        return 0.0f;
    }

    return
        (1000000.0f * WINDOW_TEETH) /
        ((float)window * TEETH);
}

// Return RPM
float WheelSpeed::rpm() {

    return revPerSecond() * 60.0f;
}

// Return km/h
float WheelSpeed::kmh() {

    return revPerSecond() * WHEEL_CIRC_M * 3.6f;
}
