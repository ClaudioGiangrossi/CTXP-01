#include "navigazione.h"
#include "profili.h"
#include "memoria/memoria.h"
#include "grafica/grafica.h"
#include "driver/driver_pompe.h"
#include "timer/timer.h"
#include "var_globali.h"
#include "pin_defs.h"

WROVER_KIT_LCD      display;

/* DICHIARAZIONE VARIABILI DI NAVIGAZIONE*/
volatile bool       button1Pressed = false;
volatile bool       button2Pressed = false;
volatile uint8_t    posizioneEncoder = fermo;


/* FUNZIONI DI NAVIGAZIONE */

void resetFlagsUI() {
    button1Pressed = false;
    button2Pressed = false;
    posizioneEncoder = fermo;
}

void schermataIniziale() {
    display.begin();
    display.setRotation(1); // landscape, 3 = USB bottom right, 1 = USB upper left
    mostraSchermataIniziale(display);
}

uint8_t selezioneMenu() {

    mostraMenu(display);

    uint8_t selezione = 1;
    muoviCursoreMenu(display, selezione);

    do
    {
        if (posizioneEncoder == destra)
        {
            if (selezione < wifi)
            {
                selezione++;
                muoviCursoreMenu(display, selezione);

            }
        }
        else if (posizioneEncoder == sinistra)
        {
            if (selezione > pompa)
            {
                selezione--;
                muoviCursoreMenu(display, selezione);
            }
        }

        posizioneEncoder = fermo;

        #ifndef NDEBUG
        Serial.printf("Selezione MENU: %d\n", selezione);
        #endif

        delay(50);

    } while (button1Pressed == false);

    if (button1Pressed == true)
    {
        evidenziaSelezioneMenu(display, selezione);
    }

    resetFlagsUI();

    return selezione;
}

uint8_t selezionePompa() {

    uint8_t selezione_pompa = pompaSelezionata; // permette di gestire errori
    mostraSelezionePompa(display, selezione_pompa);

    do
    {
        if (posizioneEncoder == sinistra)
        {
            selezione_pompa = piccola;
            mostraSelezionePompa(display, selezione_pompa);
            delay(130);
        }
        else if (posizioneEncoder == destra)
        {
            selezione_pompa = grande;
            mostraSelezionePompa(display, selezione_pompa);
        }

        posizioneEncoder = fermo;

        #ifndef NDEBUG
        Serial.printf("Pompa: %d\n", selezione_pompa);
        #endif

        delay(80);

    }   while ((button1Pressed == false) && (button2Pressed == false));

    if (button2Pressed == true)
    {
        #ifndef NDEBUG
        Serial.println("BACK");
        #endif

        return menu;
    }

    if (button1Pressed == true)
    {
        if (registraPompaSelezionata(selezione_pompa))
        {
            #ifndef NDEBUG
            Serial.printf("DRIVED POMPA %d", selezione_pompa);
            #endif
            /* TODO: conferma visiva sul display */
        }
    }

    delay(150);
    resetFlagsUI();
    return menu;

}

uint8_t menuSteady() {

    enum        opzioni {run, velocita, rotazione};
    uint8_t     selezione = run;
    uint8_t     pompa = pompaSelezionata;
    mostraSteady(display, selezione, true);

    do
    {
        if (posizioneEncoder == sinistra)
        {
            if (selezione > run)
            {
            selezione--;
            mostraSteady(display, selezione, false);
            }
        }
        else if (posizioneEncoder == destra)
        {
            if (selezione < rotazione)
            {
            selezione++;
            mostraSteady(display, selezione, false);
            }
        }

        posizioneEncoder = fermo;
        
        #ifndef NDEBUG
        if      (selezione == run)          {Serial.println("AVVIO?");}
        else if (selezione == velocita)     {Serial.println("VELOCITÀ?");}
        else if (selezione == rotazione)    {Serial.println("ROTAZIONE?");}
        #endif

        delay(150);

    } while ((button1Pressed == false) && (button2Pressed == false));

    if(button2Pressed)
    {
        button2Pressed = false;
        return menu;
    }

    if (button1Pressed)
    {
        button1Pressed = false;

        if (selezione == run)
        {
            runSteady(pompa);
        }
        else if (selezione == velocita)
        {
            delay(150);
            uint8_t velocitaScelta = selezionaVelocitaDefault(velocitaDefault);
            scriviVelocitaDefault(velocitaScelta);
            velocitaDefault = velocitaScelta;
        }
        else if (selezione == rotazione)
        {
            sensoRotazione(pompa);
        }

        mostraMenu(display);
        muoviCursoreMenu(display, steady);

        return steady;
    }

    #ifndef NDEBUG
    Serial.println("Non dovrei essere qui -> fine menuSteady");
    #endif

    return menu;
    
}

