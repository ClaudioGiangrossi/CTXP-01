#ifndef VAR_GLOBALI_H
#define VAR_GLOBALI_H

#include <Arduino.h>
#include <Preferences.h>
#include <WROVER_KIT_LCD.h>
#include <assert.h>
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


/* ==================================== TIMERS ==================================== */
/* ================================================================================ */


/* TIMER HANDLES */
extern hw_timer_t   *outputTimer;
extern hw_timer_t   *uiTimer;

/* TIMER VARIABLES */
extern uint16_t     preScaler; // se = 80 allora setta Ton = ticks * us
extern uint64_t     ticks;
extern bool         outputTimerFlag;

#endif