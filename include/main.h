#include "Global.h"

void preTransmission()
{
        digitalWrite(Serial2DERE, 1);
}
void postTransmission()
{
        digitalWrite(Serial2DERE, 0);
}

const char *ssid = "VFD-Manager-AP";
const char *password = "12345678";

byte SymbolRightArrow[] = {
    B00000,
    B00100,
    B00110,
    B11111,
    B11111,
    B00110,
    B00100,
    B00000
};
byte SymbolLeftArrow[] = {
    B00000,
    B00100,
    B01100,
    B11111,
    B11111,
    B01100,
    B00100,
    B00000
};
byte SymbolSelected[] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000
};

void ValueUpdate()
{
        menu.GraphicValueUpdate();
}

ESP32Encoder encoder;
Menu menu;
WK600 vfd;
LiquidCrystal_I2C lcd(0x27, LCD_COL_NUM, LCD_ROW_NUM);
Ticker timerReadVdf(ValueUpdate, 1000, 0,MILLIS);

void BtnUpdate()
{
        StartMotoreBtn.read();
        StopMotoreBtn.read();
        VDFErrorBtn.read();

        if (StartMotoreBtn.wasPressed())
        {
                vfd.setSpeed(menu.MenuValues[0]);
                Serial.printf("\nSetted speed percentas: %u",menu.MenuValues[0]);
                uint32_t tempDir = menu.MenuValues[1];
                if (tempDir==0)
                {
                        vfd.run_forw();
                        Serial.print("\nPuls start pressed. Send RUN forward command");
                }
                else
                {
                        vfd.run_rev();
                        Serial.print("\nPuls start pressed. Send RUN reverse command");
                }
        }
        if (StopMotoreBtn.wasPressed())
        {
                vfd.stop();
                Serial.print("\nPuls stop pressed. Send STOP command");
        }
        if (StopMotoreBtn.pressedFor(PULS_HOLD_TIME))
        {
                vfd.reset();
                Serial.print("\nPuls stop holded. Send RESET command");
        }
        if (VDFErrorBtn.wasPressed())
        {
                uint32_t tempVal = vfd.getFaultCode();
                menu.DisplayedValueUpdate(5, tempVal, MenuValueType::NUMBER);
                Serial.printf("\nPuls vdf err pressed. Read ERROR CODE: %u",tempVal);
        }
        if (VDFErrorBtn.wasReleased())
        {
                menu.DisplayedValueUpdate(5, 0, MenuValueType::NUMBER);
                Serial.print("\nPuls vdf err released. Reset ERROR CODE");
        }
}