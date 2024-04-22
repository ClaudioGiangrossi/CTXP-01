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
uint64_t uiTicks = 100000;          // T_interrupt = 100 ms
uint64_t encoderTicks = 50000;     // T_interrupt = 50 ms
dac_channel_t dac = DAC_CHANNEL_2; // DAC PIN 26
uint8_t *ptrBuffer = NULL;
uint16_t bufferSize = 0;
uint16_t indexBuffer = 0;

volatile bool outputTimerFlag = false;
volatile bool uiTimerFlag = false;

volatile uint8_t buffer_PB1[3] = {0};
volatile uint8_t buffer_PB2[3] = {0};

volatile uint8_t buffer_encoder_A[5] = {0};
volatile uint8_t buffer_encoder_B[5] = {0};
volatile uint8_t somma_A = 0;
volatile uint8_t somma_B = 0;

bool uiTimerEnable()
{
    uiTimer = timerBegin(0, preScaler, true);
    timerAttachInterrupt(uiTimer, &uiRead, true);
    timerAlarmWrite(uiTimer, uiTicks, true);
    timerAlarmEnable(uiTimer);

    

    return true;
}

bool uiTimerDisable()
{
    timerDetachInterrupt(uiTimer);
    timerAlarmDisable(uiTimer);
    timerEnd(uiTimer);
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

void IRAM_ATTR uiRead2()
{

    /* lettura PB1 */
    if (!button1Pressed)
    {
        buffer_PB1[2] = buffer_PB1[1];
        buffer_PB1[1] = buffer_PB1[0];
        buffer_PB1[0] = !digitalRead(pushButton1);

        if ((buffer_PB1[2] + buffer_PB1[1] + buffer_PB1[0]) > 1)
        {
            button1Pressed = true;
        }
    }

    /* lettura PB2 */
    if (!button2Pressed)
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

void IRAM_ATTR readEncoder()
{

    somma_A = 0;
    somma_B = 0;

    /* scorro la FIFO, sommo 4 letture eccetto quella che butto via */
    for (uint8_t i = 0; i < 4; i++)
    {
        somma_A += buffer_encoder_A[i + 1];
        buffer_encoder_A[i] = buffer_encoder_A[i + 1];

        somma_B += buffer_encoder_B[i + 1];
        buffer_encoder_B[i] = buffer_encoder_B[i + 1];
    }

    /* faccio una nuova lettura, la aggiungo alla somma */
    buffer_encoder_A[5] = !digitalRead(encoderPinA);
    somma_A += buffer_encoder_A[5];

    buffer_encoder_B[5] = !digitalRead(encoderPinB);
    somma_B += buffer_encoder_B[5];

    if (posizioneEncoder == fermo)
    {
        if (somma_A > 2)
        {
            posizioneEncoder = sinistra;
        }
        else if (somma_B > 2)
        {
            posizioneEncoder = destra;
        }
    }
}

void IRAM_ATTR readEncoderA()
{
    if (posizioneEncoder == fermo)
    {
        if (digitalRead(encoderPinB) == HIGH)
        {
            if (digitalRead(encoderPinA) == LOW) // controllo ridondante per robustezza
            {
                posizioneEncoder = sinistra;
            }
        }
    }
}

void IRAM_ATTR readEncoderB()
{
    if (posizioneEncoder == fermo)
    {
        if (digitalRead(encoderPinA) == HIGH)
        {
            if (digitalRead(encoderPinB) == LOW) // controllo ridondante per robustezza
                posizioneEncoder = destra;
        }
    }
}

void IRAM_ATTR outputCampione()
{
    dac_output_voltage(dac, *(ptrBuffer + indexBuffer));
    if ((++indexBuffer) >= bufferSize)
    {
        indexBuffer = 0;
    }
}