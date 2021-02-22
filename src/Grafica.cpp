#include "Grafica.h"

Menu::Menu()
{
    EncoderValue = 0;
    _ActPage = 1;
    _ActRow = 1;
    _PageQuantity = 1;
}

void Menu::begin(LiquidCrystal_I2C &lcd, std::vector<MenuEntityList> menuEntityList, ESP32Encoder &enc, Button &encBtn, WK600 &vfd)
{
    //Copio i riferimenti nelle variabili interne alla classe
    _lcd = &lcd;
    _enc = &enc;
    _encBtn = &encBtn;
    _vfd = &vfd;
    _menuArray = menuEntityList;

    //Inizializzo LCD ed ENCODER
    _encBtn->begin();
    _lcd->init();
    _lcd->backlight();

    //Ricavo il numero di elementi totale del menu
    _MenuEntityQuantity = _menuArray.size();
    #ifdef LCD_DEBUG
        Serial.println(_MenuEntityQuantity);
    #endif

    //Stampo l'intestazione della prima pagina
    for (int8_t k = 0; k < _MenuEntityQuantity; ++k)
    {
        
        _lcd->setCursor(_menuArray[k].position.x ,_menuArray[k].position.y);
        _lcd->printstr(_menuArray[k].name.c_str());
        #ifdef LCD_DEBUG
            Serial.printf("Menu entity number %u\n",k);
            Serial.printf("Value: %s\n",_menuArray[k].name.c_str());
            Serial.printf("Coord X: %u\n",_menuArray[k].position.x);
            Serial.printf("Coord Y: %u\n",_menuArray[k].position.y);
            Serial.printf("Assigned page: %u\n",_menuArray[k].position.page);  
        #endif

        //Ricavo la quantita totale di pagine
        if (_menuArray[k].position.page > _PageQuantity-1)
        {
            _PageQuantity = _menuArray[k].position.page + 1;
        }
    }
    _lcd->setCursor(0,0);

    //Ricavo i valori attuali dell inverter
    
}

void Menu::update()
{
    EncoderUpdate();
}

void Menu::EncoderUpdate()
{
    _encBtn->read();
    if(_tempEncoderCount != _enc->getCount())
    {
        _tempEncoderCount = _enc->getCount();
        EncoderValue = (int32_t)_enc->getCount();
        #ifdef ENC_DEBUG
            Serial.println("Encoder count = "+String((int32_t)_enc->getCount()));
        #endif
    }
    if(_encBtn->wasPressed())
    {
        #ifdef ENC_DEBUG
            Serial.println("Encoder button pressed");
        #endif
        menuMode = MENU_EDIT;
    }
}