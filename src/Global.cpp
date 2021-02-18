#include "Global.h"

Global GVL;
RemoteDebug Debug;
ModbusMaster node;

Global::Global()
{
    xFirstCycle = false;
}

void Global::init()
{
    if (!Global::xFirstCycle)
    {
        debugW("DEVICE CONNECTED");
    }
}