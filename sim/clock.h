#ifndef CLOCK_H
#define CLOCK_H

#include <stdio.h>

int clock;
int finish_clock;

void init_clock();
void increase_clock();
void end_clock();
int get_clock();

#endif