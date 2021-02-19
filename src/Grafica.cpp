#include "Grafica.h"

Menu::Menu()
{

}

void Menu::begin(LiquidCrystal_I2C &lcd, String MenuArray[][4], size_t pags, size_t rows)
{
    for (size_t i = 0; i < pags; ++i)
    {
        Serial.printf("Pagina: %u\n",i);
        for (size_t j = 0; j < rows; ++j)
        {
            Serial.printf("Valore: %s\n",MenuArray[i][j]);
        }
    }
}

void Menu::update()
{

}