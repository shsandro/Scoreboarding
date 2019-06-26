#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "instructions.h"
#include "instructions_queue.h"
#include "registers.h"
#include "memory.h"
#include "sim.h"
#include "./scoreboarding/scoreboarding.h"

void fetch_stage();
void execution_stage();

#endif