void runSteady(uint8_t pompa) {
    uint8_t         velocita = velocitaDefault;
    uint64_t        ultimoAggiornamento = millis();
    uint64_t        lastSerialRequest = millis();
    uint64_t        intervalloAggiornamento = 5000; // ms
    uint64_t        serialRequestTime = 1000; // ms
    uint8_t         stopState = false;

    setPinRotazione();
    dacWrite(DAC, velocita);
    digitalWrite(STOP_OUT, stopState);

    mostraRunSteady(display, velocita);
    Serial.println("Valori validi di velocità: da 0 a 255.");

    while ((faultSVILUPPO(pompa) == false) && (button2Pressed == false))
    {
        if (posizioneEncoder != fermo)
        {
            if ((posizioneEncoder == destra) && (velocita < 255))
            {
                dacWrite(DAC, ++velocita);
            }
            else if (posizioneEncoder == sinistra && (velocita > 0))
            {
                dacWrite(DAC, --velocita);
            }

            posizioneEncoder = fermo;
            aggiornaVelocitaMostrata(display, velocita);

            #ifndef NDEBUG
            Serial.printf("\nVelocita: %u", velocita);
            #endif
        }

        if (button1Pressed == true)
        {
            if (digitalRead(pushButton1) == HIGH)   // controllo se ho già rilasciato il pushbutton
            {
                button1Pressed = false;
                stopState = !stopState;
                digitalWrite(STOP_OUT, stopState);
                aggiornaStatoSteady(display, stopState);
            }
        }
/*
        if ((millis() - ultimoAggiornamento) > intervalloAggiornamento)
        {
            aggiornamentoDati(pompa);
            ultimoAggiornamento = millis();
            // displayDati();
        }
*/

        if (millis() - lastSerialRequest > serialRequestTime)
        {
            if (Serial.available() > 0)
            {
                uint8_t datoRicevuto = Serial.parseInt();
                if ((datoRicevuto > 0) && (datoRicevuto < 256))
                {
                    Serial.println(datoRicevuto);
                    velocita = datoRicevuto;
                    dacWrite(DAC, velocita);
                    aggiornaVelocitaMostrata(display, velocita);
                }
            }
        }
    }

    if (button2Pressed == true)
    {
        while (velocita > 0)
        {
            button2Pressed = false;
            dacWrite(DAC, --velocita);
            delay(1);
        }
    }

}

uint8_t selezionaVelocitaDefault(uint8_t velocitaAttuale) {
    
    resetFlagsUI();
    
    uint8_t velocitaSelezionata = velocitaAttuale;

    mostraVelocitaSelezionata(display, velocitaSelezionata, true);

    while ((button1Pressed == false) && (button2Pressed == false))
    {
        if (posizioneEncoder != fermo)
        {
            if (posizioneEncoder == destra)
            {
                if (velocitaSelezionata < 255)
                {
                    velocitaSelezionata++;
                }
                else if (velocitaSelezionata == 255)
                {
                    velocitaSelezionata = 0;
                }
            }
            else if (posizioneEncoder == sinistra)
            {
                if (velocitaSelezionata > 0)
                {
                velocitaSelezionata--;
                }
                else if (velocitaSelezionata == 0)
                {
                    velocitaSelezionata = 255;
                }
            }

            posizioneEncoder = fermo;
            mostraVelocitaSelezionata(display, velocitaSelezionata, false);
        }

        delay(10);

        #ifndef NDEBUG
        Serial.printf("\nVelocita Selezionata: %u", velocitaSelezionata);
        #endif
    }

    if (button2Pressed)
    {
        button2Pressed = false;
        delay(150);
    }
 
    if (button1Pressed)
    {
        button1Pressed = false;
        delay(150);
        return velocitaSelezionata;
    }

    return velocitaAttuale;
}

bool sensoRotazione(uint8_t pompa) {
    
    uint8_t selezione = rotazione;

    switch (pompa)
    {
    case piccola:
        mostraSelezioneRotazione(display, !selezione, true);
        break;
    case grande:
        mostraSelezioneRotazione(display, selezione, true);
    default:
        break;
    }

    delay(200);
    resetFlagsUI();

    do
    {
        if (posizioneEncoder == sinistra)
        {
            if (pompa == piccola)
            {
                selezione = 1;
            }
            else if (pompa == grande)
            {
                selezione = 0;
            }

            mostraSelezioneRotazione(display, 0, false);

            posizioneEncoder = fermo;

            #ifndef NDEBUG
            Serial.println("SENSO ANTIORARIO");
            #endif
        }
        
        else if (posizioneEncoder == destra)
        {
            if (pompa == piccola)
            {
                selezione = 0;
            }
            else if (pompa == grande)
            {
                selezione = 1;
            }

            mostraSelezioneRotazione(display, 1, false);

            posizioneEncoder = fermo;

            #ifndef NDEBUG
            Serial.println("SENSO ORARIO");
            #endif
        }
        
    } while ((button1Pressed == false) && (button2Pressed == false));

    if (button1Pressed)
    {
        rotazione = selezione;
        printRotazione(display, pompa, selezione);
        delay(200);
        return true;
    }

    delay(200);
    return false;
}

