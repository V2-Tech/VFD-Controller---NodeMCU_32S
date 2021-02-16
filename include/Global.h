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

/////////////////////////////////////////////////////             
               /* COSTANTI */
/////////////////////////////////////////////////////
//OTA
#define USE_ARDUINO_OTA true
#define HOST_NAME "NodeMCUdebug"
#define USE_MDNS true

//Seriale
#define RXD2 16
#define TXD2 17
#define MAX485_DE_RE GPIO_NUM_32

//Encoder
#define ENCODER_PINA     2   // If the encoder moved in the wrong direction, swap PINA and PINB
#define ENCODER_PINB     3
#define ENCODER_BTN      4
#define ENCODER_STEPS_PER_NOTCH    4   // Change this depending on which encoder is used
#define ENCODER_HOLDTIME 500 //ms

//INPUT
#define OUTPUT_RUN        13
#define OUTPUT_RESET      12

//OUTPUT
#define INPUT_START      6
#define INPUT_ERROR       7

//PULSANTI
#define PULS_HOLD_TIME    2000 //ms

//PARAMETRI MOTORE
#define MOTOR_RPM 2850 //rpm
#define MOTOR_IN 5 //A
#define MOTOR_VN 400 // V
#define MOTOR_MAX_HZ 50 //HZ

//PARAMETRI LCD
#define LCD_ROW_NUM 4
#define LCD_COL_NUM 20
#define BLINKTIME 500 //ms
#define CYCLEREADTIME 1000 //ms

//INDIRIZZI MODBUS WK600
#define SPEED_ADDR 0x1000
#define COMMANDS_ADDR 0x2000
#define MONITOR_OUTPOWER_ADDR 0x7005
#define MONITOR_FDK_SPEED_ADDR 0x7013
#define MONITOR_COMM_ADDR 0x701C
#define MONITOR_STATUS_ADDR 0x703D
#define MONITOR_FAULTCODE_ADDR 0x703E
#define COMMAND_RUN 0x0001
#define COMMAND_STOP 0x0000
#define COMMAND_RESET 0x0010

//STATO DRIVE WK600
#define STATUS_RUN 0b00011
#define STATUS_STOP 0b00000
#define STATUS_ACCEL 0b00100
#define STATUS_DECEL 0b01000

/////////////////////////////////////////////////////             
               /* FUNZIONI GLOBALI*/
/////////////////////////////////////////////////////
//Seriale
void preTransmission();
void postTransmission();

//Encoder
void timerIsr();

//Inizializzazioni
void InitSerial();
void InitEncoder();
void InitGraphic();

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

class Global{
    public:
        int16_t EncoderValue;
    private:
};

extern Global GVL;
#endif