#pragma once
#include "Arduino.h"

class Menu{
    public:
        void begin(uint8_t, uint8_t[]);

    private:
        uint8_t _PageNumber;
        uint8_t _PageRows[];
};