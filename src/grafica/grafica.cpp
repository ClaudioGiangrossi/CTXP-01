#include "var_globali.h"
#include "grafica.h"
#include "startImage.h"
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBoldOblique18pt7b.h>

#define MAX_X 320
#define MAX_Y 240

#define WHITE 0xFFFF
#define BLACK 0x0000
#define CELESTINO 0x555
#define RED 0XF800
#define GREY WROVER_DARKGREY



uint16_t        x_boxDestro = 122;
uint16_t        y_boxDestro = 14;
uint16_t        w_boxDestro = 185;
uint16_t        h_boxDestro = 164;
GFXcanvas16     canvasBoxDestro(w_boxDestro, h_boxDestro);

void mostraSchermataIniziale(WROVER_KIT_LCD d){   //short for display
    
    d.drawBitmap(0, 0, STARTIMAGE_WIDTH, STARTIMAGE_HEIGHT, startImage);

    int16_t cursore_X = 0;
    int16_t cursore_Y = 200;
    int16_t X1 = 80;
    int16_t X2 = 237;

    for (uint16_t i = 0; i < X1 + 1; i++)
    {
        d.drawPixel(i, cursore_Y - 1, WHITE);
        d.drawPixel(i, cursore_Y, WHITE);
        d.drawPixel(i, cursore_Y + 1, WHITE);
        delay(5);
    }

    d.drawPixel(X1, cursore_Y + 2, WHITE);
    //d.drawPixel(X1, cursore_Y + 3, WHITE);
    
    for (uint16_t i = X1; i < X2; i++)
    {
        d.drawPixel(i, cursore_Y - 2 + round( (30 * sin((double) i/(2*PI))) ), WHITE);
        d.drawPixel(i, cursore_Y - 1 + round( (30 * sin((double) i/(2*PI))) ), WHITE);
        d.drawPixel(i, cursore_Y     + round( (30 * sin((double) i/(2*PI))) ), WHITE);
        d.drawPixel(i, cursore_Y + 1 + round( (30 * sin((double) i/(2*PI))) ), WHITE);
        d.drawPixel(i, cursore_Y + 2 + round( (30 * sin((double) i/(2*PI))) ), WHITE);
        delay(5);
    }

    for (uint16_t i = X2 - 1; i < MAX_X; i++)
    {
        d.drawPixel(i, cursore_Y - 1, WHITE);
        d.drawPixel(i, cursore_Y, WHITE);
        d.drawPixel(i, cursore_Y + 1, WHITE);
        delay(5);
    }
    

    delay(2000);
}

void mostraMenu(WROVER_KIT_LCD d){

    d.fillScreen(WROVER_BLACK);

    // Box celestino inferiore
    d.fillRect(0, 190, MAX_X, 50, CELESTINO);
    d.drawLine(0, 191, MAX_X, 191, WHITE);
    d.drawLine(0, 190, MAX_X, 190, WHITE);

    // Scritte box celestino
    int16_t cursore_X = 12;
    int16_t cursore_Y = 220;

    d.setFont(&FreeSansBold9pt7b);
    d.setTextColor(WHITE);
    d.setTextWrap(false);
    d.setCursor(cursore_X, cursore_Y);
    d.print("POMPA");
    d.setCursor(cursore_X + 79, cursore_Y); // 13 è lo spazio
    d.print("STEADY");
    d.setCursor(cursore_X + 79 + 83, cursore_Y);
    d.print("PROFILO");
    d.setCursor(cursore_X + 79 + 83 + 91, cursore_Y);
    d.print("WIFI");


    // Box sinistra
    cursore_X = 10;
    cursore_Y = 30;

    d.drawRect(5, 5, 100, 180, WHITE);
    d.drawLine(6, 183, 103, 183, WHITE);
    d.drawLine(6, 185, 103, 185, 0xBDF7);

    d.setFont(&FreeSans9pt7b);
    d.setTextColor(GREY);
    d.setCursor(cursore_X, cursore_Y);
    d.print("Pompa:");
    d.setTextColor(WHITE);
    d.setCursor(cursore_X + 10, cursore_Y + 25);
    printPompa(d, pompaSelezionata);

    d.setCursor(cursore_X, cursore_Y + 50);
    d.setTextColor(GREY);
    d.print("Rotazione:");
    d.setCursor(cursore_X + 10, cursore_Y + 75);
    d.setTextColor(WHITE);
    printRotazione(d, pompaSelezionata, rotazione);

    // Box destra
    d.drawRect(114, 5, 201, 180, WHITE); // rettangolo più esterno
    d.drawRect(116, 6, 198, 178, WHITE);
    d.drawRect(117, 7, 196, 176, 0xD6BA);
    d.drawRect(117, 8, 195, 174, 0xAD55);
    d.drawRect(118, 9, 193, 172, 0x94B2);
    d.drawRect(119, 10, 191, 170, 0x6B4D);
    d.drawRect(120, 11, 189, 168, 0x52AA); // rettangolo più interno
    d.drawLine(115, 183, 115, 6, WHITE);
    d.drawLine(116, 7, 116, 182, 0xD6BA);
    d.drawLine(103, 182, 103, 6, WHITE);
    d.drawLine(105, 183, 105, 5, 0xBDF7);

}

