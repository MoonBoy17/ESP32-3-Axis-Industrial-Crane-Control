#pragma once

// ==========================================
// CRANE PARAMETERS
// ==========================================

// Speed & Acceleration
// Steps per second
#define X_MAX_SPEED     1000.0
#define X_ACCEL         500.0
#define X_HOMING_SPEED  200.0

#define Y_MAX_SPEED     1000.0
#define Y_ACCEL         500.0
#define Y_HOMING_SPEED  200.0

#define Z_MAX_SPEED     800.0
#define Z_ACCEL         400.0
#define Z_HOMING_SPEED  150.0

// Travel Limits (Steps) -- Set after calibration/measurement
#define X_MAX_POS       10000 
#define Y_MAX_POS       10000
#define Z_MAX_POS       5000

// Homing
#define HOMING_BACKOFF  100 // Steps to back off after trigger

// WiFi
#define WIFI_SSID       "CRANE_NET"
#define WIFI_PASS       "12345678"
#define API_PORT        80
