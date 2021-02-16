#include "Global.h"

void preTransmission()
{
  digitalWrite(MAX485_DE_RE, 1);
}
void postTransmission()
{
  digitalWrite(MAX485_DE_RE, 0);
}

const char* ssid     = "VFD-Manager-AP";
const char* password = "12345678";

RemoteDebug Debug;

ModbusMaster node;