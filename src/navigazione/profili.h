#ifndef PROFILI_H
#define PROFILI_H

#include <Arduino.h>

/* Versione primitiva per gestire il database di profili */

/* STRUTTURA DI BASE */
typedef struct _profilo_t{
    String      nome;
    uint16_t    size;
    _profilo_t   *pre;           // puntatori per implementazione con lista collegata
    _profilo_t   *next;          // per permettere allocazione dinamica in futuro 
    uint8_t     campioni[];
} profilo_t;


/* FUNZIONI DI GESTIONE DELLA LISTA DEI PROFILI */
bool          appendProfilo(profilo_t *database, profilo_t nuovoProfilo);
bool          cancellaProfilo(profilo_t *database, profilo_t daCancellare); // TODO finire
bool          caricaProfiliBase(profilo_t *database);
bool          caricaProfiliUtente(profilo_t *database); // TODO quelli salvati in memoria, inseriti da seriale
profilo_t*    caricaDatabase(profilo_t*);
profilo_t*    cercaProfilo(profilo_t *ptrNavigazione, String nomeProfilo);
void          visualizzaDatabase(profilo_t*);

#endif