#include "memoria.h"
#include "var_globali.h"

/*
   memoria: oggetto per salvare in memoria
   "stato": namespace in uso -> da usare con begin
   "key": associato al valore da salvare -> da usare con get/put
*/

Preferences memoria;

void scriviPompa(uint8_t pompa) {
    memoria.begin("stato", RW_MODE);
    memoria.putUChar("pompa", pompa);
    memoria.end();
}

uint8_t chiediPompa() {
    memoria.begin("stato", RO_MODE);
    uint8_t pompaInMemoria = memoria.getUChar("pompa");
    memoria.end();
    return pompaInMemoria;
}

void scriviProfiliMemorizzati(uint8_t profili_in_memoria) {
    memoria.begin("stato", RW_MODE);
    memoria.putUChar("profiliMEM", profili_in_memoria);
    memoria.end();
}

uint8_t chiediProfiliMemorizzati() {
    memoria.begin("stato", RO_MODE);
    uint8_t profiliTotali = memoria.getUChar("profiliMEM");
    memoria.end();
    return profiliTotali;
}

void scriviProfiloCaricato(String nomeProfilo) {
    memoria.begin("stato", RW_MODE);
    memoria.putString("nomeProfilo", nomeProfilo);
    memoria.end();
}

String chiediProfiloCaricato() {
    memoria.begin("stato", RO_MODE);
    String nomeProfilo = memoria.getString("nomeProfilo");
    memoria.end();
    return nomeProfilo;
}

void scriviVelocitaDefault(uint8_t velocitaDaSalvare) {
    memoria.begin("stato", RW_MODE);
    memoria.putUChar("velDefault", velocitaDaSalvare);
    memoria.end();
}

uint8_t chiediVelocitaDefault() {
    memoria.begin("stato", RO_MODE);
    uint8_t velocitaInMemoria = memoria.getUChar("velDefault");
    memoria.end();
    return velocitaInMemoria;
}

bool registraPompaSelezionata(uint8_t selezione_pompa) {

    if ((selezione_pompa == piccola) || (selezione_pompa == grande))
    {
        scriviPompa(selezione_pompa);
        pompaSelezionata = selezione_pompa;
        return true;
    }
    
    else
    {
        pompaSelezionata = errore;

        #ifndef NDEBUG
        Serial.println("ERRORE: pompa selezionata non valida");
        #endif

        return false;
    }

}

bool salvaProfiloInMemoria() {

    scriviProfiloCaricato(ultimoProfiloCaricato);

    #ifndef NDEBUG
    Serial.println("\nProfilo caricato in memoria");
    #endif

    delay(100);
    return true;
}