void muoviCursoreMenu(WROVER_KIT_LCD d, uint8_t selezione) {

    int16_t     cursore_X = 12;
    int16_t     cursore_Y = 220;
    int16_t     boundary_X, boundary_Y;
    uint16_t    w, h;

    d.setCursor(cursore_X, cursore_Y);
    d.setFont(&FreeSansBold9pt7b);
    
    d.fillRect(0, 190, MAX_X, 50, CELESTINO);
    d.drawLine(0, 191, MAX_X, 191, WHITE);
    d.drawLine(0, 190, MAX_X, 190, WHITE);

    d.setTextColor(WHITE);
    d.setTextWrap(false);
    d.setCursor(cursore_X, cursore_Y);
    d.print("POMPA");
    d.setCursor(cursore_X + 79, cursore_Y); // 13 è lo spazio
    d.print("STEADY");
    d.setCursor(cursore_X + 79 + 83, cursore_Y);
    d.print("PROFILO");
    d.setCursor(cursore_X + 79 + 83 + 91, cursore_Y);
    d.print("WIFI");

    switch (selezione)
    {
    case pompa:
        d.getTextBounds("POMPA", cursore_X, cursore_Y, 
                        &boundary_X, &boundary_Y, &w, &h);
        d.fillRoundRect(boundary_X - 5, boundary_Y - 5, w + 10, h + 10, 5, BLACK);
        d.setCursor(cursore_X, cursore_Y);
        d.print("POMPA");
        break;

    case steady:
        d.getTextBounds("STEADY", cursore_X + 79, cursore_Y, 
                        &boundary_X, &boundary_Y, &w, &h);
        d.fillRoundRect(boundary_X - 5, boundary_Y - 5, w + 10, h + 10, 5, BLACK);
        d.setCursor(cursore_X + 79, cursore_Y);
        d.print("STEADY");
        break;

    case profilo:
        d.getTextBounds("PROFILO", cursore_X + 79 + 83, cursore_Y, 
                        &boundary_X, &boundary_Y, &w, &h);
        d.fillRoundRect(boundary_X - 5, boundary_Y - 5, w + 10, h + 10, 5, BLACK);
        d.setCursor(cursore_X + 79 + 83, cursore_Y);
        d.print("PROFILO");
        break;

    case wifi:
        d.getTextBounds("WIFI", cursore_X + 79 + 83 + 91, cursore_Y,
                        &boundary_X, &boundary_Y, &w, &h);
        d.fillRoundRect(boundary_X - 5, boundary_Y - 5, w + 10, h + 10, 5, BLACK);
        d.setCursor(cursore_X + 79 + 83 + 91, cursore_Y);
        d.print("WIFI");
        break; 

    default:
          #ifndef NDEBUG
          Serial.println("ERRORE: selezione ha assunto un valore illegale");
          #endif
          esp_restart();
        break;
    }
}

void evidenziaSelezioneMenu(WROVER_KIT_LCD d, uint8_t selezione) {

    int16_t     cursore_X = 12;
    int16_t     cursore_Y = 220;

    d.setFont(&FreeSansBold9pt7b);

    switch (selezione)
    {
    case pompa:
        d.setCursor(cursore_X, cursore_Y);
        d.setTextColor(RED);
        d.print("POMPA");
        delay(100);
        d.setCursor(cursore_X, cursore_Y);
        d.setTextColor(WHITE);
        d.print("POMPA");
        break;

    case steady:
        d.setCursor(cursore_X + 79, cursore_Y);
        d.setTextColor(RED);
        d.print("STEADY");
        delay(100);
        d.setCursor(cursore_X + 79, cursore_Y);
        d.setTextColor(WHITE);
        d.print("STEADY");
        break;

    case profilo:
        d.setCursor(cursore_X + 79 + 83, cursore_Y);
        d.setTextColor(RED);
        d.print("PROFILO");
        delay(100);
        d.setCursor(cursore_X + 79 + 83, cursore_Y);
        d.setTextColor(WHITE);
        d.print("PROFILO");
        break;

    case wifi:
        d.setCursor(cursore_X + 79 + 83 + 91, cursore_Y);
        d.setTextColor(RED);
        d.print("WIFI");
        delay(100);
        d.setCursor(cursore_X + 79 + 83 + 91, cursore_Y);
        d.setTextColor(WHITE);
        d.print("WIFI");
        break; 

    default:
          #ifndef NDEBUG
          Serial.println("ERRORE: selezione ha assunto un valore illegale");
          #endif
          esp_restart();
        break;
    }
}

