#include "navigazione.h"
#include "profili.h"
#include "memoria/memoria.h"
#include "grafica/grafica.h"
#include "driver/driver_pompe.h"
#include "timer/timer.h"
#include "var_globali.h"
#include "pin_defs.h"

//-------------------------------------------------------
//              WiFi
//-------------------------------------------------------
#include <WiFi.h>
extern WiFiServer server; 
unsigned long currentTime = millis();       // Current time
unsigned long previousTime = 0;             // Previous time
const long timeoutTime = 2000;  // Define timeout time in milliseconds (example: 2000ms = 2s)
String header = "";                  // Variable to store the HTTP request
String output26State = "off";       // Auxiliar variables to store the current output state
String output27State = "off";
const int output26 = 26;            // Assign output variables to GPIO pins
const int output27 = 27;


WROVER_KIT_LCD      display;

/* DICHIARAZIONE VARIABILI DI NAVIGAZIONE*/
volatile bool       button1Pressed = false;
volatile bool       button2Pressed = false;
volatile uint8_t    posizioneEncoder = fermo;


//-------------------------------------------------------
//              FUNZIONI WEB
//-------------------------------------------------------
void VisualizzaLaPaginaHTML (WiFiClient client, String output26State, String output27State) 
{
    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #555555;}</style></head>");
            // Web Page Heading
    client.println("<body><h1>Controllore pompa - Web Server</h1>");
            // Display current state, and ON/OFF buttons for GPIO 26  
    client.println("<p>GPIO 26 - State " + output26State + "</p>");
            // If the output26State is off, it displays the ON button       
    if (output26State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
    } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
    } 
            // Display current state, and ON/OFF buttons for GPIO 27  
    client.println("<p>GPIO 27 - State " + output27State + "</p>");
            // If the output27State is off, it displays the ON button       
    if (output27State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
    } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
    }
    client.println("</body></html>");
            
            // The HTTP response ends with another blank line
    client.println();
}

//-------------------------------------------------------
//              FUNZIONI DI NAVIGAZIONE
//-------------------------------------------------------
//int ComputeDeltaIncrement (int State) {
int ComputeDeltaIncrement (void) {
    static unsigned int TimeOld;
    int                 TimeNew, DeltaTime;
    int                 k;

    TimeNew = Encoder_Timer;
//    if (State)
//        TimeOld = TimeNew;

    DeltaTime = TimeNew - TimeOld;
    TimeOld = TimeNew;
    k = 1000 / DeltaTime;
    if (k < 1)  k = 1;
    if (k > 10) k = 10;
    return (k);
 }

void resetFlagsUI() {
    button1Pressed = false;
    button2Pressed = false;
    posizioneEncoder = fermo;
}

void schermataIniziale() {
    display.begin();
    display.setRotation(3); // landscape, 3 = USB bottom right, 1 = USB upper left
    mostraSchermataIniziale(display);
}

bool init_DAC() {
    dac_output_enable(dac);
    dac_output_voltage(dac, 0);
    return true;
}

