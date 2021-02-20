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

ESP32Encoder encoder;
Menu menu;
WK600 vfd;
LiquidCrystal_I2C lcd(0x27,LCD_COL_NUM,LCD_ROW_NUM);