void mostraSelezionePompa(WROVER_KIT_LCD d, uint8_t pompaDaEvidenziare) {
    int16_t    X = 135;
    int16_t    Y1 = 50;
    int16_t    Y2 = 125;

    switch (pompaDaEvidenziare)
    {
    case piccola:
        disegnaCursore(d, X, Y1, WHITE);
        d.setCursor(X + 30, Y1 + 15);
        d.setTextColor(WHITE);
        d.setFont(&FreeSans18pt7b);
        d.print("120U");

        disegnaCursore(d, X, Y2 - 7, BLACK);
        d.setCursor(X + 30, Y2 + 15);
        d.setTextColor(GREY);
        d.setFont(&FreeSans18pt7b);
        d.print("PMD24C");

        break;
    case grande:
        disegnaCursore(d, X, Y1, BLACK);
        d.setCursor(X + 30, Y1 + 15);
        d.setTextColor(GREY);
        d.setFont(&FreeSans18pt7b);
        d.print("120U");

        disegnaCursore(d, X, Y2 - 7, WHITE);
        d.setCursor(X + 30, Y2 + 15);
        d.setTextColor(WHITE);
        d.setFont(&FreeSans18pt7b);
        d.print("PMD24C");

    default:
        break;
    }
}

void mostraSteady(WROVER_KIT_LCD d, uint8_t selezione, bool clearScreen) {
    
    enum        opzioni {run, velocita, rotazione};

    int16_t    X = 135;
    int16_t    Y1 = 30;
    int16_t    Y2 = 85;
    int16_t    Y3 = 140;

    if (clearScreen)
    {
        d.fillRect(125, 15, 180, 155, BLACK); // clear box a destra
    }

    d.setFont(&FreeSans12pt7b);
    d.setTextColor(GREY);
    d.setCursor(X + 25, Y1 + 16);
    d.print("AVVIA");
    d.setCursor(X + 25, Y2 + 16);
    d.print("VELOCITA'");
    d.setCursor(X + 25, Y3 + 16);
    d.print("ROTAZIONE");

    switch (selezione)
    {
    case run:
        disegnaCursorePiccolo(d, X, Y1, WHITE);
        disegnaCursorePiccolo(d, X, Y2, BLACK);
        disegnaCursorePiccolo(d, X, Y3, BLACK);

        d.setTextColor(WHITE);
        d.setCursor(X + 25, Y1 + 16);
        d.print("AVVIA");
        break;
    case velocita:
        disegnaCursorePiccolo(d, X, Y1, BLACK);
        disegnaCursorePiccolo(d, X, Y2, WHITE);
        disegnaCursorePiccolo(d, X, Y3, BLACK);
        d.setTextColor(WHITE);
        d.setCursor(X + 25, Y2 + 16);
        d.print("VELOCITA'");
        break;
    case rotazione:
        disegnaCursorePiccolo(d, X, Y1, BLACK);
        disegnaCursorePiccolo(d, X, Y2, BLACK);
        disegnaCursorePiccolo(d, X, Y3, WHITE);
        d.setTextColor(WHITE);
        d.setCursor(X + 25, Y3 + 16);
        d.print("ROTAZIONE");
        break;
    default:
        break;
    }

}

void mostraRunSteady(WROVER_KIT_LCD d, uint8_t velocita) {

    d.fillScreen(BLACK);
    d.fillRoundRect(10, 10, 165, 40, 10, CELESTINO);
    d.setTextColor(BLACK);
    d.setFont(&FreeSansBold18pt7b);
    d.setCursor(22, 42);
    d.print("STEADY");

    d.setCursor(102, 90);
    d.setFont(&FreeSans12pt7b);
    d.setTextColor(GREY);
    d.print("VELOCITA'");

    aggiornaVelocitaMostrata(d, velocita);

    aggiornaStatoSteady(d, 1);

}

void mostraVelocitaSelezionata(WROVER_KIT_LCD d, uint8_t velocita, bool clearScreen) {
    /* l'uso di clearScreen è per cancellare le scritte precedenti sul display
    tenendo conto di quando è stata effettuata la chiamata di questa funzione */

    int16_t    X = 135;
    int16_t    Y1 = 30;
    int16_t    Y2 = 85;
    int16_t    Y3 = 140;

    if (clearScreen)
    {
        d.setFont(&FreeSans12pt7b);
        d.setTextColor(BLACK);
        d.setCursor(X + 25, Y1 + 16);
        d.print("AVVIA");
        d.setCursor(X + 25, Y3 + 16);
        d.print("ROTAZIONE");
        d.setCursor(X + 25, Y2 + 16);
        d.setTextColor(GREY);
        d.print("VELOCITA'");
        disegnaCursorePiccolo(d, X, Y2, BLACK);
        disegnaCursorePiccolo(d, X + 25, Y2 + 43, WHITE);
    }


    d.fillRect(X + 50, Y2 + 35, 100, 35, BLACK);
    
    d.setFont(&FreeSansBold18pt7b);
    d.setCursor(X + 50, Y2 + 61);
    d.setTextColor(WHITE);
    d.print(velocita * 100 / 255);
    d.print("%");

}

