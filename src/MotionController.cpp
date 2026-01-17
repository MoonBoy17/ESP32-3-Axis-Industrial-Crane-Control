#include "MotionController.h"

MotionController::MotionController() 
    : xAxis(PIN_X_STEP, PIN_X_DIR, PIN_X_ENABLE, PIN_X_MIN, PIN_X_MAX),
      yAxis(PIN_Y_STEP, PIN_Y_DIR, PIN_Y_ENABLE, PIN_Y_MIN, PIN_Y_MAX),
      zAxis(PIN_Z_STEP, PIN_Z_DIR, PIN_Z_ENABLE, PIN_Z_MIN, PIN_Z_MAX)
{
}

void MotionController::begin() {
    xAxis.begin(X_MAX_SPEED, X_ACCEL);
    yAxis.begin(Y_MAX_SPEED, Y_ACCEL);
    zAxis.begin(Z_MAX_SPEED, Z_ACCEL);
    
    Safety::begin();
}

void MotionController::loop() {
    // 1. Safety Check
    if (Safety::emergencyStopTriggered) {
        _state = SystemState::ESTOP;
        stopAll();
    }
    
    // 2. Axis Loops
    xAxis.loop();
    yAxis.loop();
    zAxis.loop();
    
    // 3. State Logic
    switch (_state) {
        case SystemState::HOMING:
            handleHoming();
            break;
            
        case SystemState::RUNNING:
            // Check soft limits or other logic
            break;
            
        case SystemState::ESTOP:
            // Remain stopped until reset
            break;
            
        default:
            break;
    }
}

void MotionController::handleHoming() {
    // Simple state machine for homing sequence
    // 0: Start -> Home Z (Safety first, move up?) Usually Home Z Up.
    // Let's assume Z Min is UP (Top of crane).
    // ... Implementation of one axis at a time ...
    
    // Placeholder for simplified simultaneous homing or sequential
    // Real implementation requires non-blocking logic:
    // if (!zAxis.isHomed()) { zAxis.updateHoming(); return; }
}

void MotionController::jogX(int dir) {
    if (_state == SystemState::ESTOP) return;
    if (dir == 0) {
        xAxis.stop();
    } else {
        // Move indefinitely? AccelStepper doesn't support "Run Forever" easily without big numbers.
        xAxis.move(dir * 1000000); 
    }
}

void MotionController::jogY(int dir) {
    if (_state == SystemState::ESTOP) return;
    if (dir == 0) yAxis.stop();
    else yAxis.move(dir * 1000000);
}

void MotionController::jogZ(int dir) {
    if (_state == SystemState::ESTOP) return;
    if (dir == 0) zAxis.stop();
    else zAxis.move(dir * 1000000);
}

void MotionController::stopAll() {
    xAxis.stop();
    yAxis.stop();
    zAxis.stop();
}

void MotionController::resetState() {
    Safety::resetError();
    _state = SystemState::IDLE;
}
