#ifndef NAVIGAZIONE_FUNZIONI
#define NAVIGAZIONE_FUNZIONI

#include <Arduino.h>
#include <WROVER_KIT_LCD.h>

void resetFlagsUI();
void schermataIniziale();
bool init_DAC();
int ComputeDeltaIncrement (int);

uint8_t selezioneMenu();
uint8_t selezionePompa();
uint8_t menuSteady();
uint8_t menuProfilo();
uint8_t menuWiFi();
uint8_t menuWiFiFra();

bool sensoRotazione(uint8_t pompa);
void runSteady(uint8_t pompa);
uint8_t selezionaVelocitaDefault(uint8_t velocitaAttuale);
bool selezioneProfilo(void * ptrDatabase_);
void listaProfili(void * ptrProfiloAttuale_);
void runProfilo(uint8_t pompa, void * ptrProfilo);
uint64_t impostaIntervalloWaveform(uint8_t BPM);
uint16_t impostaStepWaveform(uint8_t BPM);

#endif