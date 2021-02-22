#include <WK600.h>

WK600::WK600()
{

}

void WK600::begin(ModbusMaster &CommMaster)
{
    _CommMaster = &CommMaster;
}

void WK600::setSpeed(uint16_t speed /*%*/)
{
    uint8_t Return = _CommMaster->writeSingleRegister(SETPOINT_ADDR, speed*100);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearTransmitBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore invio del setpoint: %u",Return);
            _CommMaster->clearTransmitBuffer();
        #endif
    }

}

void WK600::run_forw()
{
    uint8_t Return = _CommMaster->writeSingleRegister(COMMANDS_ADDR, VFDCommand::COMMAND_FORW);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearTransmitBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore invio comando di run forward: %u",Return);
        #endif
        _CommMaster->clearTransmitBuffer();
    }
}

void WK600::run_rev()
{
    uint8_t Return = _CommMaster->writeSingleRegister(COMMANDS_ADDR, VFDCommand::COMMAND_REV);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearTransmitBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore invio comando di run reverse: %u",Return);
            _CommMaster->clearTransmitBuffer();
        #endif
    }
}
void WK600::stop()
{
    uint8_t Return = _CommMaster->writeSingleRegister(COMMANDS_ADDR, VFDCommand::COMMAND_STOP);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearTransmitBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore invio comando di run reverse: %u",Return);
            _CommMaster->clearTransmitBuffer();
        #endif
    }
}

void WK600::free_stop()
{
    uint8_t Return = _CommMaster->writeSingleRegister(COMMANDS_ADDR, VFDCommand::COMMAND_FREE_STOP);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearTransmitBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore invio comando di run reverse: %u",Return);
            _CommMaster->clearTransmitBuffer();
        #endif
    }
}

void WK600::reset()
{
    uint8_t Return = _CommMaster->writeSingleRegister(COMMANDS_ADDR, VFDCommand::COMMAND_RESET);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearTransmitBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore invio comando di run reverse: %u",Return);
            _CommMaster->clearTransmitBuffer();
        #endif
    }    
}

int16_t WK600::getActSetpoint()
{
    uint8_t Return = _CommMaster->readHoldingRegisters(SETPOINT_ADDR,1);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearResponseBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore lettura setpoint: %u",Return);
            _CommMaster->clearResponseBuffer();
        #endif
    } 
}

int16_t WK600::getActVin()
{
    uint8_t Return = _CommMaster->readHoldingRegisters(MONITOR_INVOLTAGE_ADDR,1);
    if (Return==_CommMaster->ku8MBSuccess){
        _CommMaster->clearResponseBuffer();
    }
    else{
        #ifdef COMM_DEBUG
            Serial.printf("\nErrore lettura tensione ingresso Vin: %u",Return);
            _CommMaster->clearResponseBuffer();
        #endif
    } 
}