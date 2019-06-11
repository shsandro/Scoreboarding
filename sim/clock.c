#include "clock.h"

void init_clock(){
    clock = 0;
}

void increase_clock(){
    clock++;
}

void end_clock(){
    finish_clock = 1;
}