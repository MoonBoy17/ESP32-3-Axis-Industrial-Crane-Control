#include "MotorAxis.h"

MotorAxis::MotorAxis(uint8_t stepPin, uint8_t dirPin, uint8_t enPin, uint8_t minPin, uint8_t maxPin) 
    : stepper(AccelStepper::DRIVER, stepPin, dirPin), _enPin(enPin), _minPin(minPin), _maxPin(maxPin) 
{
}

void MotorAxis::begin(float maxSpeed, float acceleration) {
    pinMode(_enPin, OUTPUT);
    digitalWrite(_enPin, HIGH); // Disable initially (High = Disable usually for Pololu/TMC)

    stepper.setMaxSpeed(maxSpeed);
    stepper.setAcceleration(acceleration);
    stepper.setEnablePin(_enPin);
    stepper.setPinsInverted(false, false, true); // Dir: Norm, Step: Norm, Enable: Inverted (True means High=Disable? Wait.)
    // AccelStepper::setPinsInverted(bool directionInvert, bool stepInvert, bool enableInvert)
    // enableInvert=false (Default) -> High is Enable? 
    // Most drivers (A4988/TMC2209): LOW = ENABLE. HIGH = DISABLE.
    // So if enableInvert = true, then enableOutputs() writes HIGH? No.
    // AccelStepper: enableOutputs() calls digitalWrite(pin, _enableInverted ? LOW : HIGH).
    // If we want LOW to Enable, we want _enableInverted to be TRUE.
    
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs(); // This should write LOW to Enable.
}

void MotorAxis::loop() {
    stepper.run();
}

void MotorAxis::move(long relativeSteps) {
    if (!Safety::isSystemSafe()) return;
    stepper.move(relativeSteps);
}

void MotorAxis::moveTo(long absolutePos) {
    if (!Safety::isSystemSafe()) return;
    stepper.moveTo(absolutePos);
}

void MotorAxis::stop() {
    stepper.stop();
}

bool MotorAxis::isMoving() {
    return stepper.isRunning();
}

long MotorAxis::currentPosition() {
    return stepper.currentPosition();
}

bool MotorAxis::checkLimits() {
    // Read MIN limit
    // Note: INPUT_ONLY pins on X/Y (34-39) might need logic flip if hardware is diff.
    // Assumed Active LOW (Grounded = Triggered).
    if (digitalRead(_minPin) == LIMIT_SWITCH_ACTIVE_STATE) { 
        // Stop going negative?
        if (stepper.speed() < 0) {
            stepper.setSpeed(0);
            stepper.stop();
            return true;
        }
    }
    // Read MAX limit
    if (digitalRead(_maxPin) == LIMIT_SWITCH_ACTIVE_STATE) {
        if (stepper.speed() > 0) {
            stepper.setSpeed(0);
            stepper.stop();
            return true;
        }
    }
    return false;
}
