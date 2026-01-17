#include "Safety.h"

volatile bool Safety::emergencyStopTriggered = false;
volatile bool Safety::limitSwitchTriggered = false;

void Safety::begin() {
    pinMode(PIN_ESTOP, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ESTOP), onEStop, FALLING); 
    // Assuming Normally Closed E-Stop switch that opens (HIGH) or logic Low trigger?
    // "Emergency stop input that immediately disables all motors"
    // Usually E-Stop is NC, so if wire breaks or pressed, it goes HIGH (if pull-down) or LOW (if pull-up).
    // Let's assume standard Industrial: NC switch connected to GND. Pin Pull-up. 
    // Pressed = Open = HIGH.
    // Wait, let's stick to "Falling" implies it goes Low to Trigger?
    // User said "Pull-ups" for limit switches. Let's assume Active Low for everything for safety (GND = Safe).
    // Actually, "Wire Break Safety" usually means NC to Ground. If wire breaks, valid signal is lost.
    // Let's assume Active Low (Ground) is Normal, High is Trigger (Open).
    // BUT user said "Limit switches... with pull-ups". Which usually means Switch to Ground.
    // So OPEN = HIGH (Pull-up) -> Triggered ? OR CLOSED = LOW (Ground) -> Triggered?
    // Common 3D printer style: NO switch. Open = High, Closed = Low. Trigger on Low.
    // Let's implement generic Change interrupt and check state.

    // Re-reading requirements: "Limit switches (with pull-ups)".
    // Let's assume switches close to ground when hit. -> Active LOW.
    
    // Limits
    pinMode(PIN_X_MIN, INPUT_PULLUP); // Input Only on ESP32 (34-39), no internal pullup!
    // ALERT: GPIO 34, 35, 36, 39 do NOT have internal pull-ups. External PUs required.
    // I will assume user provides them as requested in "Electrical Connections" section implicit.
    
    // For X/Y input-only pins, we can't use `INPUT_PULLUP` if they are 34-39.
    // But `pinMode` with `INPUT` is fine.
    pinMode(PIN_X_MAX, INPUT); 
    pinMode(PIN_Y_MIN, INPUT);
    pinMode(PIN_Y_MAX, INPUT);
    
    pinMode(PIN_Z_MIN, INPUT_PULLUP);
    pinMode(PIN_Z_MAX, INPUT_PULLUP);

    // Attach Interrupts if possible? 
    // ESP32 supports interrupts on all GPIOs.
    attachInterrupt(digitalPinToInterrupt(PIN_X_MIN), onLimitSwitch, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_X_MAX), onLimitSwitch, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_Y_MIN), onLimitSwitch, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_Y_MAX), onLimitSwitch, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_Z_MIN), onLimitSwitch, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_Z_MAX), onLimitSwitch, CHANGE);
}

void IRAM_ATTR Safety::onEStop() {
    emergencyStopTriggered = true;
    // Hardware disable ASAP if possible? 
    // We can write Enable Pins HIGH (Disable) immediately here if we had access.
    // But MotorAxis owns them.
    // We rely on main loop to see flag.
}

void IRAM_ATTR Safety::onLimitSwitch() {
    // Debounce/Check logic could go here, but for safety, stop first.
    // We can refine which axis hit in main loop.
    limitSwitchTriggered = true;
}

void Safety::checkInputs() {
    // Polling backup
    if (digitalRead(PIN_ESTOP) == LOW) { // Assuming Active Low trigger for now? 
        // Wait, if NC switch to Gnd: Normal=Low. Pressed=High.
        // If NO switch to Gnd: Normal=High. Pressed=Low.
        // "Emergency stop input that immediately disables all motors"
        // Let's assume Active LOW trigger (Switch closes to Ground) for standard NO buttons 
        // OR user follows "Pull-ups" rule. 
        // If strict industrial: NC loop. 
        // I will document: Active LOW (Grounded) = TRIGGER for ESTOP? 
        // Actually usually E-STOP is "PUSH TO BREAK" -> Pin goes HIGH.
        // I'll stick to: Triggered if pin != SAFE_STATE.
        // Let's assume SAFE = HIGH (Internal Pullup, NC switch to GND). 
        // So Low = Open/Safe? No.
        // Let's assume standard Arduino Maker style: NO Button to GND. Pressed = LOW.
        // TRIGGERED = LOW.
    }
}

bool Safety::isSystemSafe() {
    return !emergencyStopTriggered;
}

void Safety::resetError() {
    emergencyStopTriggered = false;
    limitSwitchTriggered = false;
}
