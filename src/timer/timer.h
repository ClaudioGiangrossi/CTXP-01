#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

bool uiTimerEnable();
bool uiTimerDisable();
void uiRead();
void uiRead2();

bool outputTimerEnable(uint64_t ticks);
bool outputTimerDisable();
void outputCampione();

void readEncoder();
void readEncoderA();
void readEncoderB();

#endif