#ifndef VAR_GLOBALI_H
#define VAR_GLOBALI_H

#include <Arduino.h>
#include <Preferences.h>
#include <WROVER_KIT_LCD.h>
#include <assert.h>
#include "driver/dac_common.h"
#include "driver/gpio.h"


// #include <WiFi.h>    commentata per non compilarla ogni volta, da sviluppare ancora wifi

// #define NDEBUG


/* ============================= VARIABILI DI STATO ============================== */
/* =============================================================================== */


enum Contesto           {menu, pompa, steady, profilo, wifi};
enum SelezionePompa     {errore, piccola, grande};
    /* PICCOLA -> 120U*/
    /* GRANDE -> PMD24C*/

extern volatile uint8_t     scenaAttuale;
extern uint8_t              pompaSelezionata;
extern uint8_t              rotazione;
    /* PICCOLA -> rotazione antioraria 1, rotazione oraria 0 */
    /* GRANDE -> rotazione antioraria 0, rotazione oraria 1 */
extern String               ultimoProfiloCaricato;
extern uint8_t              profiliMemorizzati;
extern uint8_t              velocitaDefault;

/* ============================= VARIABILI DI INPUT ============================== */
/* =============================================================================== */


enum posizioneEncoder_enum {sinistra, fermo, destra};

extern volatile uint8_t     posizioneEncoder;
extern volatile bool        button1Pressed;
extern volatile bool        button2Pressed;
extern volatile unsigned int Encoder_Timer;

/* ==================================== TIMERS ==================================== */
/* ================================================================================ */


/* TIMER HANDLES */
extern hw_timer_t   *outputTimer;
extern hw_timer_t   *uiTimer;

/* UI TIMER VARIABLES */
extern volatile bool       uiTimerFlag;

/* DAC OUTPUT TIMER VARIABLES */
extern uint16_t            preScaler; // se = 80 allora setta Ton = ticks * us
extern uint64_t            ticks;
extern volatile bool       outputTimerFlag;
extern dac_channel_t       dac;
extern uint8_t*            ptrBuffer;
extern uint16_t            bufferSize;
extern uint16_t            indexBuffer;

#endif