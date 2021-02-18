#include "Global.h"

void preTransmission()
{
  digitalWrite(Serial2DERE, 1);
}
void postTransmission()
{
  digitalWrite(Serial2DERE, 0);
}

const char* ssid     = "VFD-Manager-AP";
const char* password = "12345678";