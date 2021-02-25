#include "Grafica.h"

Menu::Menu()
{
    EncoderCounts[MENU_EDIT] = 0;
    EncoderCounts[MENU_SCROL] = 0;
    _ActPage = 0;
    _ActRow = 0;
    _PageQuantity = 1;
    menuMode = MENU_EDIT;
}

void Menu::begin(LiquidCrystal_I2C &lcd, uint8_t lcd_cols,uint8_t lcd_rows, std::vector<MenuEntityList> menuEntityList, ESP32Encoder &enc, 
        uint32_t enc_lower_limit, uint32_t enc_upper_limit, Button &encBtn, WK600 &vfd)
{
    //Copio i riferimenti nelle variabili interne alla classe
    _lcd = &lcd;
    _enc = &enc;
    _encBtn = &encBtn;
    _vfd = &vfd;
    _menuArray = menuEntityList;
    _lcdRowNum = lcd_rows;
    _lcdColNum = lcd_cols;
    _enc_lower = enc_lower_limit;
    _enc_upper = enc_upper_limit;

    //Inizializzo LCD ed ENCODER
    _encBtn->begin();
    _lcd->begin(lcd_cols, lcd_rows);
    _lcd->init();
    _lcd->backlight();

    //Ricavo il numero di elementi totale del menu
    _MenuEntityQuantity = _menuArray.size();
    #ifdef LCD_DEBUG
        Serial.printf("Number of total menu entity: %u\n",_MenuEntityQuantity);
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
    Menu::SelectionUpdate(0);
    _lcd->setCursor(0,0);
    _lcd->cursor_off();

    //Genero il vettore dei valori associati alle voci MENU SCROL modificabili tramite rotazione encoder
    _menuValues.push_back((int32_t)_vfd->getActSetpoint());
    _menuValues.push_back(0);

    Menu::DisplayedValueUpdate(0, _menuValues[0], MenuValueType::NUMBER);
    Menu::DisplayedValueUpdate(1, _menuValues[1], MenuValueType::SYMBOL);
}

void Menu::EncoderUpdate()
{
    _encBtn->read();
    if((_tempEncoderCount[MENU_EDIT] != _enc->getCount()) && (menuMode == MENU_EDIT) || (_tempEncoderCount[MENU_SCROL] != _enc->getCount()) && (menuMode == MENU_SCROL))
    {
        if (menuMode == MENU_SCROL)
        {
            if (_enc->getCount()>1)
            {
                _enc->setCount(0);
            }
            if (_enc->getCount()<0)
            {
                _enc->setCount(1);
            }
            EncoderCounts[MENU_SCROL] = _enc->getCount();
            Menu::SelectionUpdate(_tempEncoderCount[MENU_SCROL]);
            _tempEncoderCount[MENU_SCROL] = EncoderCounts[MENU_SCROL];
            #ifdef LCD_DEBUG
                Serial.print("\nEncoder SCROL count = "+String((int32_t)_enc->getCount()));
                Serial.printf("\ntempEncoder SCROL count = %u",_tempEncoderCount[MENU_SCROL]);
            #endif
        }
        else
        {
            Menu::SelectionValueUpdate();
            #ifdef ENC_DEBUG
                Serial.print("\nEncoder EDIT count = "+String((int32_t)_enc->getCount()));
                Serial.printf("\ntempEncoder EDIT count = %u",_tempEncoderCount[MENU_EDIT]);
            #endif
        }
        
    }
    if(_encBtn->wasPressed())
    {
        if (menuMode == MENU_SCROL)
        {
            menuMode = MENU_EDIT;
            Menu::LoadSelectionValue();
            #ifdef LCD_DEBUG
                Serial.print("\nMenu in modalita EDIT");
            #endif
        }
        else
        {
            menuMode = MENU_SCROL;
            _enc->setCount(EncoderCounts[MENU_SCROL]);
            #ifdef LCD_DEBUG
                Serial.print("\nMenu in modalita SCROL");
            #endif
        }    
    }
}

void Menu::DisplayedValueUpdate(uint8_t MenuEntityNum, int32_t value, MenuValueType ValueType)
{
    if (ValueType==MenuValueType::NUMBER)
    {
        std::ostringstream ostr;
        ostr << _menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() +  1;
        std::string tempStr = ostr.str();
        int16_t tempStrLength = tempStr.length();
        ostr << value;
        tempStr = ostr.str();
        tempStrLength = tempStrLength + tempStr.length();
        #ifdef LCD_DEBUG
            Serial.printf("\nMenu NUMBER value update length check result: %u",tempStrLength);
        #endif
        if (tempStrLength<=_lcdColNum)
        {
            //Scrivo il campo
            _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1, _menuArray[MenuEntityNum].position.y);
            _lcd->print(value);
            _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + tempStr.length() - 1, _menuArray[MenuEntityNum].position.y);
            _lcd->printstr("  ");
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
            Serial.printf("\nMenu SYMBOL value update leght check result: %u",tempStrLength);
        #endif
        if (tempStrLength<=_lcdColNum)
        {
            _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1, _menuArray[MenuEntityNum].position.y);
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

void Menu::SelectionUpdate(uint8_t PreviousSelection)
{
    _lcd->setCursor(_menuArray[PreviousSelection].position.x - 1, _menuArray[PreviousSelection].position.y);
    _lcd->print(" ");
    _lcd->setCursor(_menuArray[EncoderCounts[MENU_SCROL]].position.x - 1, _menuArray[EncoderCounts[MENU_SCROL]].position.y);
    _lcd->write(2);
}

void Menu::SelectionValueUpdate()
{
    if(EncoderCounts[MENU_SCROL]==0)
    {
        if (_enc->getCount()<=_enc_upper)
        {
            _tempEncoderCount[MENU_EDIT] = _enc->getCount();
            EncoderCounts[MENU_EDIT] = (int32_t)_enc->getCount();
        }
        else
        {
            _tempEncoderCount[MENU_EDIT] = _enc_upper;
            EncoderCounts[MENU_EDIT] = (int32_t)_enc_upper;
            _enc->setCount(_enc_upper);
        }
        if (_enc->getCount()>=_enc_lower)
        {
            _tempEncoderCount[MENU_EDIT] = _enc->getCount();
            EncoderCounts[MENU_EDIT] = (int32_t)_enc->getCount();
        }
        else
        {
            _tempEncoderCount[MENU_EDIT] = _enc_lower;
            EncoderCounts[MENU_EDIT] = (int32_t)_enc_lower;
            _enc->setCount(_enc_lower);
        }
        Menu::DisplayedValueUpdate(EncoderCounts[MENU_SCROL], EncoderCounts[MENU_EDIT], MenuValueType::NUMBER);
    }
    else
    {
        if (_enc->getCount()>1)
        {
            _enc->setCount(0);
        }
        if (_enc->getCount()<0)
        {
            _enc->setCount(1);
        }
        _tempEncoderCount[MENU_EDIT] = _enc->getCount();
        EncoderCounts[MENU_EDIT] = (int32_t)_enc->getCount();
        Menu::DisplayedValueUpdate(EncoderCounts[MENU_SCROL], EncoderCounts[MENU_EDIT], MenuValueType::SYMBOL);
    }
    _menuValues[EncoderCounts[MENU_SCROL]] = EncoderCounts[MENU_EDIT];
}

void Menu::LoadSelectionValue()
{
    EncoderCounts[MENU_EDIT] = _menuValues[EncoderCounts[MENU_SCROL]];
    _tempEncoderCount[MENU_EDIT] = EncoderCounts[MENU_EDIT];
    _enc->setCount(EncoderCounts[MENU_EDIT]);
}