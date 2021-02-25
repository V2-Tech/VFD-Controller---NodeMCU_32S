#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <JC_Button.h>
#include "WK600.h"
#include <array>
#include <string>
#include <sstream>

#define ENC_DEBUG 
#define LCD_DEBUG

typedef enum
{
	MENU_EDIT,
    MENU_SCROL
} MenuMode;

typedef enum
{
	NUMBER,
    SYMBOL
} MenuValueType;

typedef struct {
    int8_t x;
    int8_t y;
    int8_t page;
} Coord;

typedef struct {
    String name;
    Coord position;
} MenuEntityList;

class Menu{
    public:
        Menu();
        void begin(LiquidCrystal_I2C &lcd, uint8_t lcd_cols,uint8_t lcd_rows, std::vector<MenuEntityList> menuEntityList, ESP32Encoder &enc, 
        uint32_t enc_lower_limit, uint32_t enc_upper_limit, Button &encBtn, WK600 &vfd, MotorParam motorParameters);
        void EncoderUpdate();
        void DisplayedValueUpdate(uint8_t MenuEntityNum, int32_t value, MenuValueType ValueType);
        void SelectionUpdate(uint8_t PreviousSelection);
        void SelectionValueUpdate();
        uint32_t EncoderCounts[2];
    private:
        LiquidCrystal_I2C* _lcd;
        ESP32Encoder* _enc;
        Button* _encBtn;
        WK600* _vfd;
        std::vector<MenuEntityList> _menuArray;
        std::vector<uint32_t> _menuValues;
        uint8_t _PageQuantity;
        uint8_t _MenuEntityQuantity;
        uint64_t _tempEncoderCount[2];
        uint8_t _ActPage;
        uint8_t _ActRow;
        uint8_t _lcdRowNum;
        uint8_t _lcdColNum;
        uint32_t _enc_lower;
        uint32_t _enc_upper;
        MenuMode menuMode;
        MotorParam _motorParameters;
        void LoadSelectionValue();
        std::string ToString(int n);
};