void aggiornaVelocitaMostrata(WROVER_KIT_LCD d, uint8_t velocita) {
    
    GFXcanvas16     canvasVelocita(200, 85);

    canvasVelocita.setFont(&FreeSansBold18pt7b);
    canvasVelocita.fillScreen(BLACK);
    canvasVelocita.setTextSize(2);
    canvasVelocita.setCursor(5, 60);
    canvasVelocita.setTextColor(WHITE);
    canvasVelocita.print(velocita * 100 / 255);
    canvasVelocita.print("%");

    d.drawBitmap(95, 91, canvasVelocita.width(), canvasVelocita.height(), 
                    canvasVelocita.getBuffer());
}

void aggiornaStatoSteady(WROVER_KIT_LCD d, uint8_t stato) {
    enum _stato {stop, run};
    
    d.setFont(&FreeSans12pt7b);

    switch (stato)
    {
    case run:
        d.drawRect(40, 190, 100, 30, RED);
        d.setTextColor(RED);
        d.setCursor(65, 213);
        d.print("RUN");

        d.drawRect(180, 190, 100, 30, BLACK);
        d.setTextColor(GREY);
        d.setCursor(200, 213);
        d.print("STOP");
        break;

    case stop:
        d.drawRect(40, 190, 100, 30, BLACK);
        d.setTextColor(GREY);
        d.setCursor(65, 213);
        d.print("RUN");

        d.drawRect(180, 190, 100, 30, RED);
        d.setTextColor(RED);
        d.setCursor(200, 213);
        d.print("STOP");
        break;

    default:
        break;
    }
}

void giraRotellina(WROVER_KIT_LCD d, bool giro) {
    GFXcanvas16 c(38, 38);
    
    if (pompaSelezionata == piccola)
    {
        if (rotazione == 0)
        {
            if (giro == false)
            {
                c.drawBitmap(0, 0, rotellina_oraria, 38, 32, WHITE);
            }
            else
            {
                c.drawBitmap(0, 0, rotellina_oraria_giro, 32, 38, WHITE);
            }
        }
        else
        {
            if (giro == false)
            {
                c.drawBitmap(0, 0, rotellina_antior, 38, 32, WHITE);
            }
            else
            {
                c.drawBitmap(0, 0, rotellina_antior_giro, 32, 38, WHITE);
            }
        }
    }
    else    // pompa grande
    {
        if (rotazione == 1)
        {
            if (giro == false)
            {
                c.drawBitmap(0, 0, rotellina_oraria, 38, 32, WHITE);
            }
            else
            {
                c.drawBitmap(0, 0, rotellina_oraria_giro, 32, 38, WHITE);
            }
        }
        else
        {
           if (giro == false)
            {
                c.drawBitmap(0, 0, rotellina_antior, 38, 32, WHITE);
            }
            else
            {
                c.drawBitmap(0, 0, rotellina_antior_giro, 32, 38, WHITE);
            } 
        }
    }

    /* fine tutte le casistiche, adesso disegno sul display */
    d.drawBitmap(250, 17, c.width(), c.height(), c.getBuffer());

}

void mostraSelezioneRotazione(WROVER_KIT_LCD d, uint8_t rotazioneSelezionata, bool clearScreen) {

    const uint8_t       rotazioneAntioraria = 0;
    const uint8_t       rotazioneOraria = 1;
    int16_t             X = 130;
    int16_t             Y1 = 50;
    int16_t             Y2 = 125;

    if (clearScreen)
    {
        d.fillRect(125, 15, 180, 155, BLACK); // clear box a destra
    }

    switch (rotazioneSelezionata)
    {
    case rotazioneAntioraria:
        disegnaCursorePiccolo(d, X, Y1, WHITE);
        d.setCursor(X + 25, Y1 + 11);
        d.setTextColor(WHITE);
        d.setFont(&FreeSans12pt7b);
        d.print("ANTIORARIA");

        disegnaCursorePiccolo(d, X, Y2 - 7, BLACK);
        d.setCursor(X + 25, Y2 + 11);
        d.setTextColor(GREY);
        d.setFont(&FreeSans12pt7b);
        d.print("ORARIA");
        break;

    case rotazioneOraria:
        disegnaCursorePiccolo(d, X, Y1, BLACK);
        d.setCursor(X + 25, Y1 + 11);
        d.setTextColor(GREY);
        d.setFont(&FreeSans12pt7b);
        d.print("ANTIORARIA");

        disegnaCursorePiccolo(d, X, Y2 - 7, WHITE);
        d.setCursor(X + 25, Y2 + 11);
        d.setTextColor(WHITE);
        d.setFont(&FreeSans12pt7b);
        d.print("ORARIA");

    default:
        break;
    }
}

