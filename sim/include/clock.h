#ifndef CLOCK_H
#define CLOCK_H

#include <stdio.h>

int clock;
int finish_clock;

void initialize_clock();
void increase_clock();
void end_clock();
int get_clock();

#endif