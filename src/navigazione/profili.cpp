#include "profili.h"
#include "var_globali.h"

/* ELEMENTI DEL DATABASE */

profilo_t sinusoide = {
    "Sinusoide",
    1000,
    NULL,
    NULL,
    {127,128,129,129,130,131,132,133,133,134,135,136,137,137,138,139,140,141,141,142,143,144,144,145,146,147,148,148,149,150,151,152,152,153,154,155,155,156,157,158,159,159,160,161,162,162,163,164,165,165,166,167,168,169,169,170,171,172,172,173,174,174,175,176,177,177,178,179,180,180,181,182,183,183,184,185,185,186,187,187,188,189,190,190,191,192,192,193,194,194,195,196,196,197,198,198,199,200,200,201,202,202,203,204,204,205,205,206,207,207,208,209,209,210,210,211,212,212,213,213,214,215,215,216,216,217,217,218,218,219,220,220,221,221,222,222,223,223,224,224,225,225,226,226,227,227,228,228,229,229,230,230,231,231,232,232,232,233,233,234,234,235,235,235,236,236,237,237,238,238,238,239,239,239,240,240,241,241,241,242,242,242,243,243,243,244,244,244,244,245,245,245,246,246,246,246,247,247,247,248,248,248,248,249,249,249,249,249,250,250,250,250,250,251,251,251,251,251,251,252,252,252,252,252,252,252,253,253,253,253,253,253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,253,253,253,253,253,253,253,253,253,253,252,252,252,252,252,252,252,251,251,251,251,251,251,250,250,250,250,250,249,249,249,249,249,248,248,248,248,247,247,247,246,246,246,246,245,245,245,244,244,244,244,243,243,243,242,242,242,241,241,241,240,240,239,239,239,238,238,238,237,237,236,236,235,235,235,234,234,233,233,232,232,232,231,231,230,230,229,229,228,228,227,227,226,226,225,225,224,224,223,223,222,222,221,221,220,220,219,218,218,217,217,216,216,215,215,214,213,213,212,212,211,210,210,209,209,208,207,207,206,205,205,204,204,203,202,202,201,200,200,199,198,198,197,196,196,195,194,194,193,192,192,191,190,190,189,188,187,187,186,185,185,184,183,183,182,181,180,180,179,178,177,177,176,175,174,174,173,172,172,171,170,169,169,168,167,166,165,165,164,163,162,162,161,160,159,159,158,157,156,155,155,154,153,152,152,151,150,149,148,148,147,146,145,144,144,143,142,141,141,140,139,138,137,137,136,135,134,133,133,132,131,130,129,129,128,127,126,125,125,124,123,122,121,121,120,119,118,117,117,116,115,114,113,113,112,111,110,110,109,108,107,106,106,105,104,103,102,102,101,100,99,99,98,97,96,95,95,94,93,92,92,91,90,89,89,88,87,86,85,85,84,83,82,82,81,80,80,79,78,77,77,76,75,74,74,73,72,71,71,70,69,69,68,67,67,66,65,64,64,63,62,62,61,60,60,59,58,58,57,56,56,55,54,54,53,52,52,51,50,50,49,49,48,47,47,46,45,45,44,44,43,42,42,41,41,40,39,39,38,38,37,37,36,36,35,34,34,33,33,32,32,31,31,30,30,29,29,28,28,27,27,26,26,25,25,24,24,23,23,22,22,22,21,21,20,20,19,19,19,18,18,17,17,16,16,16,15,15,15,14,14,13,13,13,12,12,12,11,11,11,10,10,10,10,9,9,9,8,8,8,8,7,7,7,6,6,6,6,5,5,5,5,5,4,4,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,7,7,7,8,8,8,8,9,9,9,10,10,10,10,11,11,11,12,12,12,13,13,13,14,14,15,15,15,16,16,16,17,17,18,18,19,19,19,20,20,21,21,22,22,22,23,23,24,24,25,25,26,26,27,27,28,28,29,29,30,30,31,31,32,32,33,33,34,34,35,36,36,37,37,38,38,39,39,40,41,41,42,42,43,44,44,45,45,46,47,47,48,49,49,50,50,51,52,52,53,54,54,55,56,56,57,58,58,59,60,60,61,62,62,63,64,64,65,66,67,67,68,69,69,70,71,71,72,73,74,74,75,76,77,77,78,79,80,80,81,82,82,83,84,85,85,86,87,88,89,89,90,91,92,92,93,94,95,95,96,97,98,99,99,100,101,102,102,103,104,105,106,106,107,108,109,110,110,111,112,113,113,114,115,116,117,117,118,119,120,121,121,122,123,124,125,125,126,}
    /* la frequenza è 2Hz */
};

profilo_t rampa = {
    "Rampa",
    256,
    NULL,
    NULL,
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255}
    /* la frequenza è 4Hz*/
};

