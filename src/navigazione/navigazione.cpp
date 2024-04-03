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
        delay(200);

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
        return menu;
    }

    if (button1Pressed == true)
    {
        if (registraPompaSelezionata(selezione_pompa) == false)
        {
            Serial.println("ERRORE in selezionePompa");
            // TODO: mostrare l'errore sul display
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

    Serial.println("Non dovrei essere qui -> fine menuSteady");
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

    do
    {
        if (posizioneEncoder == sinistra)
        {
            if ((selezione == sceltaProfilo) || (selezione == rotazione) )
            {
                selezione--;
                mostraProfilo(display, selezione, false);
            }
        }
        else if (posizioneEncoder == destra)
        {
            if ((selezione == run) || (selezione == sceltaProfilo))
            {
                selezione++;
                mostraProfilo(display, selezione, false);
            }

        }

        #ifndef NDEBUG
        Serial.printf("%u, %u \n", button1Pressed, button2Pressed);
        #endif

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
            if (selezioneProfilo((void*) ptrDatabase))
            {
                profilo_t *profiloSelezionato = cercaProfilo(ptrDatabase, ultimoProfiloCaricato);
                runProfilo(pompa, (void*) profiloSelezionato);
                mostraMenu(display);
            }
        }
        else if (selezione == 0)
        {
            profilo_t *profiloSelezionato = cercaProfilo(ptrDatabase, ultimoProfiloCaricato);
            runProfilo(pompa, (void*) profiloSelezionato);
            mostraMenu(display);
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

bool selezioneProfilo(void *ptrDatabase_) {

    /* Dichiarazioni e init puntatori*/
    String profiloAttuale = ultimoProfiloCaricato;

    profilo_t *ptrDatabase = (profilo_t*) ptrDatabase_;     // cast necessario perchè sono scarso

    profilo_t *profiloSelezionato = cercaProfilo(ptrDatabase, profiloAttuale);

    if (profiloSelezionato == NULL)
    {
        Serial.println("ERRORE in selezioneProfilo");
        return false;
    }

    /* Display della lista */
    listaProfili(profiloSelezionato);

    #ifndef NDEBUG
    Serial.printf("%u, %u \n", button1Pressed, button2Pressed);
    Serial.println("---------------------------------------");
    #endif

    delay(500);
    resetFlagsUI();

    /* Menu di selezione del profilo, con lista a scorrimento */
    do
    {
        if (posizioneEncoder != fermo)
        {
            if (posizioneEncoder == sinistra)
            {   
                if ((profiloSelezionato->pre != NULL) && (profiloSelezionato->pre->nome != "head"))
                {
                    profiloSelezionato = profiloSelezionato->pre;
                }
            }
            else if (posizioneEncoder == destra)
            {
                if (profiloSelezionato->next != NULL)
                {
                    profiloSelezionato = profiloSelezionato->next;
                }
            }

            if (profiloSelezionato->nome != "head")     // controllo forse ridondante
            {
                listaProfili(profiloSelezionato);
            }

            #ifndef NDEBUG
            Serial.printf("%u, %u \n", button1Pressed, button2Pressed);
            #endif

            posizioneEncoder = fermo;
            delay(250);

        }

    } while((button2Pressed == false) && (button1Pressed == false));

    /* Se si decide di tornare indietro */
    if (button2Pressed == true)
    {
        return false;
    }

    /* Se si decide di selezionare un nuovo profilo */
    if (button1Pressed == true)
    {
        // evitiamo di caricare per errore roba che non dovrebbe essere caricata
        if ((profiloSelezionato != NULL) && (profiloSelezionato->nome != "head"))
        {
            scriviProfiloCaricato(profiloSelezionato->nome);
            ultimoProfiloCaricato = profiloSelezionato->nome;
        }

        delay(100);
        return true;
    }

    return false;   // non dovrei mai arrivare qui
}

void listaProfili(void * ptrProfiloAttuale_) {
    profilo_t *ptrProfiloAttuale = (profilo_t*) ptrProfiloAttuale_;

    if ((ptrProfiloAttuale->pre == NULL) || (ptrProfiloAttuale->pre->nome == "head"))
    {                                                // ptrProfiloAttuale punta a head, lista vuota
      mostraListaProfili(display, true,              // non dovrebbe accadere 
                        ptrProfiloAttuale->nome,
                        "  --",
                        ptrProfiloAttuale->next->nome);  
    }
    else if (ptrProfiloAttuale->next == NULL)   // ptrProfiloAttuale punta all'ultimo profilo
    {                                           // può accadere costantemente
        mostraListaProfili(display, true,
                        ptrProfiloAttuale->nome,
                        ptrProfiloAttuale->pre->nome,
                        "  --");
    }
    else
    {
        mostraListaProfili(display, true,
                            ptrProfiloAttuale->nome,
                            ptrProfiloAttuale->pre->nome,
                            ptrProfiloAttuale->next->nome);
    }
}

void runProfilo(uint8_t pompa, void * ptrProfilo_) {
    
    /* ------------------SETUP------------------ */

    const uint64_t        ticksNumeratore = 60 * 1000000;           // ticks = (10^6 * 60) / (samples * BPM);
    const profilo_t *     ptrProfilo = (profilo_t*) ptrProfilo_;    // cast necessario perché sono scarso
    enum casoStato        {statoBPM, statoGain, statoOffset, stop, tutto};

    /* Init variabili */
    bool            stopState = true;           // ? stop di default
    uint8_t         BPM = 60;                   // ? range: 40, 100
    float           gain = 1;                   // ? range: 0.1, 10
    float           offset = 0;                 // ? range: -126, 127
    uint8_t         outputValue = 0;
    uint16_t        sample = 0;
    uint64_t        ticks;
    uint64_t        ticksDenominatore = (uint64_t) ptrProfilo->size * (uint64_t) BPM;
    uint8_t         selezioneSottomenu = 0;

    /* Init output timer */
    ticks = ticksNumeratore / ticksDenominatore;
    
    if (!outputTimerEnable(ticks))
        {
            Serial.println("ERRORE in outputTimerEnable in runProfilo");
            return;
        }

    /* Aggiorna il display, mostra il menu e la forma d'onda */
    display.fillScreen(WROVER_BLACK);
    mostraNomeProfilo(display, ptrProfilo->nome);
    mostraStatusProfilo(display, tutto, false, BPM, gain, offset, stopState);
    
    selezioneSottomenu = statoBPM;
    mostraStatusProfilo(display, selezioneSottomenu, false, BPM, gain, offset, stopState);

    mostraWaveformProfilo(display); // TODO

    resetFlagsUI();

    
    /* ------------------LOOP------------------ */

    while(button2Pressed == false) {

        /* OUTPUT TIMER CALLBACK */
        if (outputTimerFlag)
        {
            outputValue  = (uint8_t) (((float)ptrProfilo->campioni[sample++] * gain) + offset);
            dacWrite(DAC, outputValue);
            if(sample >= ptrProfilo->size)
            {
                sample = 0;
            }

            #ifndef NDEBUG
            outputValue = outputValue * 100 / 256;
            for (uint8_t i = 0; i < outputValue; i++)
            {
                Serial.print(" ");
            }
            Serial.print("* \n");
            #endif
        }

        /* scorrimento del sottomenu */
        if ((uiTimerFlag) && (posizioneEncoder != fermo))
        {
            if ((posizioneEncoder == sinistra) && (selezioneSottomenu > statoBPM))
            {
                selezioneSottomenu--;
            }
            else if ((posizioneEncoder == destra) && (selezioneSottomenu < stop))
            {
                selezioneSottomenu++;
            }
            
            mostraStatusProfilo(display, selezioneSottomenu, false, BPM, gain, offset, stopState);
            posizioneEncoder = fermo;
        }

        if (button1Pressed)
        {
            button1Pressed = false;
            mostraStatusProfilo(display, selezioneSottomenu, true, BPM, gain, offset, stopState);
            // TODO
        }



    }

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