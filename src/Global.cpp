#include "Global.h"

Global GVL;
RemoteDebug Debug;
ModbusMaster node;
Button EncBtn(InEncoderBUT);
Button StartMotoreBtn(PulsStartMotore);
Button StopMotoreBtn(PulsStopMotore);
Button VDFErrorBtn(InVFDErr);

Global::Global()
{
    motorParameter = {2850, 5, 400, 50};
    MenuEntitys.push_back({"RPM DES:", {6,0,0}}); //{Stringa, x-pos, y-pos, pag}
    MenuEntitys.push_back({"DIR DES:", {6,1,0}});
    MenuEntitys.push_back({"RPM:", {0,2,0}});
    MenuEntitys.push_back({"PWR:", {11,2,0}});
    MenuEntitys.push_back({"STATUS:", {0,3,0}});
    MenuEntitys.push_back({"ERR:", {11,3,0}});
}

void Global::begin()
{
    debugW("DEVICE CONNECTED");
    lcd.home();
}