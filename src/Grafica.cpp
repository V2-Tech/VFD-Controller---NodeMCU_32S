#include "Grafica.h"

Menu::Menu()
{

}

void Menu::begin(LiquidCrystal_I2C &lcd, String MenuArray[][4], size_t pags, size_t rows, ESP32Encoder &enc, Button &encBtn)
{
    _lcd = &lcd;
    _enc = &enc;
    _encBtn = &encBtn;
    /*
    for (size_t i = 0; i < pags; ++i)
    {
        Serial.printf("Pagina: %u\n",i);
        for (size_t j = 0; j < rows; ++j)
        {
            Serial.printf("Valore: %s\n",MenuArray[i][j]);
        }
    }
    */
}

void Menu::update()
{
    EncoderUpdate();
}

void Menu::EncoderUpdate()
{
    if(EncoderValue != _enc->getCount())
    {
        EncoderValue = _enc->getCount();
        //Serial.println("Encoder count = "+String((int32_t)_enc->getCount()));
    }
    if(_encBtn->wasPressed())
    {

    }
}