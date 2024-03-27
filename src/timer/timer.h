#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

bool uiTimerEnable();
bool uiTimerDisable();
void uiRead();

bool outputTimerEnable();
bool outputTimerDisable();
void outputCampione();

void readEncoderA();
void readEncoderB();

void interruptEncoderA();
void interruptEncoderB();


#endif