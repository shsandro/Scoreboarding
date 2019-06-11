#include "clock.h"

int clock;
bool finish_clock = false;

void init_clock(){
    clock = 0;
}

void increase_clock(){
    clock++;
}

void finish_clock(){
    finish_clock = true;
}