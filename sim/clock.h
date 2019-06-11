#ifndef CLOCK_H
#define CLOCK_H
#include <stdio.h>

int clock;
int finish_clock = 0;

void init_clock();
void increase_clock();
void finish_clock();

#endif