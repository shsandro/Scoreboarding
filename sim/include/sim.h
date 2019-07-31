#ifndef SIM_H
#define SIM_H

#include <string.h>
#include "memory.h"
#include "registers.h"
#include "cpu.h"
#include "run.h"
#include "instructions_queue.h"
#include "clock.h"
#include "../scoreboarding/include/ula.h"
#include "../scoreboarding/include/scoreboarding.h"
#include "../scoreboarding/include/scoreboarding_list.h"
#include "../translater/integration.h"
#include "../translater/translater.tab.h"


extern int num_instructions;
extern int finish_PC;
extern FILE* output;
extern FILE* input_assembly;

void initialize_simulator();

#endif