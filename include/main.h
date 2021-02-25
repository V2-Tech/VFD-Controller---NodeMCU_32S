#include "Global.h"

void preTransmission()
{
  digitalWrite(Serial2DERE, 1);
}
void postTransmission()
{
  digitalWrite(Serial2DERE, 0);
}

const char* ssid     = "VFD-Manager-AP";
const char* password = "12345678";

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

ESP32Encoder encoder;
Menu menu;
WK600 vfd;
LiquidCrystal_I2C lcd(0x27,LCD_COL_NUM,LCD_ROW_NUM);