void mostraProfilo(WROVER_KIT_LCD d, uint8_t selezione, bool clearScreen){
    
    enum        opzioni {run, sceltaProfilo, rotazione};

    int16_t    X = 135;
    int16_t    Y1 = 30;
    int16_t    Y2 = 85;
    int16_t    Y3 = 140;

    if (clearScreen)
    {
        d.fillRect(125, 15, 180, 155, BLACK); // clear box a destra
    }

    d.setFont(&FreeSans12pt7b);
    d.setTextColor(GREY);
    d.setCursor(X + 25, Y1 + 16);
    d.print("AVVIA");
    d.setCursor(X + 25, Y2 + 16);
    d.print("PROFILO");
    d.setCursor(X + 25, Y3 + 16);
    d.print("ROTAZIONE");

    switch (selezione)
    {
    case run:
        disegnaCursorePiccolo(d, X, Y1, WHITE);
        disegnaCursorePiccolo(d, X, Y2, BLACK);
        disegnaCursorePiccolo(d, X, Y3, BLACK);

        d.setTextColor(WHITE);
        d.setCursor(X + 25, Y1 + 16);
        d.print("AVVIA");
        break;
    case sceltaProfilo:
        disegnaCursorePiccolo(d, X, Y1, BLACK);
        disegnaCursorePiccolo(d, X, Y2, WHITE);
        disegnaCursorePiccolo(d, X, Y3, BLACK);
        d.setTextColor(WHITE);
        d.setCursor(X + 25, Y2 + 16);
        d.print("PROFILO");
        break;
    case rotazione:
        disegnaCursorePiccolo(d, X, Y1, BLACK);
        disegnaCursorePiccolo(d, X, Y2, BLACK);
        disegnaCursorePiccolo(d, X, Y3, WHITE);
        d.setTextColor(WHITE);
        d.setCursor(X + 25, Y3 + 16);
        d.print("ROTAZIONE");
        break;
    default:
        break;
    }

}

void mostraWiFi(WROVER_KIT_LCD d){
    d.fillScreen(WROVER_BLACK);
    d.setCursor(150, 100);
    d.println("WIFI");
}

void printPompa(WROVER_KIT_LCD d, uint8_t Pompa) {

    int16_t x, y;
    uint16_t w, h;

    d.getTextBounds("PMD24C", d.getCursorX(), d.getCursorY(), &x, &y, &w, &h);
    d.fillRect(x, y, (int16_t) w, (int16_t) h, BLACK);

    if (Pompa == piccola)
    {
        d.print("120U");
    }
    else if (Pompa == grande)
    {
        d.print("PMD24C");
    }
}

void printRotazione(WROVER_KIT_LCD d, uint8_t Pompa, uint8_t Rotazione) {

    int16_t x, y;
    uint16_t w, h;
    
    int16_t cursore_X = 10;
    int16_t cursore_Y = 30;

    d.setFont(&FreeSans9pt7b);
    d.setTextColor(BLACK);
    d.setCursor(cursore_X + 10, cursore_Y + 75);
    d.print("ORARIA");
    d.setCursor(cursore_X + 10, cursore_Y + 75);
    d.print("  ANTI");

    d.setTextColor(WHITE);

    if (Pompa == piccola)
    {
        if (Rotazione == 0)
        {
            d.setCursor(cursore_X + 10, cursore_Y + 75);
            d.print("ORARIA");
        }
        else if (Rotazione == 1)
        {
            d.setCursor(cursore_X + 10, cursore_Y + 75);
            d.print("  ANTI");
        }
    }
    else if (Pompa == grande)
    {
        if (Rotazione == 0)
        {
            d.setCursor(cursore_X + 10, cursore_Y + 75);
            d.print("  ANTI");
        }
        else if (Rotazione == 1)
        {
            d.setCursor(cursore_X + 10, cursore_Y + 75);
            d.print("ORARIA");
        }
    }
}

