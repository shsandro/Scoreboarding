#include "clock.h"
#include "registers.h"

void init_clock(){
    clock = 0;
    finish_clock = 0;
}

void increase_clock(){
    ++clock;
}

void end_clock(){
    finish_clock = 1;
}