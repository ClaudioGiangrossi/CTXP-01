#include "driver_pompe.h"
#include "var_globali.h"
#include "pin_defs.h"


bool fault(uint8_t pompa) {
    if (pompa == piccola)
    {
        if (digitalRead(ALARM_INPUT) == 1)
        {
            return true;
        }
        else
            return false;
    }
    else if (pompa == grande)
    {
        if (digitalRead(ALARM_INPUT) == 0)
        {
            return true;
        }
        else
            return false;
    }

    #ifndef NDEBUG
    Serial.println("Non dovrei essere qui -> fine di fault()");
    #endif

    return true;
}

bool faultSVILUPPO(uint8_t pompa){
   return false; // da usare solo in fase di sviluppo per semplicità hardware
}

void setPinRotazione(){
    digitalWrite(ROTATION_OUT, rotazione);

    #ifndef NDEBUG
    Serial.printf("\nRotazione: %u\n", rotazione);
    #endif
}


void aggiornamentoDati(uint8_t pompa) {
    
    // TODO: fare funzionare realmente questa sezione

    // leggiTacho();
    // leggiSensore();

    #ifndef NDEBUG
    Serial.println("\nTACHO AGGIORNATO");
    Serial.println("\nDATO DI PORTATA AGGIORNATO");
    #endif
}
