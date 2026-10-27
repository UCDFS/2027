#include "sensors.h"

float readAPPS() {
  int16_t apps1Raw = analogRead(APPS1_PIN);
  int16_t apps2Raw = analogRead(APPS2_PIN);

  float pct1 = (float)(APPS1_REST - apps1Raw) * 100.0f / (float)(APPS1_REST - APPS1_FULL);
  float pct2 = (float)(APPS2_REST - apps2Raw) * 100.0f / (float)(APPS2_REST - APPS2_FULL);

  float avg = pct1 + pct2 * 0.5f;
  return std::clamp(avg, 0.0f, 100.0f);
}

float readBPS() {
  int16_t bpsRaw = analogRead(BPS_PIN);
  int16_t delta = BPS_REST - bpsRaw;
  // TODO: calibrate bps against reference gauge and implement adcToBar() like in TEENSY repo

  // placeholder
  return 0.0f;
}




// readSuspendion(leftOrRightInput)

// readWheelSpeed(leftOrRightInput)
/*
  Wheel speed from an inductive proximity sensor (RS PRO 206-6160, PNP, NO)
  reading a steel trigger wheel. Works on Arduino Uno/Nano/Mega, Teensy, ESP32.

  WIRING
    Sensor brown (+V)   -> regulated 12 V   (CHECK colours on the datasheet connection diagram)
    Sensor blue  (0 V)  -> GND (shared with the microcontroller GND)
    Sensor black (out)  -> [10k] -+-[3.3k]- GND
                                  +-- SENSOR_PIN
    (the 3.3k also acts as the pull-down; use 10k/3.3k only for a stable 12 V supply)
    If you use an inverting optocoupler instead, change RISING to FALLING below.

  HOW IT WORKS
    Each tooth gives a rising edge. The interrupt records a timestamp.
    Speed is calculated from the time taken by the last WINDOW_TEETH teeth.
    WINDOW_TEETH = TEETH averages over a full revolution (cancels uneven teeth).
    A smaller window reacts faster but is noisier.
*/




#include <Arduino.h>   // defines millis, micros, pinMode, attachInterrupt, uint32_t etc.

// ---------------- Settings: change these for your car ----------------
const uint8_t  SENSOR_PIN     = 2;        // must be an interrupt-capable pin
const uint8_t  TEETH          = 12;       // teeth on your trigger wheel
const uint8_t  WINDOW_TEETH   = 12;       // teeth to average over (1..TEETH)
const float    WHEEL_CIRC_M   = 1.45f;    // rolling circumference of the loaded tyre, metres

const uint32_t MIN_PERIOD_US  = 400;      // ignore pulses closer than this (sensor max is 2 kHz = 500 us)
const uint32_t TIMEOUT_US     = 1000000;  // no tooth for 1 s -> car treated as stopped
const uint32_t STARTUP_MS     = 150;      // sensor output invalid for up to 100 ms after power-up
const uint32_t PRINT_EVERY_MS = 50;       // serial output rate (20 Hz)
// ----------------------------------------------------------------------

// ESP32 needs ISRs placed in IRAM; other boards ignore this
#if defined(ESP32)
  #define ISR_ATTR IRAM_ATTR
#else
  #define ISR_ATTR
#endif

// Shared with the interrupt: must be volatile
volatile uint32_t stamps[TEETH];     // ring buffer of recent tooth timestamps (us)
volatile uint8_t  head     = 0;      // next slot to write
volatile uint8_t  count    = 0;      // number of valid stamps stored
volatile uint32_t lastUs   = 0;      // time of the latest accepted tooth
volatile uint32_t windowUs = 0;      // time taken by the last WINDOW_TEETH teeth

void ISR_ATTR toothISR() {
  if (millis() < STARTUP_MS) return;             // ignore power-up stuff

  uint32_t now = micros();
  if ((now - lastUs) < MIN_PERIOD_US) return;    // reject noise / double triggers
  lastUs = now;

  if (count >= WINDOW_TEETH) {
    // stamp from WINDOW_TEETH pulses ago (oldest slot when window == TEETH)
    uint8_t idx = (head + TEETH - WINDOW_TEETH) % TEETH;
    windowUs = now - stamps[idx];                // unsigned maths survives micros() rollover
  }

  stamps[head] = now;
  head = (head + 1) % TEETH;
  if (count < TEETH) count++;
}

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);                    // external pull-down is on the circuit
  attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), toothISR, RISING);
}

void loop() {
  static uint32_t lastPrint = 0;
  if (millis() - lastPrint < PRINT_EVERY_MS) return;
  lastPrint = millis();

  // Take a consistent snapshot of the shared variables
  noInterrupts();
  uint32_t win  = windowUs;
  uint32_t last = lastUs;
  uint8_t  n    = count;
  interrupts();

  float wheelRpm = 0.0f;
  float speedKmh = 0.0f;

  bool stopped = (micros() - last) >= TIMEOUT_US;

  if (stopped) {
    // Reset so the next movement starts a fresh window
    noInterrupts();
    count = 0; head = 0; windowUs = 0;
    interrupts();
  } else if (n >= WINDOW_TEETH && win > 0) {
    // revolutions per second = (teeth counted / TEETH) / time in seconds
    float revPerSec = (1000000.0f * WINDOW_TEETH) / ((float)win * TEETH);
    wheelRpm = revPerSec * 60.0f;
    speedKmh = revPerSec * WHEEL_CIRC_M * 3.6f;
  }

  Serial.print(wheelRpm, 1);  Serial.print(" rpm, ");
  Serial.print(speedKmh, 1);  Serial.println(" km/h");
}

// readSteeringAngle()
