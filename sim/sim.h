#ifndef SIM_H
#define SIM_H

#include "memory.h"
#include "registers.h"
#include "cpu.h"
#include "./scoreboarding/ula.h"
#include "instructions_queue.h"
#include "./scoreboarding/scoreboarding_list.h"
#include "./translater/integration.h"
#include "./translater/translater.tab.h"
#include <string.h>

extern int num_instructions;
extern int finish_PC;
extern FILE* output;
extern FILE* input_assembly;

void initialize_simulator();

#endif