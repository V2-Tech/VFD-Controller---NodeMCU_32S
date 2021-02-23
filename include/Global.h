#ifndef GLOBAL_H
#define GLOBAL_H
/////////////////////////////////////////////////////             
               /* ABILITAZIONI FEATURES */
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////             
               /* LIBRERIE */
/////////////////////////////////////////////////////
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include <ArduinoOTA.h>
#include "RemoteDebug.h"        //https://github.com/JoaoLopesF/RemoteDebug
#include <HardwareSerial.h>
#include <ModbusMaster.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <JC_Button.h>
#include <Ticker.h>
#include "WK600.h"
#include "Grafica.h"
#include <vector>
#include <string>

/////////////////////////////////////////////////////             
               /* COSTANTI */
/////////////////////////////////////////////////////
//OTA
#define USE_ARDUINO_OTA true
#define HOST_NAME "NodeMCUdebug"
#define USE_MDNS true

//Comunicazione
#define I2Cscl GPIO_NUM_22
#define I2Csda GPIO_NUM_21
#define Serial2TX2 GPIO_NUM_17
#define Serial2RX2 GPIO_NUM_16
#define Serial2DERE GPIO_NUM_4

//Encoder
#define InEncoderCLK GPIO_NUM_26
#define InEncoderDT GPIO_NUM_25
#define InEncoderBUT GPIO_NUM_33
#define ENCODER_STEPS_PER_NOTCH    4   // Change this depending on which encoder is used
#define ENCODER_HOLDTIME 1000 //ms

//Stepper
#define InStepperAlarm GPIO_NUM_36 //(serve pull-up ext)
#define InStepperInPos GPIO_NUM_39 //(serve pull-up ext)
#define OutStepperEN GPIO_NUM_23
#define OutStepperDIR GPIO_NUM_19
#define OutStepperSTEP GPIO_NUM_18

// Generic INPUT
#define PulsStartMotore GPIO_NUM_14
#define PulsStopMotore GPIO_NUM_27
#define PulsStartTaglio GPIO_NUM_12
#define PulsStopTaglio GPIO_NUM_13
#define FcsDestro GPIO_NUM_32
#define FcsSinistro GPIO_NUM_5
#define InVFDErr GPIO_NUM_34 //(serve pull-up ext)

//Generic OUTPUT
//None for now

//PULSANTI
#define PULS_HOLD_TIME    2000 //ms

//PARAMETRI LCD
#define LCD_ROW_NUM 4
#define LCD_COL_NUM 20
#define BLINKTIME 500 //ms
#define CYCLEREADTIME 1000 //ms
#define PAGES_NUM 2

//PARAMETRI INVERTER
#define DRIVE_MODBUS_ID 0x0A
/////////////////////////////////////////////////////             
               /* FUNZIONI GLOBALI*/
/////////////////////////////////////////////////////
//Seriale
void preTransmission();
void postTransmission();

//Encoder
void timerIsr();

/////////////////////////////////////////////////////             
               /* VARIABILI GLOBALI */
/////////////////////////////////////////////////////
//Singleton GVL
/*
class gvl
{
    public:
        gvl(const gvl&) = delete;

        static gvl& Get()
        {
            static gvl gvl_instance;
            return gvl_instance;
        }

        static int16_t EncoderValueGet(){ return Get().i_EncoderValue; }
        void EncoderValueSet(int16_t value){ i_EncoderValue = value; }
    private:
        gvl(){}
        int16_t i_EncoderValue = 0;
};
*/

class Global
{
    public:
        Global();

        //Variabili
        int16_t EncoderValue;
        std::vector<MenuEntityList> MenuEntitys;
        Coord MenuEntityPos[];
        MotorParam motorParameter;
        byte SymbolRightArrow[];
        byte SymbolLeftArrow[];
        byte SymbolSelected[];

        //Funzioni
        void begin();

    private:
        //Variabili

        //Funzioni
};



extern Global GVL;
extern RemoteDebug Debug;
extern ModbusMaster node;
extern ESP32Encoder encoder;
extern LiquidCrystal_I2C lcd;
extern Menu menu;
extern Button EncBtn;
extern WK600 vfd;

#endif