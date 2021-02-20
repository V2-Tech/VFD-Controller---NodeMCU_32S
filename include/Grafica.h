#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <JC_Button.h>
#include "WK600.h"
#include <array>
#include <string>

#define ENC_DEBUG 
#define LCD_DEBUG

typedef enum
{
	MENU_EDIT,
    MENU_SCROL
} MenuMode;

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
        void begin(LiquidCrystal_I2C &lcd, std::vector<MenuEntityList> menuEntityList, ESP32Encoder &enc, Button &encBtn, WK600 &vfd);
        void update();
        uint32_t EncoderValue;
    private:
        LiquidCrystal_I2C* _lcd;
        ESP32Encoder* _enc;
        Button* _encBtn;
        WK600* _vfd;
        std::vector<MenuEntityList> _menuArray;
        uint8_t _PageQuantity;
        uint8_t _MenuEntityQuantity;
        void EncoderUpdate();
        uint64_t _tempEncoderCount;
        uint8_t _ActPage;
        uint8_t _ActRow;
        MenuMode menuMode;
};