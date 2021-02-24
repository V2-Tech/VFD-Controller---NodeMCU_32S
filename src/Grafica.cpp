#include "Grafica.h"

Menu::Menu()
{
    EncoderCounts = 0;
    _ActPage = 1;
    _ActRow = 1;
    _PageQuantity = 1;
}

void Menu::begin(LiquidCrystal_I2C &lcd, uint8_t lcd_cols,uint8_t lcd_rows, std::vector<MenuEntityList> menuEntityList, ESP32Encoder &enc, Button &encBtn, WK600 &vfd)
{
    //Copio i riferimenti nelle variabili interne alla classe
    _lcd = &lcd;
    _enc = &enc;
    _encBtn = &encBtn;
    _vfd = &vfd;
    _menuArray = menuEntityList;
    _lcdRowNum = lcd_rows;
    _lcdColNum = lcd_cols;

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
    _lcd->cursor_off();

    //Carico valore iniziale dell encoder

}

void Menu::EncoderUpdate(uint16_t lowerLimit, uint16_t upperLimit)
{
    _encBtn->read();
    if(_tempEncoderCount != _enc->getCount())
    {
        if (_enc->getCount()<=upperLimit)
        {
            _tempEncoderCount = _enc->getCount();
            EncoderCounts = (int32_t)_enc->getCount();
        }
        else
        {
            _tempEncoderCount = upperLimit;
            EncoderCounts = (int32_t)upperLimit;
            _enc->setCount(upperLimit);
        }
        if (_enc->getCount()>=lowerLimit)
        {
            _tempEncoderCount = _enc->getCount();
            EncoderCounts = (int32_t)_enc->getCount();
        }
        else
        {
            _tempEncoderCount = lowerLimit;
            EncoderCounts = (int32_t)lowerLimit;
            _enc->setCount(lowerLimit);
        }
        #ifdef ENC_DEBUG
            Serial.println("\nEncoder count = "+String((int32_t)_enc->getCount()));
        #endif
        Menu::MenuValueUpdate(0, EncoderCounts, MenuValueType::NUMBER);
    }
    if(_encBtn->wasPressed())
    {
        #ifdef ENC_DEBUG
            Serial.println("\nEncoder button pressed");
        #endif
        menuMode = MENU_EDIT;
    }    
}

void Menu::MenuValueUpdate(uint8_t MenuEntityNum, uint32_t value, MenuValueType ValueType)
{
    if (ValueType==MenuValueType::NUMBER)
    {
        std::ostringstream ostr;
        ostr << _menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() +  1;
        std::string tempStr = ostr.str();
        int16_t tempStrLength = tempStr.length();
        ostr << value;
        tempStr = ostr.str();
        tempStrLength = tempStr.length();
        #ifdef LCD_DEBUG
            Serial.printf("\nMenu value update leght check result: %u",tempStrLength);
        #endif
        if (tempStrLength<=_lcdColNum)
        {
            //Resetto l eventuale campo inutilizzato
            int16_t tempVal = _lcdColNum - _menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length();
            for(int8_t k = 0; k < tempVal; ++k)
            {
                _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + k, _menuArray[MenuEntityNum].position.y);
                _lcd->printstr(" ");
            }
            //Scrivo il campo
            _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1, _menuArray[MenuEntityNum].position.y);
            _lcd->print(value);
            #ifdef LCD_DEBUG
                Serial.printf("\nCoord X: %u",_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1);
                Serial.printf("\nCoord Y: %u",_menuArray[MenuEntityNum].position.y);
                Serial.printf("\nValue: %u",value);  
            #endif
        }
        else
        {
            #ifdef LCD_DEBUG
                Serial.printf("\nMenu value update leght check failed. Total length is: %u",tempStrLength);
            #endif
        }
    }
    else
    {
        std::ostringstream ostr;
        ostr << _menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 2;
        std::string tempStr = ostr.str();
        int16_t tempStrLength = tempStr.length();
        #ifdef LCD_DEBUG
            Serial.printf("\nMenu value update leght check result: %u",tempStrLength);
        #endif
        if (tempStrLength<=_lcdColNum)
        {
            _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 2, _menuArray[MenuEntityNum].position.y);
            _lcd->write(value);
            #ifdef LCD_DEBUG
                Serial.printf("\nCoord X: %u",_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 2);
                Serial.printf("\nCoord Y: %u",_menuArray[MenuEntityNum].position.y);
                Serial.printf("\nSymbol number: %u",value);  
            #endif
        }
        else
        {
            #ifdef LCD_DEBUG
                Serial.printf("\nMenu value update leght check failed. Total length is: %u",tempStrLength);
            #endif
        }     
    }
}