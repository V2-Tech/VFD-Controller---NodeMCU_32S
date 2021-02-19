#pragma once
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <JC_Button.h>
#include <array>
#include <string>

class Menu{
    public:
        Menu();
        void begin(LiquidCrystal_I2C &lcd, String array[][4], size_t pags, size_t rows, ESP32Encoder &enc, Button &encBtn);
        void update();
        uint32_t EncoderValue;
    private:
        LiquidCrystal_I2C* _lcd;
        ESP32Encoder* _enc;
        Button* _encBtn;
        uint8_t _PageNumber;
        String _PageRows[];
        void EncoderUpdate();
        uint32_t _tempEncoderValue;
};