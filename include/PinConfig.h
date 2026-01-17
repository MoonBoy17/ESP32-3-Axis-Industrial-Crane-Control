#pragma once
#include <Arduino.h>

// ==========================================
// PIN DEFINITIONS
// ==========================================

// X AXIS (Input Only Limit 34/35)
#define PIN_X_STEP      26
#define PIN_X_DIR       25
#define PIN_X_ENABLE    13
#define PIN_X_MIN       34  // Input Only
#define PIN_X_MAX       35  // Input Only

// Y AXIS (Input Only Limit 36/39)
#define PIN_Y_STEP      27
#define PIN_Y_DIR       14 
#define PIN_Y_ENABLE    12
#define PIN_Y_MIN       36  // Input Only, Sensor VP
#define PIN_Y_MAX       39  // Input Only, Sensor VN

// Z AXIS (Standard IO)
#define PIN_Z_STEP      33
#define PIN_Z_DIR       32
#define PIN_Z_ENABLE    15
#define PIN_Z_MIN       18
#define PIN_Z_MAX       19

// SAFETY
#define PIN_ESTOP       23  // Interrupt capable

// Limit Switch Configuration
#define LIMIT_SWITCH_ACTIVE_STATE LOW  // Pull-up, active low
