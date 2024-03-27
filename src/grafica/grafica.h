#ifndef GRAFICA_FUNZIONI
#define GRAFICA_FUNZIONI

#include <Arduino.h>
#include <WROVER_KIT_LCD.h>

void mostraSchermataIniziale(WROVER_KIT_LCD);
void mostraMenu(WROVER_KIT_LCD);
void mostraSteady(WROVER_KIT_LCD, uint8_t selezione, bool clearScreen);
void mostraProfilo(WROVER_KIT_LCD, uint8_t selezione, bool clearScreen);
void mostraWiFi(WROVER_KIT_LCD);


void muoviCursoreMenu(WROVER_KIT_LCD, uint8_t selezione);
void evidenziaSelezioneMenu(WROVER_KIT_LCD, uint8_t selezione);


void printPompa(WROVER_KIT_LCD, uint8_t Pompa);
void printRotazione(WROVER_KIT_LCD, uint8_t Pompa, uint8_t Rotazione);


void mostraSelezionePompa(WROVER_KIT_LCD, uint8_t pompaDaEvidenziare);
void mostraVelocitaSelezionata(WROVER_KIT_LCD, uint8_t velocita, bool clearScreen);
void mostraSelezioneRotazione(WROVER_KIT_LCD, uint8_t rotazioneSelezionata, bool clearScreen);


void disegnaCursore(WROVER_KIT_LCD, int16_t X, int16_t Y, uint16_t colore);
void disegnaCursorePiccolo(WROVER_KIT_LCD, int16_t X, int16_t Y, uint16_t colore);

void mostraRunSteady(WROVER_KIT_LCD, uint8_t velocita);
void aggiornaVelocitaMostrata(WROVER_KIT_LCD, uint8_t velocita);
void aggiornaStatoSteady(WROVER_KIT_LCD, uint8_t stato);


void infoWiFi();



#endif