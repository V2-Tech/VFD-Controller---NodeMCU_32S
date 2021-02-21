#include <WK600.h>

WK600::WK600()
{

}

void WK600::begin(ModbusMaster &CommMaster)
{
    _CommMaster = &CommMaster;
}