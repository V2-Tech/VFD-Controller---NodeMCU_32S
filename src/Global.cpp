#include "Global.h"

Global GVL;
RemoteDebug Debug;
ModbusMaster node;
Button EncBtn(InEncoderBUT);

Global::Global()
{
    MotorParam motorParameter = {2850, 5, 400, 50};
    MenuEntitys.push_back({"RPM DES:", {4,0,0}}); //{Stringa, x-pos, y-pos, pag}
    MenuEntitys.push_back({"DIR DES:", {4,1,0}});
    MenuEntitys.push_back({"RPM:", {0,2,0}});
    MenuEntitys.push_back({"PWR: ", {9,2,0}});
    MenuEntitys.push_back({"STATUS:", {0,3,0}});
    MenuEntitys.push_back({"ERR:", {12,3,0}});
    byte SymbolRightArrow[] = {
        0x04,
        0x06,
        0x06,
        0x1F,
        0x1F,
        0x06,
        0x06,
        0x04
    };

    byte SymbolLeftArrow[] = {
        0x04,
        0x0C,
        0x1C,
        0x1F,
        0x1F,
        0x1C,
        0x0C,
        0x04
    };
}

void Global::begin()
{
    debugW("DEVICE CONNECTED");
    lcd.createChar(0, GVL.SymbolRightArrow);
    lcd.createChar(1, GVL.SymbolLeftArrow);
    menu.MenuValueUpdate(0, vfd.getActSetpoint(), MenuValueType::NUMBER);
    menu.MenuValueUpdate(1, 1, MenuValueType::SYMBOL);
}