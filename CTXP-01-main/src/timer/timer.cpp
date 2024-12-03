#include "timer.h"
#include "var_globali.h"
#include "pin_defs.h"

/* TIMER DEFINITIONS */
hw_timer_t *outputTimer = NULL;
hw_timer_t *uiTimer = NULL;
hw_timer_t *encoderTimer = NULL;

/**
 * timer_ticks = source clock / prescaler
 * By default, the source clock is APB clock running at 80 MHz (from timer.h)
 * timer_ticks è la velocità dell'incremento del contatore dei ticks
 * timerAlarmWrite() genera un interrupt dopo N ticks
 **/

uint16_t preScaler = 80;           // allora timer_ticks = 1MHz, T_interrupt = ticks * us
uint64_t uiTicks = 123000;          // T_interrupt = 123 ms
//uint64_t encoderTicks = 70;     // T_interrupt = 0.07 ms
uint64_t encoderTicksFra = 100;     // T_interrupt = 500 us
dac_channel_t dac = DAC_CHANNEL_2; // DAC PIN 26
uint8_t *ptrBuffer = NULL;
uint16_t bufferSize = 0;
uint16_t indexBuffer = 0;

volatile bool outputTimerFlag = false;
volatile bool uiTimerFlag = false;
//  volatile bool wait = false;              // Fra
// volatile uint8_t buffer_PB1[3] = {0};     // Fra
// volatile uint8_t buffer_PB2[3] = {0};     // Fra
volatile uint8_t        Encoder_Av[3] = {0};
volatile uint8_t        Encoder_Bv[3] = {0};
volatile uint8_t        Encoder_State = 0;
volatile unsigned int   Encoder_Timer = 0;

bool uiTimerEnable()
{
    uiTimer = timerBegin(0, preScaler, true);                   // Count UP @ 80 MHz. Ogni 1 us genera un Timer_Tick
    timerAttachInterrupt(uiTimer, &uiRead, true);               // Aggancio alla ISR per la lettura dei pulsanti, fronte di salita
    timerAlarmWrite(uiTimer, uiTicks, true);                    // Genera l'interrupt ogni 123 ms
    timerAlarmEnable(uiTimer);                                  // Abilita il timer

    encoderTimer = timerBegin(1, preScaler, true);              // Count UP @ 80 MHz. Ogni 1 us genera un Timer_Tick
//    timerAttachInterrupt(encoderTimer, &readEncoder, true);     // Aggancio alla ISR di lettura encoder, fronte di salita
    timerAttachInterrupt(encoderTimer, &readEncoderFra, true);     // Aggancio alla ISR di lettura encoder, fronte di salita
//    timerAlarmWrite(encoderTimer, encoderTicks, true);          // Genera un interrupt ogni 70 us
    timerAlarmWrite(encoderTimer, encoderTicksFra, true);          // Genera un interrupt ogni 500 us
    timerAlarmEnable(encoderTimer);                             // Abilita l'interrupt

    return true;
}

bool uiTimerDisable()
{
    timerDetachInterrupt(uiTimer);
    timerAlarmDisable(uiTimer);
    timerEnd(uiTimer);

    timerDetachInterrupt(encoderTimer);
    timerAlarmDisable(encoderTimer);
    timerEnd(encoderTimer);

    return true;
}

bool outputTimerEnable(uint64_t ticks)
{
    outputTimer = timerBegin(2, preScaler, true);
    timerAttachInterrupt(outputTimer, &outputCampione, true);
    timerAlarmWrite(outputTimer, ticks, true);
    timerAlarmEnable(outputTimer);
    return true;
}

bool outputTimerDisable()
{
    timerDetachInterrupt(outputTimer);
    timerAlarmDisable(outputTimer);
    timerEnd(outputTimer);
    return true;
}

/* =========================== INTERRUPT SERVICE ROUTINES =========================== */

void IRAM_ATTR uiRead()
{
    if (!digitalRead(pushButton1))
    {
        button1Pressed = true;
    }

    if (!digitalRead(pushButton2))
    {
        button2Pressed = true;
    }
}


