# ESP32 3-Axis Industrial Crane Control

A professional-grade, standalone firmware for controlling a 3-axis overhead crane using Stepper Motors and an ESP32. Features a mobile-friendly Web Interface, robust Safety Logic (E-Stop/Limits), and real-time WebSocket control.

## 🏗️ System Overview

- **Controller**: ESP32 Dev Module
- **Motors**: 3x Stepper Motors (Step/Dir Drivers)
- **Interface**: WiFi Web App (Hosted on ESP32)
- **Safety**: Dedicated Interrupt-driven Input Safety Check

## 🔌 Wiring Guide

**Note:** Logic Ground and Motor Driver Ground MUST be connected.

| Component | ESP32 Pin | Logic Level | Notes |
|-----------|-----------|-------------|-------|
| **X-Axis Step** | GPIO 26 | 3.3V | |
| **X-Axis Dir**  | GPIO 25 | 3.3V | |
| **X-Axis En**   | GPIO 13 | 3.3V | Active HIGH to Disable |
| **Y-Axis Step** | GPIO 27 | 3.3V | |
| **Y-Axis Dir**  | GPIO 14 | 3.3V | |
| **Y-Axis En**   | GPIO 12 | 3.3V | Active HIGH to Disable |
| **Z-Axis Step** | GPIO 33 | 3.3V | |
| **Z-Axis Dir**  | GPIO 32 | 3.3V | |
| **Z-Axis En**   | GPIO 15 | 3.3V | Active HIGH to Disable |
| **E-STOP**      | GPIO 23 | Active LOW | Connect NC Switch to GND. Pin has Internal Pullup. Break/Open = Trigger. Wait. Code assumes FALLING, please verify switch logic. Use NC switch to Ground. |
| **X-Limit MIN** | GPIO 34 | Active LOW | **Input Only. EXTERNAL PULLUP REQUIRED (10k to 3.3V)** |
| **X-Limit MAX** | GPIO 35 | Active LOW | **Input Only. EXTERNAL PULLUP REQUIRED** |
| **Y-Limit MIN** | GPIO 36 | Active LOW | **Input Only. EXTERNAL PULLUP REQUIRED** |
| **Y-Limit MAX** | GPIO 39 | Active LOW | **Input Only. EXTERNAL PULLUP REQUIRED** |
| **Z-Limit MIN** | GPIO 18 | Active LOW | Internal Pullup OK |
| **Z-Limit MAX** | GPIO 19 | Active LOW | Internal Pullup OK |

### ⚠️ IMPORTANT SAFETY NOTE
**GPIO 34, 35, 36, 39 are INPUT ONLY and DO NOT have internal pull-up resistors.** You MUST add an external 10k resistor from these pins to 3.3V, and connect your limit switches to Ground.

## 🚀 Installation & Build

1. **Install PlatformIO**: Ensure you have VSCode with PlatformIO extension.
2. **Build Filesystem**: Run `Build Filesystem Image` task to pack `data/` folder (HTML/CSS).
3. **Upload Filesystem**: Run `Upload Filesystem Image` to flash UI to SPIFFS.
4. **Upload Firmware**: Run `Upload` to flash the ESP32 code.

## 📱 Usage

1. **Power On**: Connect 12V/24V to Motor Drivers, USB/5V to ESP32.
2. **WiFi**: Connect to SSID: `CRANE_NET` / Password: `12345678` (Configurable in `CraneConfig.h`).
3. **Open Browser**: Navigate to `http://192.168.4.1` (or whatever IP is printed on Serial).
4. **Controls**:
    - **Jog**: Press and hold directional buttons to move. Release to stop.
    - **Home All**: Moves all axes to MIN limits to calibrate zero.
    - **E-Stop**: Immediately cuts power to motors (Software Stop). *Physical E-Stop cutting Main Power recommended for real Industry use.*

## 🛡️ Safety Functions

- **Emergency Stop**: Dedicated interrupt input. When triggered (Falling Edge), system enters `ESTOP` state. Requires Reset.
- **Limit Switches**: Interrupt-driven. Stops motion if triggered in direction of travel.
- **Connection Safety**: If WiFi connection is lost (WebSocket disconnect), motors stop automatically (implemented in loop cleanup).

## 🧩 Architecture

- `MovementController`: Coordinates high-level motion and state machine.
- `MotorAxis`: Wraps `AccelStepper` with safety limit checks.
- `Safety`: Handles ISRs for E-Stop and Limits.
- `WebInterface`: AsyncWebSocket handling for low-latency control.

---
**Disclaimer**: This firmware is for educational/prototyping purposes. Industrial lifting equipment requires redundant hardware safety interlocks, not just software control. Proceed at your own risk.
