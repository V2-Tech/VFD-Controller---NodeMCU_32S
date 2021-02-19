#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <array>
#include <string>

class Menu{
    public:
        Menu();
        void begin(LiquidCrystal_I2C &lcd, String array[][4], size_t, size_t);
        void update();
    private:
        LiquidCrystal_I2C* _lcd;
        uint8_t _PageNumber;
        String _PageRows[];
};