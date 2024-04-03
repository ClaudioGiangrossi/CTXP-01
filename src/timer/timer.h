#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

bool uiTimerEnable();
bool uiTimerDisable();
void uiRead();

bool outputTimerEnable(uint64_t ticks);
bool outputTimerDisable();
void outputCampione();

void readEncoderA();
void readEncoderB();

void interruptEncoderA();
void interruptEncoderB();


#endif