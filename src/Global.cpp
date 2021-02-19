#include "Global.h"

Global GVL;
RemoteDebug Debug;
ModbusMaster node;
Button EncBtn(InEncoderBUT);

Global::Global()
{
    xFirstCycle = false;
    MenuPages[0][0] = "Valore 1: ";
    MenuPages[0][1] = "Valore 2: ";
    MenuPages[0][2] = "Valore 3: ";
    MenuPages[0][3] = "Valore 4: ";
    MenuPages[1][0] = "Valore 5: ";
    MenuPages[1][1] = "Valore 6: ";
    MenuPages[1][2] = "Valore 7: ";
    MenuPages[1][3] = "Valore 8: ";
}

void Global::init()
{
    if (!Global::xFirstCycle)
    {
        debugW("DEVICE CONNECTED");
        Global::xFirstCycle = true;
    }
}