void disegnaCursore(WROVER_KIT_LCD d, int16_t X, int16_t Y, uint16_t colore) {
    d.drawLine(0 + X, 0 + Y, X + 0, Y + 22, colore);
    d.drawLine(1 + X, 0 + Y, X + 1, Y + 22, colore);
    d.drawLine(1 + X, 0 + Y, X + 22, Y + 11, colore);
    d.drawLine(1 + X, 22 + Y, X +  20, Y + 12, colore);
    d.drawLine(3 + X, 2 + Y, X + 3, Y + 20, colore);
    d.drawLine(4 + X, 3 + Y, X + 4, Y + 20, colore);
    d.drawLine(5 + X, 3 + Y, X + 5, Y + 19, colore);
    d.drawLine(6 + X, 4 + Y, X + 6, Y + 18, colore);
    d.drawLine(7 + X, 4 + Y, X + 7, Y + 19, colore);
    d.drawLine(8 + X, 5 + Y, X + 8, Y + 17, colore);
    d.drawLine(9 + X, 6 + Y, X + 9, Y + 17, colore);
    d.drawLine(2 + X, 2 + Y, X + 2, Y + 20, colore);
    d.drawLine(X + 10, Y + 6, X + 10, Y + 16, colore);
    d.drawLine(X + 11, Y + 7, X + 11, Y + 16, colore);
    d.drawLine(X + 12, Y + 7, X + 12, Y + 16, colore);
    d.drawLine(X + 13, Y + 8, X + 13, Y + 15, colore);
    d.drawLine(X + 14, Y + 9, X + 14, Y + 14, colore);
    d.drawLine(X + 15, Y + 9, X + 15, Y + 15, colore);
    d.drawLine(X + 16, Y + 10, X +  16, Y +  14, colore);
    d.drawLine(X + 17, Y + 10, X +  17, Y +  14, colore);
    d.drawLine(X + 18, Y + 11, X +  18, Y +  12, colore);
    d.drawLine(X + 19, Y + 11, X +  19, Y +  12, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 9, Y +  5, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 11, Y +  6, colore);
    d.drawPixel(X + 9, Y + 11, colore);
    d.drawPixel(X + 13, Y +  7, colore);
    d.drawPixel(X + 14, Y +  8, colore);
    d.drawPixel(X + 15, Y +  8, colore);
    d.drawPixel(X + 16, Y +  9, colore);
    d.drawPixel(X + 17, Y +  9, colore);
    d.drawPixel(X + 18, Y + 10, colore);
    d.drawPixel(X + 19, Y + 10, colore);
    d.drawPixel(X + 20, Y + 11, colore);
    d.drawPixel(X + 21, Y + 11, colore);
    d.drawPixel(X + 21, Y + 12, colore);
}

void disegnaCursorePiccolo (WROVER_KIT_LCD d, int16_t X, int16_t Y, uint16_t colore) {
    d.drawLine(X + 0, Y + 0,  X + 0, Y + 15, colore);
    d.drawLine(X + 0, Y + 0,  X + 13, Y + 6, colore);
    d.drawLine(X + 1, Y + 15, X + 12, Y + 9, colore);
    d.drawPixel(X + 13, Y + 9, colore);
    d.fillRect(X + 1, Y + 6, 11, 4, colore);
    d.drawRect(X + 1, Y + 10, 7, 2, colore);
    d.drawRect(X + 1, Y + 4, 7, 2, colore);
    d.drawLine(X + 1, Y + 3, X + 5, Y + 3, colore);
    d.drawLine(X + 1, Y + 2, X + 3, Y + 2, colore);
    d.drawLine(X + 1, Y + 12, X +  5, Y + 12, colore);
    d.drawLine(X + 1, Y + 13, X +  3, Y + 13, colore);
    d.drawPixel(X + 1, Y + 1, colore);
    d.drawPixel(X + 1, Y + 14, colore);
    d.drawPixel(X + 8, Y + 5, colore);
    d.drawPixel(X + 9, Y + 5, colore);
    d.drawPixel(X + 8, Y + 10, colore);
    d.drawPixel(X + 9, Y + 10, colore);
    d.drawRect(X + 12, Y + 7, 2, 2, colore);
    d.drawPixel(X + 14, Y + 7, colore);
    d.drawPixel(X + 14, Y + 8, colore);
}

void mostraListaProfili(WROVER_KIT_LCD d, bool clearScreen, String attuale, String pre, String next) {

    int16_t y_offset = 20;

    int16_t    X = 135 - x_boxDestro;
    int16_t    Y1 = 40 - y_boxDestro + y_offset;
    int16_t    Y2 = 85 - y_boxDestro + y_offset;
    int16_t    Y3 = 135 - y_boxDestro + y_offset;

    canvasBoxDestro.fillScreen(BLACK);
    canvasBoxDestro.setFont(&FreeSansBold12pt7b);
    canvasBoxDestro.setTextWrap(false);
    canvasBoxDestro.setCursor(X, Y2);
    canvasBoxDestro.setTextColor(WHITE);
    canvasBoxDestro.print(attuale);

    canvasBoxDestro.setFont(&FreeSans12pt7b);
    canvasBoxDestro.setTextColor(GREY);

    canvasBoxDestro.setCursor(X + 20, Y1);
    if (pre.length() > 13)
    {
        canvasBoxDestro.setFont(&FreeSans9pt7b);
        canvasBoxDestro.setCursor(X + 20, Y1 - 10);
    }
    canvasBoxDestro.print(pre);

    canvasBoxDestro.setCursor(X + 20, Y3);
    if (next.length() > 13)
    {
        canvasBoxDestro.setFont(&FreeSans9pt7b);
        canvasBoxDestro.setCursor(X + 20, Y3 - 8);
    }
    canvasBoxDestro.print(next);

    d.drawBitmap(x_boxDestro, y_boxDestro, canvasBoxDestro.width(), canvasBoxDestro.height(), 
                    canvasBoxDestro.getBuffer());

}

void mostraNomeProfilo(WROVER_KIT_LCD d, String nome) {
    int16_t x = 20;
    int16_t y = 32;
    d.setFont(&FreeSansBold18pt7b);
    d.setTextColor(CELESTINO);
    d.setCursor(x, y);
    d.print(nome);
}



