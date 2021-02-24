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
    byte SymbolRightArrow[] = {
        0x04,
        0x06,
        0x07,
        0x1F,
        0x1F,
        0x07,
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
    byte SymbolSelected[] = {
        0x00,
        0x0E,
        0x1F,
        0x1F,
        0x1F,
        0x1F,
        0x0E,
        0x00
    };
}

void Global::begin()
{
    debugW("DEVICE CONNECTED");
    lcd.createChar(0, GVL.SymbolRightArrow);
    lcd.createChar(1, GVL.SymbolLeftArrow);
    lcd.createChar(2, GVL.SymbolSelected);
    EncoderValue = (int32_t)vfd.getActSetpoint();
    encoder.setCount(EncoderValue);
    menu.MenuValueUpdate(0, EncoderValue, MenuValueType::NUMBER);
    menu.MenuValueUpdate(1, 0, MenuValueType::SYMBOL);
     
}