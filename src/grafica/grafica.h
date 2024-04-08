#ifndef GRAFICA_FUNZIONI
#define GRAFICA_FUNZIONI

#include <Arduino.h>
#include <WROVER_KIT_LCD.h>

/* --------------------------------ICONE-------------------------------- */

static const unsigned char PROGMEM rotellina_antior[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x07,0x01,0x80,0x00,0x00,0x18,0x00,0x40,0x00,0x00,0x20,0x7e,0x40,0x00,0x00,0x43,0xff,0xc0,0x00,0x00,0x87,0x81,0x80,0x00,0x00,0x8c,0x00,0x02,0x00,0x01,0x10,0x00,0x05,0x00,0x01,0x10,0x00,0x08,0x80,0x02,0x20,0x00,0x10,0x40,0x02,0x20,0x00,0x20,0x20,0x04,0x40,0x00,0x40,0x10,0x04,0x40,0x00,0x80,0x08,0x04,0x40,0x00,0xf8,0xf8,0x7c,0x7c,0x00,0xf8,0xf8,0x40,0x04,0x00,0x08,0x80,0x60,0x0c,0x00,0x08,0x80,0x70,0x1c,0x00,0x11,0x00,0x38,0x38,0x00,0x11,0x00,0x1c,0x70,0x00,0x22,0x00,0x0e,0xe0,0x00,0x22,0x00,0x07,0xc0,0x00,0xc4,0x00,0x03,0x80,0x00,0x84,0x00,0x00,0x06,0x07,0x08,0x00,0x00,0x09,0xf8,0x10,0x00,0x00,0x08,0x00,0x70,0x00,0x00,0x0e,0x03,0xe0,0x00,0x00,0x07,0xff,0x80,0x00,0x00,0x01,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
// c.drawBitmap(0, 0, rotellina_antior, 38, 32, WHITE);

static const unsigned char PROGMEM rotellina_antior_giro[] = {0x00,0x00,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,0x39,0x00,0x00,0x00,0x71,0x00,0x00,0x00,0xe1,0xe0,0x00,0x01,0xc0,0x18,0x00,0x01,0x80,0x06,0x00,0x01,0xc0,0x01,0x80,0x00,0xe1,0xe0,0x40,0x00,0x71,0x18,0x20,0x00,0x39,0x06,0x10,0x1c,0x1d,0x01,0x10,0x32,0x0f,0x01,0x88,0x32,0x00,0x00,0xc8,0x64,0x00,0x00,0xc8,0x64,0x00,0x00,0xc4,0x64,0x00,0x00,0x64,0x64,0x00,0x00,0x64,0x64,0x00,0x00,0x64,0x64,0x00,0x00,0x64,0x62,0x00,0x00,0x64,0x32,0x00,0x00,0x64,0x32,0x00,0x00,0xc8,0x31,0x81,0xc0,0xc8,0x18,0x81,0xa0,0x70,0x18,0x61,0x90,0x00,0x0c,0x19,0x88,0x00,0x02,0x07,0x84,0x00,0x01,0x80,0x02,0x00,0x00,0x60,0x01,0x00,0x00,0x18,0x02,0x00,0x00,0x07,0x84,0x00,0x00,0x01,0x88,0x00,0x00,0x01,0x90,0x00,0x00,0x01,0xa0,0x00,0x00,0x01,0xc0,0x00,0x00,0x00,0x00,0x00};
// c.drawBitmap(0, 0, rotellina_antior_giro, 32, 38, WHITE);

static const unsigned char PROGMEM rotellina_oraria[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xfc,0x00,0x00,0x00,0x07,0xff,0x80,0x00,0x00,0x0e,0x03,0xe0,0x00,0x00,0x08,0x00,0x70,0x00,0x00,0x09,0xf8,0x10,0x00,0x00,0x06,0x07,0x08,0x00,0x03,0x80,0x00,0x84,0x00,0x07,0xc0,0x00,0xc4,0x00,0x0e,0xe0,0x00,0x22,0x00,0x1c,0x70,0x00,0x22,0x00,0x38,0x38,0x00,0x11,0x00,0x70,0x1c,0x00,0x11,0x00,0x60,0x0c,0x00,0x08,0x80,0x40,0x04,0x00,0x08,0x80,0x7c,0x7c,0x00,0xf8,0xf8,0x04,0x40,0x00,0xf8,0xf8,0x04,0x40,0x00,0x80,0x08,0x04,0x40,0x00,0x40,0x10,0x02,0x20,0x00,0x20,0x20,0x02,0x20,0x00,0x10,0x40,0x01,0x10,0x00,0x08,0x80,0x01,0x10,0x00,0x05,0x00,0x00,0x8c,0x00,0x02,0x00,0x00,0x87,0x81,0x80,0x00,0x00,0x43,0xff,0xc0,0x00,0x00,0x20,0x7e,0x40,0x00,0x00,0x18,0x00,0x40,0x00,0x00,0x07,0x01,0x80,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
// c.drawBitmap(0, 0, rotellina_oraria, 38, 32, WHITE);


static const unsigned char PROGMEM rotellina_oraria_giro[] = {0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x00,0x00,0x00,0xb8,0x00,0x00,0x00,0x9c,0x00,0x00,0x00,0x8e,0x00,0x00,0x07,0x87,0x00,0x00,0x18,0x03,0x80,0x00,0x60,0x01,0x80,0x01,0x80,0x03,0x80,0x02,0x07,0x87,0x00,0x04,0x18,0x8e,0x00,0x08,0x60,0x9c,0x00,0x08,0x80,0xb8,0x38,0x11,0x80,0xf0,0x4c,0x13,0x00,0x00,0x4c,0x13,0x00,0x00,0x26,0x23,0x00,0x00,0x26,0x26,0x00,0x00,0x26,0x26,0x00,0x00,0x26,0x26,0x00,0x00,0x26,0x26,0x00,0x00,0x26,0x26,0x00,0x00,0x46,0x26,0x00,0x00,0x4c,0x13,0x00,0x00,0x4c,0x13,0x03,0x81,0x8c,0x0e,0x05,0x81,0x18,0x00,0x09,0x86,0x18,0x00,0x11,0x98,0x30,0x00,0x21,0xe0,0x40,0x00,0x40,0x01,0x80,0x00,0x80,0x06,0x00,0x00,0x40,0x18,0x00,0x00,0x21,0xe0,0x00,0x00,0x11,0x80,0x00,0x00,0x09,0x80,0x00,0x00,0x05,0x80,0x00,0x00,0x03,0x80,0x00,0x00,0x00,0x00,0x00};
// c.drawBitmap(0, 0, rotellina_oraria_giro, 32, 38, WHITE);



static const unsigned char PROGMEM freccia_spicy[] = {0x01,0x80,0x01,0x40,0x01,0x20,0xff,0x50,0x80,0x68,0xbf,0xf4,0xaa,0xfa,0xb5,0x7a,0xbf,0xf4,0x80,0x68,0xff,0x50,0x01,0x20,0x01,0x40,0x01,0x80};

//static const unsigned char PROGMEM cursore_ganzo[] = {0x7f,0xff,0x20,0x06,0x10,0x1c,0x08,0x78,0x05,0xf0,0x07,0xe0,0x07,0xc0,0x07,0x80,0x07,0x00,0x06,0x00,0x04,0x00};
//c.drawBitmap(0, 0, cursore_ganzo, 16, 11, WHITE);




/* --------------------------------FUNZIONI-------------------------------- */

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
void disegnaCursorePiccoloCanvas(GFXcanvas16, int16_t X, int16_t Y, uint16_t colore);

void mostraRunSteady(WROVER_KIT_LCD, uint8_t velocita);
void aggiornaVelocitaMostrata(WROVER_KIT_LCD, uint8_t velocita);
void aggiornaStatoSteady(WROVER_KIT_LCD, uint8_t stato);
void giraRotellina(WROVER_KIT_LCD, bool giro);

void mostraListaProfili(WROVER_KIT_LCD, bool clearScreen, String, String, String);
void mostraNomeProfilo(WROVER_KIT_LCD, String nome);
void mostraStatusProfilo(WROVER_KIT_LCD, uint8_t caso, bool edit,
                         uint8_t BPM, float gain, float offset, bool stopState);
void mostraStatusStop(WROVER_KIT_LCD, bool stopState, uint16_t colore);
void mostraRiquadroWaveform(WROVER_KIT_LCD);
void mostraWaveformProfilo(WROVER_KIT_LCD, uint16_t waveformStep);

void infoWiFi();



#endif