void mostraStatusProfilo(WROVER_KIT_LCD d, uint8_t caso, bool edit, 
                         uint8_t BPM, float gain, float offset, bool stopState) {
    
    GFXcanvas16     canvasStatus(200, 110);

    enum casoStato {statoBPM, statoGain, statoOffset, stop, tutto};

    uint16_t    x_offset = 20;
    uint16_t    x_statusInfo = x_offset + 10;

    canvasStatus.setFont(&FreeSans12pt7b);
    canvasStatus.setTextColor(GREY);

    /* display del blocco di stato */
    canvasStatus.setCursor(x_statusInfo, 40);
    canvasStatus.printf("BPM: %u", BPM);
    canvasStatus.setCursor(x_statusInfo, 69);
    canvasStatus.printf("GAIN: %0.1f", gain);
    canvasStatus.setCursor(x_statusInfo, 100);
    canvasStatus.printf("OFFSET: %0.0f", (offset / 127 * 100));
    canvasStatus.print("%");
    // mostraStatusStop(d, statoStop, GREY);


    // evidenzio l'opzione che voglio editare 
    if  (!edit)
    {
        canvasStatus.setTextColor(WHITE);
        switch (caso)
            {
            case stop:
                // mostraStatusStop(d, statoStop, WHITE);
                break;

            case statoBPM:
                canvasStatus.drawBitmap(x_statusInfo - 20, 40 - 14, freccia_spicy, 15, 14, WHITE);
                canvasStatus.setCursor(x_statusInfo, 40);
                canvasStatus.printf("BPM: %u", BPM);
                break;

            case statoGain:
                canvasStatus.drawBitmap(x_statusInfo - 20, 70 - 14, freccia_spicy, 15, 14, WHITE);
                canvasStatus.setCursor(x_statusInfo, 69);
                canvasStatus.printf("GAIN: %0.1f", gain);
                break;

            case statoOffset:
                canvasStatus.drawBitmap(x_statusInfo - 20, 100 - 14, freccia_spicy, 15, 14, WHITE);
                canvasStatus.setCursor(x_statusInfo, 100);
                canvasStatus.printf("OFFSET: %0.0f", (offset / 127 * 100));
                canvasStatus.print("%");
                break;

            default:
                break;
            }
    }

    if(edit)
    {
        canvasStatus.fillScreen(BLACK);
        canvasStatus.setTextColor(RED);
        switch (caso)
            {
            case stop:
                // mostraStatusStop(d, statoStop, RED);
                break;

            case statoBPM:
                canvasStatus.setCursor(x_statusInfo, 40);
                canvasStatus.printf("BPM: %u", BPM);
                break;

            case statoGain:
                canvasStatus.setCursor(x_statusInfo, 70);
                canvasStatus.printf("GAIN: %0.1f", gain);
                break;

            case statoOffset:
                canvasStatus.setCursor(x_statusInfo, 100);
                canvasStatus.printf("OFFSET: %0.0f", (offset / 127 * 100));
                canvasStatus.print("%");
                break;

            default:
                break;
            }
    }

    /* disegno i canvas sul display */
    d.drawBitmap(x_offset, 40, canvasStatus.width(), canvasStatus.height(), canvasStatus.getBuffer());
}

void mostraStatusStop(WROVER_KIT_LCD d, bool stopState, uint16_t colore) {

    GFXcanvas16     canvasStop(100, 25);

    canvasStop.setFont(&FreeSans9pt7b);
    canvasStop.setTextColor(colore);

    canvasStop.setCursor(0, 20);
    if (stopState == true)
    {
        canvasStop.print("  STOP");
    }
    else
    {
        canvasStop.print("RUNNING");
    }

    d.drawBitmap(190, 140, canvasStop.width(), canvasStop.height(), canvasStop.getBuffer());
}

void mostraStatoFase(WROVER_KIT_LCD d, bool mostraFase, bool clear) {
    d.setCursor(250, 137);
    d.setFont(&FreeSansBold9pt7b);

    if (mostraFase)
        d.setTextColor(RED);
    else
        d.setTextColor(WHITE);

    if (clear)
        d.setTextColor(BLACK);
    
    d.print("FASE");
}

void mostraWaveformProfilo(WROVER_KIT_LCD d, uint16_t waveformStep, bool mostraFase) {
    if (mostraFase)
        mostraWaveformFase(d);
    else
        mostraWaveformDinamica(d, waveformStep);
}