profilo_t test = {
        "Onda quadra",
        10,
        NULL,
        NULL,
        {0, 0, 0, 0, 0, 255, 255, 255, 255, 255}
    };

profilo_t test2 = {
        "15Caratteri_888",
        13,
        NULL,
        NULL,
        {125, 125, 125, 125, 125, 0, 0, 0, 0, 0, 0, 0, 0}
    };

/* IMPORTANTE! SE AGGIUNGI PROFILI QUI SOPRA, RICORDA DI MODIFICARE LA FUNZIONE */
/* caricaProfiliBase() PER AGGIUNGERLI AL DATABASE INIZIALE */


/* FUNZIONI DI GESTIONE DELLA LISTA DEI PROFILI */

bool appendProfilo(profilo_t *database, profilo_t *nuovoProfilo) {
    
    profilo_t *ptr = database;

    if (ptr->pre != NULL)
    {
        Serial.println("ERROR: non è stato passato il puntatore alla lista");
        return false;
    }

    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    nuovoProfilo->pre = ptr;
    nuovoProfilo->next = NULL;    // sarebbe già NULL di default
    ptr->next = nuovoProfilo;

    return true;
    
}

bool cancellaProfilo(profilo_t *database, profilo_t daCancellare) {

    profilo_t *ptr = database;

    if (ptr->pre != NULL)
    {
        Serial.println("ERROR: non è stato passato il puntatore alla lista");
        return false;
    }

    String target = daCancellare.nome;

    while ((ptr->next)->nome != target)
    {
        ptr = ptr->next;
    }

    // TODO finire

    return false;
}

bool caricaProfiliBase(profilo_t *database) {

    bool esito = true;
    esito &= appendProfilo(database, &sinusoide);
    esito &= appendProfilo(database, &rampa);
    esito &= appendProfilo(database, &test);
    esito &= appendProfilo(database, &test2);

    // esito &= appendProfilo(database, &<nuovoProfilo>);
    // append per ogni profilo

    return esito;   // se tutti gli append vanno a buon fine esito è true
}

profilo_t *caricaDatabase(profilo_t *ptrDatabase) {
    if (caricaProfiliBase(ptrDatabase) == true)
    {
        // TODO if(caricaProfiliUtente(ptrDatabase) == true)
        Serial.println("Database caricato correttamente");
        return ptrDatabase;
    }
    else
    {
        Serial.println("ERRORE in caricaDatabaseProfili()");
        return NULL;
    }
}

void visualizzaDatabase(profilo_t *ptrDatabase) {

    profilo_t *ptrNavigazione = ptrDatabase; // ? necessario

    if (ptrNavigazione->pre != NULL)
    {
        Serial.println("Non è stato passato il puntatore alla lista");
    }

    while (ptrNavigazione != NULL)
    {
        Serial.print(ptrNavigazione->nome);
        Serial.printf(", size: %u", ptrNavigazione->size);
        Serial.println();
        ptrNavigazione = ptrNavigazione->next;
    }
    
    Serial.println();
    
}

profilo_t *cercaProfilo(profilo_t *ptrNavigazione, String nomeProfilo) {

    if (ptrNavigazione->pre != NULL)
    {
        Serial.println("Non è stato passato il puntatore alla lista");
        return NULL;
    }
    
    while (ptrNavigazione != NULL)
    {
        if (ptrNavigazione->nome == nomeProfilo)
        {
            return ptrNavigazione;
        }

        ptrNavigazione = ptrNavigazione->next;
        
    }

    return NULL;
    
}

bool setupBuffer(profilo_t * ptrProfilo, float gain, float offset) {

    float outputValue[ptrProfilo->size] = {0};
    uint8_t *output_8bit = (uint8_t*) malloc(ptrProfilo->size);

    if(output_8bit == NULL)
    {
        Serial.println("[ERRORE] in setupBuffer: allocazione non riuscita");
        return false;
    }

    for (uint16_t i = 0; i < ptrProfilo->size; i++)
    {
        /* calcolo valore con gain e offset */
        outputValue[i]  = (((float)ptrProfilo->campioni[i]) * gain) + offset;

        /* impongo che outputValue rientri nella dinamica a 8bit del DAC*/
        if (outputValue[i] < 0)
            {
                outputValue[i] = 0;
            }
        else if (outputValue[i] > 255)
            {
               outputValue[i] = 255;
            }

        /* copio i valori sul buffer globale, castando a uint8_t */
        *(output_8bit + i) = (uint8_t) outputValue[i];
        
        // Serial.printf("%f, %u\n", outputValue[i], *(output_8bit + i));
    }

    ptrBuffer = output_8bit;
    bufferSize = ptrProfilo->size;
    indexBuffer = 0;
    
    return true;
}
