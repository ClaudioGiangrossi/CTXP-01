#ifndef NAVIGAZIONE_FUNZIONI
#define NAVIGAZIONE_FUNZIONI

#include <Arduino.h>
#include <WROVER_KIT_LCD.h>

void resetFlagsUI();
void schermataIniziale();

uint8_t selezioneMenu();
uint8_t selezionePompa();
uint8_t menuSteady();
uint8_t menuProfilo();
uint8_t menuWiFi();

bool sensoRotazione(uint8_t pompa);
void runSteady(uint8_t pompa);
uint8_t selezionaVelocitaDefault(uint8_t velocitaAttuale);

bool selezioneProfilo();
void runProfilo(uint8_t pompa);

#endif