//--------------------------------------------------------------------------------
//      Codice non usato
//--------------------------------------------------------------------------------
/*
void IRAM_ATTR uiRead2()
{
    if (!button1Pressed)            // lettura PB1
    {
        buffer_PB1[2] = buffer_PB1[1];
        buffer_PB1[1] = buffer_PB1[0];
        buffer_PB1[0] = !digitalRead(pushButton1);

        if ((buffer_PB1[2] + buffer_PB1[1] + buffer_PB1[0]) > 1)
        {
            button1Pressed = true;
        }
    }
    if (!button2Pressed)                // lettura PB2
    {
        buffer_PB2[2] = buffer_PB2[1];
        buffer_PB2[1] = buffer_PB2[0];
        buffer_PB2[0] = !digitalRead(pushButton2);

        if ((buffer_PB2[2] + buffer_PB2[1] + buffer_PB2[0]) > 1)
        {
            button2Pressed = true;
        }
    }
}
*/

//--------------------------------------------------------------------------------
//          Nuova driver Encoder (Fra)
//--------------------------------------------------------------------------------
void IRAM_ATTR readEncoderFra()
{
    uint8_t Ar, Br, Vr;

    Encoder_Av[2] = Encoder_Av[1];
    Encoder_Bv[2] = Encoder_Bv[1];
    Encoder_Av[1] = Encoder_Av[0];
    Encoder_Bv[1] = Encoder_Bv[0];
    Encoder_Av[0] = (digitalRead(encoderPinA)) & 1;
    Encoder_Bv[0] = (digitalRead(encoderPinB)) & 1;
    Encoder_Timer++;

    if ((Encoder_Av[2] + Encoder_Av[1] + Encoder_Av[0]) > 1)    Ar = 1;
    else                                                        Ar = 0;
    if ((Encoder_Bv[2] + Encoder_Bv[1] + Encoder_Bv[0]) > 1)    Br = 1;
    else                                                        Br = 0;

    Vr = (Ar << 1) | Br;

    switch (Encoder_State) { 
        case 0:
            if      (Vr == 1)    Encoder_State = 1;
            else if (Vr == 2)    Encoder_State = 4;
            else                 Encoder_State = 0;
            break;

        case 1:
            if (Vr == 1)    break;
            if (Vr == 0)    Encoder_State = 2;
            else            Encoder_State = 7;
            break;
        case 2:
            if (Vr == 0)    break;
            if (Vr == 2)    Encoder_State = 3;
            else            Encoder_State = 7;
            break;
        case 3:
            if (Vr == 2)    break;
            if (Vr == 3) {  posizioneEncoder = sinistra;
                            Encoder_State = 0; }
            else            Encoder_State = 7;
            break;

        case 4:
            if (Vr == 2)    break;
            if (Vr == 0)    Encoder_State = 5;
            else            Encoder_State = 7;
            break;
        case 5:
            if (Vr == 0)    break;
            if (Vr == 1)    Encoder_State = 6;
            else            Encoder_State = 7;
            break;
        case 6:
            if (Vr == 1)    break;
            if (Vr == 3) {  posizioneEncoder = destra;
                            Encoder_State = 0; }
            else            Encoder_State = 7;
            break;
        case 7:             // Error
        default:
            Encoder_State = 0;
            break;
    }
}



/* F
void IRAM_ATTR readEncoder()
{
    if (gpio_get_level((gpio_num_t) encoderPinA) && gpio_get_level((gpio_num_t) encoderPinB))
    {
        wait = false;
    }
}

void IRAM_ATTR readEncoderA()
{
    if (wait == false)
    {
        if (gpio_get_level((gpio_num_t) encoderPinB) == HIGH)
        {
            if (gpio_get_level((gpio_num_t) encoderPinA) == LOW) // controllo ridondante per robustezza
            {
                posizioneEncoder = sinistra;
            }
        }

        wait = true;
    }
}

void IRAM_ATTR readEncoderB()
{
    if (wait == false)
    {
        if (gpio_get_level((gpio_num_t) encoderPinA) == HIGH)
        {
            if (gpio_get_level((gpio_num_t) encoderPinB) == LOW) // controllo ridondante per robustezza
                posizioneEncoder = destra;
        }

        wait = true;
    }
}
*/


void IRAM_ATTR outputCampione()
{
    dac_output_voltage(dac, *(ptrBuffer + indexBuffer));
    if ((++indexBuffer) >= bufferSize)
    {
        indexBuffer = 0;
    }
}