#pragma once
#include "MotorAxis.h"
#include "CraneConfig.h"

enum class SystemState {
    IDLE,
    HOMING,
    RUNNING,
    ESTOP,
    ERROR
};

class MotionController {
public:
    MotionController();
    
    void begin();
    void loop();

    // Commands
    void homeAll();
    void jogX(int dir); // -1, 0, 1
    void jogY(int dir);
    void jogZ(int dir);
    void stopAll();
    
    SystemState getState() { return _state; }
    void resetState(); // Try to clear Estop/Error if safe

private:
    void handleHoming();
    
    MotorAxis xAxis;
    MotorAxis yAxis;
    MotorAxis zAxis;
    
    SystemState _state = SystemState::IDLE;
    int _homingStage = 0; // 0=Idle, 1=X, 2=Y, 3=Z, 4=Done
};
