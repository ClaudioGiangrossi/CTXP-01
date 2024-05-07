#CTXP-01 OVERVIEW
Questa repository contiene il firmware e documentazione hardware per un controllore per pompe peristaltiche, basato su microcontrollore ESP32 e l'Arduino framework di PlatformIO.

#FUNZIONALITÀ DI BASE
1. POMPA: selezione fra i modelli di pompe disponibili.
2. STEADY: genera un segnale di tensione costante. Il sottomenù velocità permette di regolare la velocità di default.
3. PROFILO: permette di riprodurre e controllare un profilo di velocità presente nel database interno.
   > [!IMPORTANT]
   > Il database interno si può aggiornare solo riprogrammando il microcontrollore attualmente.
4.  WIFI: TODO

#COME AGGIUNGERE UN PROFILO AL DATABASE
Per convenienza viene spiegato come aggiungere un profilo partendo da un vettore in Matlab.

1. Avviare lo [script](https://github.com/signo-codebase/CTXP-01/blob/main/PW_database/PW_to_C_Array.m) per la generazione della struttura dati. Questo script accetta un vettore colonna
   e genera un file di testo che contiene una struttura **profilo_t** da inserire nel codice. La sequenza di campioni è generata partendo dal vettore iniziale, per poi essere normalizzata
   rispetto al massimo valore e adattata alla dinamica di 8 bit del DAC.
2. Copiare dal file *C_Array.txt* la struttura e incollarla nella [sezione profili] (https://github.com/signo-codebase/CTXP-01/blob/main/src/navigazione/profili.cpp) del file *profili.cpp*
3. Sempre in *profili.cpp* __aggiungere il puntatore__ all'interno della funzione __caricaProfiliBase(profilo_t *database)__ seguendo lo schema:
   > esito &= appendProfilo(database, &<nuovoProfilo>);
   >[!WARNING]
   > Se si dimentica questo passaggio non viene caricato il profilo
4. Fare una nuova build e upload del codice
