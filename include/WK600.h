#pragma once
#include <Arduino.h>
#include <HardwareSerial.h>
#include <ModbusMaster.h>

typedef enum
{

} VFDResult;

class WK600
{
    public:
        WK600();
        void begin(ModbusMaster &CommMaster);
        VFDResult run();
        VFDResult stop();
        VFDResult reset();
    private:
        ModbusMaster* _CommMaster;
};