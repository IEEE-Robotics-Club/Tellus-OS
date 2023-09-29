#include <Arduino.h>

struct BlinkyStruct{
    byte blinkTimeHB;
    byte blinkTimeLB;
    byte lightState;
    byte lastToggleHB;
    byte lastToggleLB;
};

