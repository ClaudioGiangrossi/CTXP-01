#ifndef MEMORIA_H
#define MEMORIA_H

#include <Arduino.h>
#include <Preferences.h>

#define RW_MODE false
#define RO_MODE true

void scriviPompa(uint8_t pompa);
uint8_t chiediPompa();

/* le funzioni ProfiliMemorizzati saranno utili per permettere il caricamento da web */
void scriviProfiliMemorizzati(uint8_t profili_in_memoria);
uint8_t chiediProfiliMemorizzati();


void scriviProfiloCaricato(String nomeProfilo);
String chiediProfiloCaricato();


bool registraPompaSelezionata(uint8_t selezione_pompa);
bool caricaProfilo();


void scriviVelocitaDefault(uint8_t velocitaDaSalvare);
uint8_t chiediVelocitaDefault();

#endif