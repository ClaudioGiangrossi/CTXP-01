#ifndef DRIVER_POMPE_H
#define DRIVER_POMPE_H

#include <Arduino.h>
#include <WROVER_KIT_LCD.h>



bool fault(uint8_t pompa);
bool faultSVILUPPO(uint8_t pompa);
void setPinRotazione();
void aggiornamentoDati(uint8_t pompa);


#endif