#ifndef SCOREBOARDING_H
#define SCOREBOARDING_H

#include "../registers.h"
#include "../instructions.h"
#include "../clock.h"
#include "ula.h"
#include <stdbool.h>

Register registers_buffer[34];

void scoreboarding(Instruction* instruction, int data);
bool issue(int operation, int destiny, int source1, int source2);
bool read_operands(int functional_unit);
bool write_back(int functional_unit);
int get_functional_unit(int operation);

#endif