void mostraWaveformFase(WROVER_KIT_LCD d) {

    uint16_t    x_offset = 20;
    uint16_t    x_canvas = 282;
    uint16_t    y_canvas = 80;

    GFXcanvas16 canvasWaveform(x_canvas, y_canvas);

    /* disegno linee verticali */
    canvasWaveform.drawFastVLine(0, 0, y_canvas - 2, GREY);
    canvasWaveform.drawFastVLine(1, 0, y_canvas - 2, GREY);
    canvasWaveform.drawFastVLine(x_canvas - 2, 0, y_canvas - 2, GREY);
    canvasWaveform.drawFastVLine(x_canvas - 1, 0, y_canvas - 2, GREY);

    /* disegno linee orizzontali */
    canvasWaveform.drawFastHLine(0, 0, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, 1, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, y_canvas - 2, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, y_canvas - 1, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, y_canvas / 2, x_canvas - 1, GREY);

    int16_t    x_dopo;
    int16_t    y_dopo;
    
    int16_t    x = 0;
    int16_t    y = *(ptrBuffer) * 70 / 256;

    /* disena la forma d'onda */
    for (uint16_t index = 1; index < bufferSize; index++)
    {
        x_dopo = (index * 280 / bufferSize);
        y_dopo = *(ptrBuffer + index) * 70 / 256;   
        
        uint16_t diff_x = x_dopo > x ? x_dopo - x : x - x_dopo; // calcola la distanza fra i due punti
        if (diff_x < 200)
        {
            canvasWaveform.drawLine(x, 70 - y + 3, x_dopo, 70 - y_dopo + 3, WHITE);
            canvasWaveform.drawLine(x, 70 - y + 4, x_dopo, 70 - y_dopo + 4, WHITE);
            canvasWaveform.drawLine(x, 70 - y + 5, x_dopo, 70 - y_dopo + 5, WHITE);
        }

        x = x_dopo;
        y = y_dopo;       
    }

    /* disegna il cursore */
    x = (indexBuffer * 280 / bufferSize);
    y = *(ptrBuffer + indexBuffer) * 70 / 256;
    canvasWaveform.fillCircle(x, 70 - y + 4, 4, RED);
    canvasWaveform.writeFastVLine(x, 0, canvasWaveform.height(), RED);

    d.drawBitmap(x_offset - 1, 154, canvasWaveform.width(), canvasWaveform.height(), canvasWaveform.getBuffer());
}

void mostraWaveformDinamica(WROVER_KIT_LCD d, uint16_t waveformStep) {
    
    uint16_t    x_offset = 20;
    uint16_t    x_canvas = 282;
    uint16_t    y_canvas = 80;

    GFXcanvas16 canvasWaveform(x_canvas, y_canvas);

    /* disegno linee verticali */
    canvasWaveform.drawFastVLine(0, 0, y_canvas - 2, GREY);
    canvasWaveform.drawFastVLine(1, 0, y_canvas - 2, GREY);
    canvasWaveform.drawFastVLine(x_canvas - 2, 0, y_canvas - 2, GREY);
    canvasWaveform.drawFastVLine(x_canvas - 1, 0, y_canvas - 2, GREY);

    /* disegno linee orizzontali */
    canvasWaveform.drawFastHLine(0, 0, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, 1, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, y_canvas - 2, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, y_canvas - 1, x_canvas, GREY);
    canvasWaveform.drawFastHLine(0, y_canvas / 2, x_canvas - 1, GREY);

    int16_t    x_dopo;
    int16_t    y_dopo;
    int16_t    shift;
    uint8_t    direzione; // 0: antioraria, 1: oraria

    if (((pompaSelezionata == piccola) && (rotazione == 1)) ||
        ((pompaSelezionata == grande)  && (rotazione == 0)))
    {
        shift = waveformStep * -1; // antioraria
        direzione = 0;
    }
    else if (((pompaSelezionata == piccola) && (rotazione == 0)) ||
             ((pompaSelezionata == grande)  && (rotazione == 1)))
    {
        shift = waveformStep; // oraria
        direzione = 1;
    }
    
    int16_t    x = 0 + shift;
    int16_t    y = *(ptrBuffer) * 70 / 255;

    if (x < 0)
        x = 280 + x;
    else if (x > 280)
        x = x - 280;

    for (uint16_t index = 1; index < bufferSize; index++)
    {
        x_dopo = (index * 280 / bufferSize) + shift;
        y_dopo = *(ptrBuffer + index) * 70 / 255;   

        /* ci assicuriamo che i pixel che "cadono fuori" facciano il giro del display*/
        if (x_dopo < 0)
            x_dopo = 280 + x_dopo;
        else if (x_dopo > 280)
            x_dopo = x_dopo - 280;
        
        uint16_t diff_x = x_dopo > x ? x_dopo - x : x - x_dopo; // calcola la distanza fra i due punti
        if (diff_x < 200)
        {
            canvasWaveform.drawLine(x, 70 - y + 3, x_dopo, 70 - y_dopo + 3, WHITE);
            canvasWaveform.drawLine(x, 70 - y + 4, x_dopo, 70 - y_dopo + 4, WHITE);
            canvasWaveform.drawLine(x, 70 - y + 5, x_dopo, 70 - y_dopo + 5, WHITE);
        }

        x = x_dopo;
        y = y_dopo;       
    }

    d.drawBitmap(x_offset - 1, 154, canvasWaveform.width(), canvasWaveform.height(), canvasWaveform.getBuffer());
}

