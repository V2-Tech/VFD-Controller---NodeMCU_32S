#include "Global.h"

Global GVL;
RemoteDebug Debug;
ModbusMaster node;
Button EncBtn(InEncoderBUT);

Global::Global()
{
    MotorParam motorParameter = {2850, 5, 400, 50};
    MenuEntitys.push_back({"RPM DES:", {6,0,0}}); //{Stringa, x-pos, y-pos, pag}
    MenuEntitys.push_back({"DIR DES:", {6,1,0}});
    MenuEntitys.push_back({"RPM:", {0,2,0}});
    MenuEntitys.push_back({"PWR: ", {9,2,0}});
    MenuEntitys.push_back({"STATUS:", {0,3,0}});
    MenuEntitys.push_back({"ERR:", {12,3,0}});
    
}

void Global::begin()
{
    debugW("DEVICE CONNECTED");
    
    lcd.home();
    EncoderValue = (int32_t)vfd.getActSetpoint();
    encoder.setCount(EncoderValue);
    //menu.DisplayedValueUpdate(0, EncoderValue, MenuValueType::NUMBER);
    //menu.DisplayedValueUpdate(1, 0, MenuValueType::SYMBOL);
}