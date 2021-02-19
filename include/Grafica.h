#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>

class Menu{
    public:
        Menu();
        void begin(LiquidCrystal_I2C &lcd, uint8_t, String[]);
        void update();
    private:
        LiquidCrystal_I2C* _lcd;
        uint8_t _PageNumber;
        String _PageRows[];
};