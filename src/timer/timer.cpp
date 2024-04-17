#include "timer.h"
#include "var_globali.h"
#include "pin_defs.h"


/* TIMER DEFINITIONS */
hw_timer_t          *outputTimer = NULL;
hw_timer_t          *uiTimer = NULL;

/** 
 * timer_ticks = source clock / prescaler 
 * By default, the source clock is APB clock running at 80 MHz (from timer.h)
 * timer_ticks è la velocità dell'incremento del contatore dei ticks 
 * timerAlarmWrite() genera un interrupt dopo N ticks 
**/

uint16_t            preScaler = 80;           // allora timer_ticks = 1MHz, T_interrupt = ticks * us
uint64_t            uiTicks = 10000;          // T_interrupt = 10 ms
dac_channel_t       dac = DAC_CHANNEL_2;      // DAC PIN 26
uint8_t*            ptrBuffer = NULL;
uint16_t            bufferSize = 0;
uint16_t            indexBuffer = 0;

volatile bool       outputTimerFlag = false;
volatile bool       uiTimerFlag = false;

bool uiTimerEnable() {
    uiTimer = timerBegin(0, preScaler, true);
    timerAttachInterrupt(uiTimer, &uiRead, true);
    timerAlarmWrite(uiTimer, uiTicks, true);
    timerAlarmEnable(uiTimer);
    return true;
}

bool uiTimerDisable() {
    timerDetachInterrupt(uiTimer);
    timerAlarmDisable(uiTimer);
    timerEnd(uiTimer);
    return true;
}

bool outputTimerEnable(uint64_t ticks) {
    outputTimer = timerBegin(2, preScaler, true);
    timerAttachInterrupt(outputTimer, &outputCampione, true);
    timerAlarmWrite(outputTimer, ticks, true);
    timerAlarmEnable(outputTimer);
    return true;
}

bool outputTimerDisable() {
    timerDetachInterrupt(outputTimer);
    timerAlarmDisable(outputTimer);
    timerEnd(outputTimer);
    return true;
}

/* =========================== INTERRUPT SERVICE ROUTINES =========================== */

void IRAM_ATTR uiRead() {
    if (!digitalRead(pushButton1))
    {
        button1Pressed = true;
    }

    if (!digitalRead(pushButton2))
    {
        button2Pressed = true;
    }

}

void IRAM_ATTR readEncoderA() {
    if (posizioneEncoder == fermo)
    {
        if (digitalRead(encoderPinB) == HIGH)
        {
            if (digitalRead(encoderPinA) == LOW)       // controllo ridondante per robustezza
            {
                    posizioneEncoder = sinistra;
            }
        }
    }
}


void IRAM_ATTR readEncoderB() {
    if (posizioneEncoder == fermo)
    {
        if (digitalRead(encoderPinA) == HIGH)
        {
            if (digitalRead(encoderPinB) == LOW)        // controllo ridondante per robustezza
                posizioneEncoder = destra;
        }
    }
}


void IRAM_ATTR outputCampione() {
    dac_output_voltage(dac, *(ptrBuffer + indexBuffer));
    if ((++indexBuffer) >= bufferSize)
    {
        indexBuffer = 0;
    }
}