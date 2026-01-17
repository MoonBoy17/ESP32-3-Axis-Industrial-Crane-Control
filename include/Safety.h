#pragma once
#include <Arduino.h>
#include "PinConfig.h"

class Safety {
public:
    static volatile bool emergencyStopTriggered;
    static volatile bool limitSwitchTriggered;

    static void begin();
    static void checkInputs();
    static bool isSystemSafe();
    static void resetError();
    
private:
    static void IRAM_ATTR onEStop();
    static void IRAM_ATTR onLimitSwitch();
};