uint8_t selezioneMenu() {

    mostraMenu(display);
    delay(80);
    resetFlagsUI();

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
        delay(100);

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
    delay(100);
    resetFlagsUI();

    do
    {
        if (posizioneEncoder == sinistra)
        {
            selezione_pompa = piccola;
            mostraSelezionePompa(display, selezione_pompa);
            delay(80);
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

        delay(50);

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

    delay(100);
    resetFlagsUI();
    return menu;

}

uint8_t menuSteady() {

    enum        opzioni {run, velocita, rotazione};
    uint8_t     selezione = run;
    uint8_t     pompa = pompaSelezionata;
    mostraSteady(display, selezione, true);
    delay(150);
    resetFlagsUI();

    do {
        if (posizioneEncoder == sinistra) {
            if (selezione > run) {
            selezione--;
            mostraSteady(display, selezione, false);
            }
        }
        else if (posizioneEncoder == destra) {
            if (selezione < rotazione) {
            selezione++;
            mostraSteady(display, selezione, false);
            }
        }

        posizioneEncoder = fermo;
        delay(100);

    } while ((button1Pressed == false) && (button2Pressed == false));

    if(button2Pressed) {
        button2Pressed = false;
        return menu;
    }

    if (button1Pressed) {
        button1Pressed = false;

        if (selezione == run) {
            delay(80);
            runSteady(pompa);

        } else if (selezione == velocita) {
            delay(80);
            uint8_t velocitaScelta = selezionaVelocitaDefault(velocitaDefault);
            scriviVelocitaDefault(velocitaScelta);
            velocitaDefault = velocitaScelta;

        } else if (selezione == rotazione) {
            delay(120);
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

    uint64_t        lastRotellina = millis();
    uint64_t        tempo_refresh = 500 + (1500 * velocita / 255);
    bool            giro = false;

    setPinRotazione();
    dac_output_voltage(dac, velocita);
    digitalWrite(STOP_OUT, stopState);

    mostraRunSteady(display, velocita);
    Serial.println("Valori validi di velocità: da 0 a 255.");

    /* icona del senso di rotazione */
    giraRotellina(display, true);

    delay(80);
    resetFlagsUI();

    while (button2Pressed == false) {
        if (posizioneEncoder != fermo) {                 // modifica della velocita

            if ((posizioneEncoder == destra) && (velocita < 255)) {
                dac_output_voltage(dac, ++velocita);
            } else if (posizioneEncoder == sinistra && (velocita > 0)) {
                dac_output_voltage(dac, --velocita);
            }

            posizioneEncoder = fermo;
            aggiornaVelocitaMostrata(display, velocita);
        }
        
        if (millis() - lastSerialRequest > serialRequestTime) {      // posso anche cambiare la velocità da seriale 
        
            if (Serial.available() > 0) {
                uint8_t datoRicevuto = Serial.parseInt();

                if ((datoRicevuto > 0) && (datoRicevuto < 256)) {
                    Serial.println(datoRicevuto);
                    velocita = datoRicevuto;
                    dac_output_voltage(dac, velocita);
                    aggiornaVelocitaMostrata(display, velocita);
                }
            }
        }

        if (button1Pressed == true) {                 // cambia stato, RUN oppure STOP
       
            if (digitalRead(pushButton1) == HIGH) {  // controllo se ho già rilasciato il pushbutton
                button1Pressed = false;
                stopState = !stopState;
                digitalWrite(STOP_OUT, stopState);
                aggiornaStatoSteady(display, stopState);
            }
        }

        /* icona del senso di rotazione */
        if ( (stopState == true) && ((millis() - lastRotellina) > tempo_refresh)) {
            giraRotellina(display, giro);
            giro = !giro;
            tempo_refresh = 150 + (1500 * (255 - velocita) / 255);
            lastRotellina = millis();
        }
    }

    if (button2Pressed == true) {
        dac_output_voltage(dac, 0);
    }
}

uint8_t selezionaVelocitaDefault(uint8_t velocitaAttuale) {

    int velocitaSelezionata = velocitaAttuale;
    int             k;

    resetFlagsUI();                         // resetta l'interfaccia utente (pulsanti e encoder)
    mostraVelocitaSelezionata(display, velocitaSelezionata, true);

//    ComputeDeltaIncrement (1);

    while ((button1Pressed == false) && (button2Pressed == false))  {
        if (posizioneEncoder != fermo) {
            k = ComputeDeltaIncrement ();

            if (posizioneEncoder == destra) {
                    velocitaSelezionata += k;

            } else if (posizioneEncoder == sinistra) {
                    velocitaSelezionata -= k;
            }

            posizioneEncoder = fermo;
            if (velocitaSelezionata < 0) velocitaSelezionata = 0;
            if (velocitaSelezionata > 255) velocitaSelezionata = 255;
            mostraVelocitaSelezionata(display, (uint8_t)velocitaSelezionata, false);

            #ifndef NDEBUG
            Serial.printf("\nVelocita Selezionata: %u", velocitaSelezionata);
            #endif
        }
    }

    if (button2Pressed) {
        button2Pressed = false;
        delay(80);
    }
 
    if (button1Pressed) {
        button1Pressed = false;
        delay(100);
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

    resetFlagsUI();
    delay(300);

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
        delay(100);
        return true;
    }

    delay(100);
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

        if (selezione == sceltaProfilo)
        {
            delay(80);
            if (selezioneProfilo((void*) ptrDatabase))
            {
                return profilo;
            }
            else
            {
                Serial.println("[ERRORE] in selezioneProfilo");
                return menu;
            }
        }
        else if (selezione == run)
        {
            profilo_t *profiloSelezionato = cercaProfilo(ptrDatabase, ultimoProfiloCaricato);
            delay(200);
            runProfilo(pompa, (void*) profiloSelezionato);
            mostraMenu(display);
        }
        else if (selezione == rotazione)
        {
            delay(120);
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

    delay(200);
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

            posizioneEncoder = fermo;
            delay(125);

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

        delay(80);
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

    const uint64_t        ticksNumeratore = 60 * 1000000;           // ticks = (60 * 10^6) / (samples * BPM);
    profilo_t *           ptrProfilo = (profilo_t*) ptrProfilo_;    // cast necessario perché sono scarso
    enum casoStato        {statoBPM, statoGain, statoOffset, fase, stop, tutto};

    /* Init variabili */
    bool            stopState = true;           
    bool            mostraFase = true;          
    uint8_t         BPM = 60;                   // range: 10, 240
    float           gain = 1;                   // range: 0.1, 5
    float           offset = 0;                 // range: -126, 127
    float           outputValue = 0;
    uint8_t         outputValue_8bit = 0;
    uint16_t        sample = 0;
    uint8_t         selezioneSottomenu = 0;
    uint64_t        timerWaveform = (105 - BPM) * 0.66f;
    uint64_t        ultimaWaveform;
    uint16_t        waveformStep;
    uint16_t        stepIncremento = 7;
    uint64_t        ticks;
    uint64_t        ticksDenominatore = ((uint64_t) ptrProfilo->size) * ((uint64_t) BPM);

    /* alloca buffer dei campioni in output */
    if (setupBuffer(ptrProfilo, gain, offset) == false)
    {
        return;
    }

    /* imposta la rotazione a seconda della pompa */
    setPinRotazione();

    // TODO stop default alto? (quindi niente stop)

    /* Aggiorna il display, mostra il menu e la forma d'onda */
    display.fillScreen(WROVER_BLACK);
    mostraNomeProfilo(display, ptrProfilo->nome);
    mostraStatusProfilo(display, tutto, false, BPM, gain, offset, stopState);
    selezioneSottomenu = statoBPM;
    mostraStatusProfilo(display, selezioneSottomenu, false, BPM, gain, offset, stopState);
    
    /* waveform */
    mostraWaveformProfilo(display, 0, mostraFase);
    ultimaWaveform = millis();


    /* Init output timer */
    ticks = ticksNumeratore / ticksDenominatore;
    
    if (!outputTimerEnable(ticks))
    {
        Serial.println("ERRORE in outputTimerEnable in runProfilo");
        return;
    }
    
    resetFlagsUI();
    
    /* ------------------LOOP------------------ */

    while(button2Pressed == false) 
    {
        if ((millis() - ultimaWaveform) > timerWaveform)
        {
            mostraWaveformProfilo(display, waveformStep, mostraFase);
            
            waveformStep = waveformStep + stepIncremento;
            if (waveformStep > 279)   // ho fatto il giro
                waveformStep = 0;
            
            ultimaWaveform = millis();
        }

        /* selezione dello status da editare */
        if (posizioneEncoder != fermo)
        {
            if ((posizioneEncoder == sinistra) && (selezioneSottomenu > statoBPM))
            {
                selezioneSottomenu--;
            }
            else if ((posizioneEncoder == destra) && (selezioneSottomenu < fase))
            {
                selezioneSottomenu++;
            }

            mostraStatusProfilo(display, selezioneSottomenu, false, BPM, gain, offset, stopState);

            if(selezioneSottomenu == fase)
                mostraStatoFase(display, mostraFase, false);
            else
                mostraStatoFase(display, mostraFase, true);

            posizioneEncoder = fermo;
            delay(80);
        }

        /* menu di edit dello status */
        if (button1Pressed)
        {
            mostraStatusProfilo(display, selezioneSottomenu, true, BPM, gain, offset, stopState);
            delay(150);
            resetFlagsUI();

            uint8_t BPM_modifica = BPM;
            float gain_modifica = gain;
            float offset_modifica = offset;

            switch (selezioneSottomenu)
            {
                case statoBPM:
                    while (!button1Pressed && !button2Pressed)
                    {
                        if (posizioneEncoder == sinistra)
                        {
                            if (BPM_modifica > 10)
                                --BPM_modifica;
                        }
                        else if (posizioneEncoder == destra)
                        {
                            if (BPM_modifica < 240)
                                ++BPM_modifica;
                        }
                        posizioneEncoder = fermo;
                        mostraStatusProfilo(display, selezioneSottomenu, true, BPM_modifica, gain, offset, stopState);
                        delay(80);
                    }

                    if (button2Pressed)
                        break;

                    /* se sono nel range, modifico BPM */
                    if (button1Pressed)
                    {
                        if ((BPM_modifica > 9) && (BPM_modifica < 241)) // ? controllo ridondante
                            BPM = BPM_modifica;
                    }

                    break;

                case statoGain:
                    while (!button1Pressed && !button2Pressed)
                    {
                        if (posizioneEncoder == sinistra)
                        {
                            if ((gain_modifica > 0.1) && (gain_modifica <= 2))
                                gain_modifica -= 0.1;
                            else if ((gain_modifica > 2) && (gain_modifica <= 5))
                                gain_modifica -= 0.5;
                        }
                        else if (posizioneEncoder == destra)
                        {
                            if ((gain_modifica < 2) && (gain_modifica > 0) )
                                gain_modifica += 0.1;
                            else if ((gain_modifica >= 2) && (gain_modifica < 5))
                                gain_modifica += 0.5;
                        }

                        posizioneEncoder = fermo;
                        mostraStatusProfilo(display, selezioneSottomenu, true, BPM, gain_modifica, offset, stopState);
                        delay(80);
                    }

                    if (button2Pressed)
                        break;

                    /* se sono nel range, modifico gain */
                    if (button1Pressed)
                    {
                        if ((gain_modifica > 0) && (gain_modifica <= 10)) // ? controllo ridondante
                            gain = gain_modifica;
                    }

                    break;

                case statoOffset:
                    while (!button1Pressed && !button2Pressed)
                    {
                        if (posizioneEncoder == sinistra)
                        {
                            if (offset_modifica > -126)
                                offset_modifica--;
                        }
                        else if (posizioneEncoder == destra)
                        {
                            if (offset_modifica < 127)
                                offset_modifica++;
                        }

                        posizioneEncoder = fermo;
                        mostraStatusProfilo(display, selezioneSottomenu, true, BPM, gain, offset_modifica, stopState);
                        delay(80);
                    }

                    if (button2Pressed)
                        break;

                    /* se sono nel range, modifico offset */
                    if (button1Pressed)
                    {
                        if ((offset_modifica >= -126) && (offset_modifica <= 127)) // ? controllo ridondante
                            offset = offset_modifica; 
                    }

                    break;

                case fase:
                    mostraFase = !mostraFase;
                    selezioneSottomenu = statoBPM;
                    break;
                default:
                    break;

            }

            /* reset output frequency, buffer e valori di aggiornamneto della waveform */
            if (button1Pressed)
            {
                outputTimerDisable();
                ticksDenominatore = ((uint64_t) ptrProfilo->size) * ((uint64_t) BPM);
                ticks = ticksNumeratore / ticksDenominatore;
                setupBuffer(ptrProfilo, gain, offset);
                outputTimerEnable(ticks);
                timerWaveform = impostaIntervalloWaveform(BPM);
                stepIncremento = impostaStepWaveform(BPM);
                Serial.print("Timer: ");
                Serial.println(timerWaveform);
                Serial.print("Step: ");
                Serial.println(stepIncremento);
                Serial.println();
            }
            
            delay(250);
            resetFlagsUI();
            mostraStatusProfilo(display, selezioneSottomenu, false, BPM, gain, offset, stopState);
            mostraStatoFase(display, mostraFase, true);
            mostraWaveformProfilo(display, waveformStep, mostraFase);
        }
    }

    /* ------------------ FINE LOOP------------------ */

    outputTimerDisable();
    dac_output_voltage(dac, 0);

    return;
}

uint64_t impostaIntervalloWaveform(uint8_t BPM) {
    
    if (BPM > 90)
        return 10;
    else if (BPM < 30)
        return 50;
    else
        return ((105 - BPM) * 0.66f);

}

uint16_t impostaStepWaveform(uint8_t BPM) {
    
    if (BPM < 35)
        return 1;
    else if (BPM < 54)
        return 3;
    else if (BPM < 74)
        return 7;
    else if (BPM < 85)
        return 9;
    else
        return 11;

}

uint8_t menuWiFi() {

    mostraWiFi(display);
    resetFlagsUI();

    while (button2Pressed == false) {
        // TODO
    }

    return menu;
}

uint8_t menuWiFiFra() {

  String str = WiFi.localIP().toString();
  
  mostraWiFiFra (display, str);

  while (button2Pressed == false) {
    WiFiClient client = server.available();   // Listen for incoming clients

    if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        header += c;
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();


            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {        // Cerca la stringa in header
              Serial.println("GPIO 26 on");
              output26State = "on";
//F              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
             output26State = "off";
//F              digitalWrite(output26, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              output27State = "on";
//F              digitalWrite(output27, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
//F              digitalWrite(output27, LOW);
            }
            
            VisualizzaLaPaginaHTML (client, output26State, output27State);
            break;                      // Break out of the while loop

          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  }
      return menu;
}