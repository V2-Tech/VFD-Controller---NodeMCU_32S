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
        uint32_t enc_lower_limit, uint32_t enc_upper_limit, Button &encBtn, WK600 &vfd, MotorParam motorParameters)
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
    _motorParameters = motorParameters;

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
    MenuValues.push_back((int32_t)_vfd->getActSetpoint());
    MenuValues.push_back(1);

    //Inizializzo il valore del encoder
    _enc->setCount(MenuValues[0]);
}

void Menu::EncoderUpdate()
{
    if (_vfd->_ConnectionOK && !_xFirstGraphicDraw){
        _xFirstGraphicDraw = true;
        FirstRead();
    }
    if (_vfd->_ConnectionOK && _xFirstGraphicDraw)
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
}

void Menu::FirstRead()
{
    //Disegno i primi valori del menu
    Menu::DisplayedValueUpdate(0, MenuValues[0], MenuValueType::NUMBER);
    Menu::DisplayedValueUpdate(1, MenuValues[1], MenuValueType::SYMBOL);
}

void Menu::DisplayedValueUpdate(uint8_t MenuEntityNum, int32_t value, MenuValueType ValueType)
{
    if (ValueType==MenuValueType::NUMBER)
    {
        int16_t tempMinFieldResetPos = _lcdColNum - 1;
        String tempStr = _menuArray[MenuEntityNum].name;
        std::string str = ToString(value);
        int16_t tempStrLength = tempStr.length() + str.length();
        #ifdef LCD_DEBUG
            Serial.printf("\nMenu NUMBER value update. Menu string length: %u",tempStr.length());
            Serial.printf("\nMenu NUMBER value update. Menu value: %s",str.c_str());
            Serial.printf("\nMenu NUMBER value update. Menu value string length: %u", str.length());
        #endif
        tempStrLength += _menuArray[MenuEntityNum].position.x + 1;
        #ifdef LCD_DEBUG
            Serial.printf("\nMenu NUMBER value update length check result: %u",tempStrLength);
        #endif
        if (tempStrLength<=_lcdColNum)
        {
            //Scrivo il campo
            _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1, _menuArray[MenuEntityNum].position.y);
            _lcd->print(value);
            /*Resetto l eventuale carattere non sovrascritto*/
            //Controllo se ho altre MenuEntity sulla stessa riga
            for (int16_t k=0; k<_MenuEntityQuantity; k++)
            {
                if (_menuArray[MenuEntityNum].position.y==_menuArray[k].position.y)
                {
                    #ifdef LCD_DEBUG
                        Serial.printf("\nFound menu entity on same row: %s",_menuArray[k].name.c_str());
                    #endif
                    if (_menuArray[MenuEntityNum].position.x<tempMinFieldResetPos && _menuArray[k].position.x>_menuArray[MenuEntityNum].position.x)
                    {
                        tempMinFieldResetPos = _menuArray[k].position.x - 1;
                    }
                }
            }
            #ifdef LCD_DEBUG
                Serial.printf("\nReset field coord X min: %u",tempMinFieldResetPos);
            #endif
            for (int16_t k=0; k<=tempMinFieldResetPos-tempStrLength; k++)
            {
                _lcd->setCursor(tempStrLength + k, _menuArray[MenuEntityNum].position.y);
                _lcd->printstr(" ");
                #ifdef LCD_DEBUG
                    Serial.printf("\nReset field coord X: %u",tempStrLength + k);
                #endif
            }
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

void Menu::DisplayedStringUpdate(uint8_t MenuEntityNum, String string)
{
    int16_t tempMinFieldResetPos = _lcdColNum - 1;
    String tempStr = _menuArray[MenuEntityNum].name;
    int16_t tempStrLength = tempStr.length() + string.length();
    #ifdef LCD_DEBUG
        Serial.printf("\nMenu NUMBER string update. Menu string length: %u",tempStr.length());
        Serial.printf("\nMenu NUMBER string update. Menu value: %s",str.c_str());
        Serial.printf("\nMenu NUMBER string update. Menu value string length: %u", str.length());
    #endif
    tempStrLength += _menuArray[MenuEntityNum].position.x + 1;
    #ifdef LCD_DEBUG
        Serial.printf("\nMenu NUMBER string update length check result: %u",tempStrLength);
    #endif
    if (tempStrLength<=_lcdColNum)
    {
        //Scrivo il campo
        _lcd->setCursor(_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1, _menuArray[MenuEntityNum].position.y);
        _lcd->printstr(string.c_str());
        /*Resetto l eventuale carattere non sovrascritto*/
        //Controllo se ho altre MenuEntity sulla stessa riga
        for (int16_t k=0; k<_MenuEntityQuantity; k++)
        {
            if (_menuArray[MenuEntityNum].position.y==_menuArray[k].position.y)
            {
                #ifdef LCD_DEBUG
                    Serial.printf("\nFound menu entity on same row: %s",_menuArray[k].name.c_str());
                #endif
                if (_menuArray[MenuEntityNum].position.x<tempMinFieldResetPos && _menuArray[k].position.x>_menuArray[MenuEntityNum].position.x)
                {
                    tempMinFieldResetPos = _menuArray[k].position.x - 1;
                }
            }
        }
        #ifdef LCD_DEBUG
            Serial.printf("\nReset field coord X min: %u",tempMinFieldResetPos);
        #endif
        for (int16_t k=0; k<=tempMinFieldResetPos-tempStrLength; k++)
        {
            _lcd->setCursor(tempStrLength + k, _menuArray[MenuEntityNum].position.y);
            _lcd->printstr(" ");
            #ifdef LCD_DEBUG
                Serial.printf("\nReset field coord X: %u",tempStrLength + k);
            #endif
        }
        #ifdef LCD_DEBUG
            Serial.printf("\nCoord X: %u",_menuArray[MenuEntityNum].position.x + _menuArray[MenuEntityNum].name.length() + 1);
            Serial.printf("\nCoord Y: %u",_menuArray[MenuEntityNum].position.y);
            Serial.printf("\nValue: %s", string);  
        #endif
    }
    else
    {
        #ifdef LCD_DEBUG
            Serial.printf("\nMenu value update leght check failed. Total length is: %u",tempStrLength);
        #endif
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
        int32_t tempVal = _vfd->PercentToRPM(EncoderCounts[MENU_EDIT], _motorParameters);
        Menu::DisplayedValueUpdate(EncoderCounts[MENU_SCROL], tempVal, MenuValueType::NUMBER);
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
    MenuValues[EncoderCounts[MENU_SCROL]] = EncoderCounts[MENU_EDIT];
}

void Menu::LoadSelectionValue()
{
    EncoderCounts[MENU_EDIT] = MenuValues[EncoderCounts[MENU_SCROL]];
    _tempEncoderCount[MENU_EDIT] = EncoderCounts[MENU_EDIT];
    _enc->setCount(EncoderCounts[MENU_EDIT]);
}

void Menu::GraphicValueUpdate()
{
    int32_t tempVal;
    /*
    tempVal = _vfd->getActSpeed();
    tempVal = _vfd->PercentToRPM(tempVal, _motorParameters);
    Menu::DisplayedValueUpdate(2,tempVal,MenuValueType::NUMBER);
    */
    tempVal = _vfd->getActOutPower();
    Menu::DisplayedValueUpdate(3,tempVal,MenuValueType::NUMBER);

    VFDStatus tempStatus = _vfd->getStatus();
    if (tempStatus==VFDStatus::STATUS_RUN_FORW)
    {
        Menu::DisplayedStringUpdate(4, "RF");
    }
    else if (tempStatus==VFDStatus::STATUS_RUN_REV)
    {
        Menu::DisplayedStringUpdate(4, "RR");
    }
    else if (tempStatus==VFDStatus::STATUS_STOP)
    {
        Menu::DisplayedStringUpdate(4, "ST");
    }
    else
    {
        Menu::DisplayedStringUpdate(4, "???");
    }
}

std::string  Menu::ToString(int n)
{
    std::ostringstream stm;
    stm << n;
    return stm.str();
}