uint8_t menuProfilo() {

    profilo_t database = {
        "head",
        1,
        NULL,
        NULL,
        {0}
    };

    profilo_t *ptrDatabase = &database;

    //caricaDatabaseProfili((void*) ptrDatabase);

    if (caricaDatabase(ptrDatabase) == NULL)
    {
        Serial.println("ERRORE: database nullo");
        delay(2000);
        return menu;
    }
    else
    {
        visualizzaDatabase(ptrDatabase);
    }

    enum        opzioni {run, sceltaProfilo, rotazione};
    uint8_t     selezione = run;
    uint8_t     pompa = pompaSelezionata;

    mostraProfilo(display, selezione, true);

    resetFlagsUI();


    #ifndef NDEBUG
    Serial.println("RUN -> Opzione 0");    
    Serial.println("PROFILO -> Opzione 1");
    Serial.println("ROTAZIONE -> Opzione 2");    
    #endif
    
    do
    {
        if (posizioneEncoder == sinistra)
        {
            if ((selezione == sceltaProfilo) || (selezione == rotazione) )
            {
                selezione--;
                mostraProfilo(display, selezione, false);
            }
           
            #ifndef NDEBUG
            Serial.printf("\nOpzione menu profilo: %u", selezione);
            #endif
        }
        else if (posizioneEncoder == destra)
        {
            if ((selezione == run) || (selezione == sceltaProfilo))
            {
                selezione++;
                mostraProfilo(display, selezione, false);
            }

            #ifndef NDEBUG
            Serial.printf("\nOpzione menu profilo: %u", selezione);
            #endif            
        }

        posizioneEncoder = fermo;
        delay(80);

    } while ((button1Pressed == false) && (button2Pressed == false));

    if(button2Pressed)
    {
        button2Pressed = false;
        return menu;
    }

    if (button1Pressed)
    {
        button1Pressed = false;

        if (selezione == 1)
        {
            if (selezioneProfilo())
            {
                runProfilo(pompa);
            }
        }
        else if (selezione == 0)
        {
            runProfilo(pompa);
        }
        else if (selezione == 2)
        {
            sensoRotazione(pompa);
        }

        return profilo;
    }

    #ifndef NDEBUG
    Serial.println("\nNon dovrei essere qui -> fine menuProfilo\n");
    #endif

    return menu;
    
}

bool selezioneProfilo() {
    resetFlagsUI();
    delay(100);



    #ifndef NDEBUG
    Serial.println("\nProfilo selezionato correttamente");
    #endif
    
    delay(100);
    return true;
}

void runProfilo(uint8_t pompa) {
    #ifndef NDEBUG
    Serial.println("\nRun profilo");
    #endif
/*

    resetFlagsUI();
    uint8_t     profilo = profiloCaricato;
    uint16_t    campioniTotali = dimensioniProfili[profilo];
    uint8_t     buffer[campioniTotali];
    uint64_t    ultimoAggiornamento = millis();
    uint64_t    intervalloAggiornamento = 5000; // ms
    uint8_t     stopState = false;


    for (uint16_t i = 0; i < campioniTotali; i++)
    {
        buffer[i] = * (listaProfili[profilo] + i);
    }

    digitalWrite(STOP_OUT, stopState);

    if (outputTimerEnable() == true)
    {
        uint16_t campioneAttuale = 0;

        while (button2Pressed == false)
        {
            if (outputTimerFlag == true)
            {
                outputTimerFlag = false;
                dacWrite(DAC, buffer[campioneAttuale++]);
            }

            if (campioneAttuale > campioniTotali - 1)
            {
                campioneAttuale = 0;
            }

            if ((millis() - ultimoAggiornamento) > intervalloAggiornamento)
            {
            aggiornamentoDati(pompa);
            ultimoAggiornamento = millis();
            // displayDati();

            }
            // TODO grafica

            if (button1Pressed == true)
            {
                button1Pressed = false;
                stopState = !stopState;
                digitalWrite(STOP_OUT, stopState);
            }
        }


        outputTimerDisable();

        if (button2Pressed == true)
        {
            button2Pressed = false;
            uint8_t valoreDecelerazione = buffer[campioneAttuale];
            while (valoreDecelerazione > 0)
            {
                dacWrite(DAC, --valoreDecelerazione);
                delay(1);
            }
        }
    }
    else
    {
        Serial.println("ERRORE ATTIVAZIONE TIMER OUTPUT");
    }

    #ifndef NDEBUG
    Serial.printf("\nFINE PROFILO\n");
    #endif

    delay(100);
*/
}

uint8_t menuWiFi() {

    mostraWiFi(display);
    resetFlagsUI();

    while (button2Pressed == false)
    {
        // TODO
    }

    return menu;
    
}