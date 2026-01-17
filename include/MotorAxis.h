#pragma once
#include <AccelStepper.h>
#include "PinConfig.h"
#include "Safety.h"

class MotorAxis {
public:
    MotorAxis(uint8_t stepPin, uint8_t dirPin, uint8_t enPin, uint8_t minPin, uint8_t maxPin);
    
    void begin(float maxSpeed, float acceleration);
    void loop(); // Call in main loop
    
    // Motion Control
    void move(long relativeSteps);
    void moveTo(long absolutePos);
    void stop();
    void setSpeed(float speed); // For usage in runSpeed() if needed, primarily using run()
    
    // State
    bool isMoving();
    long currentPosition();
    bool isHomed() { return _homed; }

    // Homing (Blocking or Non-blocking state machine to be implemented in MotionController usually, 
    // but helper here)
    bool checkLimits();

    AccelStepper stepper;

private:
    uint8_t _enPin, _minPin, _maxPin;
    bool _homed = false;
};
