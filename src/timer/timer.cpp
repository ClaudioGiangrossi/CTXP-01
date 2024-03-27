#include "timer.h"
#include "var_globali.h"
#include "pin_defs.h"

/* TIMER DEFINITIONS */
hw_timer_t      *outputTimer = NULL;
hw_timer_t      *uiTimer = NULL;
uint16_t        preScaler = 80;      // Ton = ticks * us
uint64_t        outputTicks = 1000;
uint64_t        UIticks = 10000;
uint64_t        ms_100 = 100*1000;
bool            outputTimerFlag = false;



bool uiTimerEnable() {
    uiTimer = timerBegin(0, preScaler, true);
    timerAttachInterrupt(uiTimer, &uiRead, true);
    timerAlarmWrite(uiTimer, UIticks, true);
    timerAlarmEnable(uiTimer);
    return true;
}

bool uiTimerDisable() {
    timerDetachInterrupt(uiTimer);
    timerAlarmDisable(uiTimer);
    timerEnd(uiTimer);
    return true;
}

bool outputTimerEnable() {
    outputTimer = timerBegin(2, preScaler, true);
    timerAttachInterrupt(outputTimer, &outputCampione, true);
    timerAlarmWrite(outputTimer, outputTicks, true);
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
                    posizioneEncoder = destra;
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
                posizioneEncoder = sinistra;
        }
    }
}


void IRAM_ATTR outputCampione() {
    outputTimerFlag = true;
}