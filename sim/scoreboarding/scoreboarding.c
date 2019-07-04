#include "scoreboarding.h"

bool ISSUED = true;

/*Recebe uma operação e devolve que tipo de unidade funcional pode executá-la*/
int get_functional_unit(int opcode, int operation){
    switch (opcode) {
        case J:
            return ADD; //jump é executado em uma uf de ADD
        default:
            switch (operation) {
                case I_MUL:
                case I_MSUB:
                case I_MADD:
                case I_MULT:
                    return MUL; //instruções que utilizam UF MUL
                case I_SUB:
                    return SUB; //instruções que utilizam UF SUB
                case I_DIV:
                    return DIV; //instruções que utilizam UF DIV
                default:
                    return ADD; //instruções que utilizam UF ADD
            }
    }
}

/*Recebe uma instrução, decodifica-a se a instrução anterior já foi emitida, e a manda para o estágio de issue*/
void scoreboarding(){
    if (ISSUED && get_status_queue() == NOT_EMPTY){
        int data = get_instruction();
        printf("instruction %d\n", data);
        Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
        int instruction_type = (data >> 26) & MASK_TYPE;
        instruction->opcode = instruction_type;
        instruction->stage = ISSUE;
        int functional_unit;

        switch (instruction_type) {
            case SPECIAL:
            {
                instruction->r_instruction.rs = (data >> 21) & MASK_REGISTER;
                instruction->r_instruction.rt = (data >> 16) & MASK_REGISTER;
                instruction->r_instruction.rd = (data >> 11) & MASK_REGISTER;
                instruction->r_instruction.shamt = (data >> 6) & MASK_REGISTER;
                instruction->r_instruction.funct = data & MASK_TYPE;
                functional_unit = get_functional_unit(instruction->opcode, instruction->r_instruction.funct);
                instruction->functional_unit = functional_unit;
                printf("SPECIAL: ");
                printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
                printf("FU: %d\n", functional_unit);
            }
                break;
            case SPECIAL2:
            {
                instruction->r_instruction.rs = (data >> 21) & MASK_REGISTER;
                instruction->r_instruction.rt = (data >> 16) & MASK_REGISTER;
                instruction->r_instruction.rd = (data >> 11) & MASK_REGISTER;
                instruction->r_instruction.shamt = (data >> 6) & MASK_REGISTER;
                instruction->r_instruction.funct = data & MASK_TYPE;
                functional_unit = get_functional_unit(instruction->opcode, instruction->r_instruction.funct);
                instruction->functional_unit = functional_unit;
                printf("SPECIAL2: ");
                printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
                printf("FU: %d\n", functional_unit);
            }
                break;
            case REGIMM:
            {
                instruction->regimm_instruction.rs = (data >> 21) & MASK_REGISTER;
                instruction->regimm_instruction.funct = (data >> 16) & MASK_REGISTER;
                instruction->regimm_instruction.offset = data & MASK_OFFSET;
                functional_unit = get_functional_unit(instruction->opcode, instruction->regimm_instruction.funct);
                instruction->functional_unit = functional_unit;
                printf("REGIMM: ");
                printf("%d %d %d %d\n", instruction->opcode, instruction->regimm_instruction.rs, instruction->regimm_instruction.funct, instruction->regimm_instruction.offset);
                printf("FU: %d\n", functional_unit);
            }
                break;
            case J:
            {
                instruction->j_instruction.target = data & MASK_TARGET;
                functional_unit = get_functional_unit(instruction->opcode, instruction->opcode);
                instruction->functional_unit = functional_unit;
                printf("J: ");
                printf("%d %d\n", instruction->opcode, instruction->j_instruction.target);
                printf("FU: %d\n", functional_unit);
            }
                break;
            default:
            {
                instruction->i_instruction.rs = (data >> 21) & MASK_REGISTER;
                instruction->i_instruction.rt = (data >> 16) & MASK_REGISTER;
                instruction->i_instruction.immediate = data & MASK_OFFSET;
                functional_unit = get_functional_unit(instruction->opcode, instruction->opcode);
                instruction->functional_unit = functional_unit;
                printf("IMMEDIATE: ");
                printf("%d %d %d %d\n", instruction->opcode, instruction->i_instruction.rs, instruction->i_instruction.rt, instruction->i_instruction.immediate);
                printf("FU: %d\n", functional_unit);
            }
        }
        ISSUED = false;
        insert_scoreboardig(instruction);
    }
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        printf("estagio: %d\t",scoreboarding_list.list[i]->stage);
    }
    printf("\n");
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        switch (scoreboarding_list.list[i]->stage) {
            case ISSUE:
                issue(scoreboarding_list.list[i]);
                break;
            case READ_OPERANDS:
                read_operands(scoreboarding_list.list[i]);
                break;
            case EXECUTION:
                execute(scoreboarding_list.list[i]);
                break;
            case WRITE_BACK:
                write_back(scoreboarding_list.list[i]);
        }
    }
}

/*Espera até que uma uf correspondente esteja disponível e o registrador de destino também*/
int wait(int functional_unit, int destiny){
    switch (functional_unit) {
        case MUL:
            if (destiny == PC_BUFFER ){
                if (functional_units[UF_MUL1].busy && functional_units[UF_MUL2].busy) return UNAVAILABLE;
                else {
                    if (!functional_units[UF_MUL1].busy){
                        functional_units[UF_MUL1].busy = true;
                        registers[destiny].fu = UF_MUL1;
                        return UF_MUL1;
                    }
                    else {
                        functional_units[UF_MUL2].busy = true;
                        registers[destiny].fu = UF_MUL2;
                        return UF_MUL2;
                    }
                }
            }
            else if ((!functional_units[UF_MUL1].busy || !functional_units[UF_MUL2].busy) && registers[destiny].fu == NONE){
                if (!functional_units[UF_MUL1].busy){
                    functional_units[UF_MUL1].busy = true;
                    registers[destiny].fu = UF_MUL1; 
                    return UF_MUL1;
                }
                else {
                    functional_units[UF_MUL2].busy = true;
                    registers[destiny].fu = UF_MUL2;
                    return UF_MUL2;
                }
            }
            return UNAVAILABLE;
            break;
        case DIV:
            if (destiny == PC_BUFFER ){
                if (functional_units[UF_DIV1].busy && functional_units[UF_DIV2].busy) return UNAVAILABLE;
                else {
                    if (!functional_units[UF_DIV1].busy){
                        functional_units[UF_DIV1].busy = true;
                        registers[destiny].fu = UF_DIV1;
                        return UF_DIV1;
                    }
                    else {
                        functional_units[UF_DIV2].busy = true;
                        registers[destiny].fu = UF_DIV2;
                        return UF_DIV2;
                    }
                }
            }
            else if ((!functional_units[UF_DIV1].busy || !functional_units[UF_DIV2].busy) && registers[destiny].fu == NONE){
                if (!functional_units[UF_DIV1].busy){
                    functional_units[UF_DIV1].busy = true;
                    registers[destiny].fu = UF_DIV1;
                    return UF_DIV1;
                }
                else {
                    functional_units[UF_DIV2].busy = true;
                    registers[destiny].fu = UF_DIV2;
                    return UF_DIV2;
                }
            }
            return UNAVAILABLE;
            break;
        case SUB:
            if (destiny == PC_BUFFER) {
                if (functional_units[UF_SUB].busy) return UNAVAILABLE;
                else {
                    functional_units[UF_SUB].busy = true;
                    registers[destiny].fu = UF_SUB;
                    return UF_SUB;
                }
            }
            else if (functional_units[UF_SUB].busy || registers[destiny].fu != NONE) return UNAVAILABLE; 
            functional_units[UF_SUB].busy = true;
            registers[destiny].fu = UF_SUB;
            return UF_SUB;
            break;
        case ADD:
            if (destiny == PC_BUFFER) {
                if (functional_units[UF_ADD].busy) return UNAVAILABLE;
                else {
                    functional_units[UF_ADD].busy = true;
                    registers[destiny].fu = UF_ADD;
                    return UF_ADD;
                }
            }
            else if (functional_units[UF_ADD].busy || registers[destiny].fu != NONE) return UNAVAILABLE; 
            functional_units[UF_ADD].busy = true;
            registers[destiny].fu = UF_ADD;
            return UF_ADD;
    }
}

void issue(Instruction* instruction){
    int available_functional_unit;
    switch (instruction->opcode){
        case SPECIAL2:
            switch (instruction->r_instruction.funct){
                case I_MADD:
                    if (registers[LOW].fu != NONE) return; // também espera pelo registrador LOW
                    available_functional_unit = wait(instruction->functional_unit, HIGH);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_MSUB:
                    if (registers[LOW].fu != NONE) return; // também espera pelo registrador LOW
                    available_functional_unit = wait(instruction->functional_unit, HIGH);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_MUL:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
            }
            break;
        case SPECIAL:
            switch (instruction->r_instruction.funct){
                case I_ADD:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_AND:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_AND;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_DIV:
                    if (registers[LOW].fu != NONE) return; // também espera pelo registrador LOW
                    available_functional_unit = wait(instruction->functional_unit, HIGH);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    functional_units[instruction->functional_unit].operation = OP_DIV;
                    functional_units[instruction->functional_unit].Fi = LOW;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_JR:
                    available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = PC_BUFFER;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_MFHI:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = HIGH;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[HIGH].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[HIGH].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_MFLO:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = LOW;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[LOW].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[LOW].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_MOVN:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_MOVZ:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_MTHI:
                    available_functional_unit = wait(instruction->functional_unit, HIGH);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_MTLO:
                    available_functional_unit = wait(instruction->functional_unit, LOW);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = LOW;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_MULT:
                    if (registers[LOW].fu != NONE) return; // também espera pelo registrador LOW
                    instruction->functional_unit = wait(instruction->functional_unit, HIGH);
                    if (instruction->functional_unit == UNAVAILABLE) return;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_NOP:
                    available_functional_unit = wait(instruction->functional_unit, 0);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = NONE;
                    functional_units[instruction->functional_unit].Fj = NONE;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = NONE;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = true;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_NOR:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_NOR;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_OR:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_OR;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_SUB:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_SUB;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_XOR:
                    available_functional_unit = wait(instruction->functional_unit, instruction->r_instruction.rd);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_XOR;
                    functional_units[instruction->functional_unit].Fi = instruction->r_instruction.rd;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    break;
            }
            break;
        case REGIMM:
            available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
            if (available_functional_unit == UNAVAILABLE) return;
            instruction->functional_unit = available_functional_unit;
            functional_units[instruction->functional_unit].operation = OP_ADD;
            functional_units[instruction->functional_unit].Fi = PC_BUFFER;
            functional_units[instruction->functional_unit].Fj = instruction->regimm_instruction.rs;
            functional_units[instruction->functional_unit].Fk = NONE;
            functional_units[instruction->functional_unit].Qj = registers[instruction->regimm_instruction.rs].fu;
            functional_units[instruction->functional_unit].Qk = NONE;
            functional_units[instruction->functional_unit].Rj = (registers[instruction->regimm_instruction.rs].fu == NONE)? true : false;
            functional_units[instruction->functional_unit].Rk = true;
            break;
        case J:
            available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
            if (available_functional_unit == UNAVAILABLE) return;
            instruction->functional_unit = available_functional_unit;
            functional_units[instruction->functional_unit].operation = OP_ADD;
            functional_units[instruction->functional_unit].Fi = PC_BUFFER;
            functional_units[instruction->functional_unit].Fj = NONE;
            functional_units[instruction->functional_unit].Fk = NONE;
            functional_units[instruction->functional_unit].Qj = NONE;
            functional_units[instruction->functional_unit].Qk = NONE;
            functional_units[instruction->functional_unit].Rj = true;
            functional_units[instruction->functional_unit].Rk = true;
            break;
        default:
            switch (instruction->opcode){
                case I_ADDI:
                    available_functional_unit = wait(instruction->functional_unit, instruction->i_instruction.rt);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_ANDI:
                    available_functional_unit = wait(instruction->functional_unit, instruction->i_instruction.rt);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_AND;
                    functional_units[instruction->functional_unit].Fi = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_BEQ:
                    available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = PC_BUFFER;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->i_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->i_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_BEQL:
                    available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = PC_BUFFER;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->i_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->i_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_BGTZ:
                    available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = PC_BUFFER;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_BLEZ:
                    available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = PC_BUFFER;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_BNE:
                    available_functional_unit = wait(instruction->functional_unit, PC_BUFFER);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = PC_BUFFER;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->i_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->i_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_LUI:
                    available_functional_unit = wait(instruction->functional_unit, instruction->i_instruction.rt);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_SL;
                    functional_units[instruction->functional_unit].Fi = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Fj = NONE;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = NONE;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = true;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_ORI:
                    available_functional_unit = wait(instruction->functional_unit, instruction->i_instruction.rt);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_OR;
                    functional_units[instruction->functional_unit].Fi = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_XORI:
                    available_functional_unit = wait(instruction->functional_unit, instruction->i_instruction.rt);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_XOR;
                    functional_units[instruction->functional_unit].Fi = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
            }
    }

    if(functional_units[instruction->functional_unit].Fi != NONE && functional_units[instruction->functional_unit].Fi != PC_BUFFER)
        registers[functional_units[instruction->functional_unit].Fi].fu = instruction->functional_unit;
    ISSUED = true;
    instruction->stage = READ_OPERANDS;
}

void read_operands(Instruction* instruction){
    if (functional_units[instruction->functional_unit].Fj != NONE && functional_units[instruction->functional_unit].Fk != NONE){
        if (registers[functional_units[instruction->functional_unit].Fj].fu != NONE || registers[functional_units[instruction->functional_unit].Fk].fu != NONE) return;
    } else if (functional_units[instruction->functional_unit].Fj != NONE){
        if (registers[functional_units[instruction->functional_unit].Fj].fu != NONE) return;
    } else {
        if (registers[functional_units[instruction->functional_unit].Fk].fu != NONE) return;
    }
    functional_units[instruction->functional_unit].Rj = true;
    functional_units[instruction->functional_unit].Rk = true;
    instruction->stage = EXECUTION;
}

void execute(Instruction* instruction){
    switch (instruction->opcode){
        case SPECIAL:
            switch (instruction->r_instruction.funct){
                case I_ADD:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }
                break;
                case I_AND:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_AND, operand_1, operand_2);
                }
                break;
                case I_DIV:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = div_unit(OP_DIV, operand_1, operand_2);
                    functional_units[instruction->functional_unit].result[1] = div_unit(OP_MOD, operand_1, operand_2);
                }    
                break;
                case I_JR:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }
                break;
                case I_MFHI:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MFLO:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MOVN:
                {
                    if (read_register(functional_units[instruction->functional_unit].Fk) != 0){
                        int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, 0);
                    } else functional_units[instruction->functional_unit].result[0] = read_register(functional_units[instruction->functional_unit].Fi);
                }    
                break;
                case I_MOVZ:
                {
                    if (read_register(functional_units[instruction->functional_unit].Fk) == 0){
                        int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                        int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                    } else functional_units[instruction->functional_unit].result[0] = read_register(functional_units[instruction->functional_unit].Fi);
                }    
                break;
                case I_MTHI:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MTLO:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MULT:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = mul_unit(operand_1, operand_2) >> 16;
                    functional_units[instruction->functional_unit].result[1] = (mul_unit(operand_1, operand_2) << 16) >> 16;
                }    
                break;
                case I_NOP:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_SL, operand_1, operand_2);
                }    
                break;
                case I_NOR:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_NOR, operand_1, operand_2);
                }    
                break;
                case I_OR:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_OR, operand_1, operand_2);
                }    
                break;
                case I_SUB:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = sub_unit(operand_1, operand_2);
                }    
                break;
                case I_XOR:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_XOR, operand_1, operand_2);
                }    
                break;
            }
            break;
        case SPECIAL2:
            switch (instruction->r_instruction.funct) {
                case I_MADD:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, (mul_unit(operand_1, operand_2) >> 16), operand_1);
                    functional_units[instruction->functional_unit].result[1] = add_unit(OP_ADD, (mul_unit(operand_1, operand_2) << 16) >> 16, operand_2);
                }    
                break;
                case I_MSUB:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = sub_unit((mul_unit(operand_1, operand_2) >> 16), operand_1);
                    functional_units[instruction->functional_unit].result[1] = sub_unit((mul_unit(operand_1, operand_2) << 16) >> 16, operand_2);
                }    
                break;
                case I_MUL:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    functional_units[instruction->functional_unit].result[0] = mul_unit(operand_1, operand_2);
                }    
                break;
            }
            break;
        case REGIMM:
            switch (instruction->regimm_instruction.funct) {
                case I_BGEZ:
                    if (read_register(functional_units[instruction->functional_unit].Fj) >= 0){
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->regimm_instruction.offset, PC.data);
                    } else functional_units[instruction->functional_unit].result[0] = PC.data;
                case I_BLTZ:
                    if (read_register(functional_units[instruction->functional_unit].Fj) < 0){
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->regimm_instruction.offset, PC.data);
                    } else functional_units[instruction->functional_unit].result[0] = PC.data;
            }
            break;
        case J:
            functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->j_instruction.target, 0);
            break;
        default:
            switch (instruction->opcode) {
                case I_ADDI:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, instruction->i_instruction.immediate);
                }    
                break;
                case I_ANDI:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_AND, operand_1, instruction->i_instruction.immediate);
                }    
                break;
                case I_BEQ:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    if (operand_1 == operand_2) functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    else functional_units[instruction->functional_unit].result[0] = PC.data;
                }    
                break;
                case I_BEQL:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    if (operand_1 == operand_2) functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    else functional_units[instruction->functional_unit].result[0] = PC.data;
                }    
                break;
                case I_BGTZ:
                     if (read_register(functional_units[instruction->functional_unit].Fj) > 0){
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    } else functional_units[instruction->functional_unit].result[0] = PC.data;
                    break;
                case I_BLEZ:
                    if (read_register(functional_units[instruction->functional_unit].Fj) <= 0){
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    } else functional_units[instruction->functional_unit].result[0] = PC.data;
                    break;
                case I_BNE:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    int operand_2 = read_register(functional_units[instruction->functional_unit].Fk);
                    if (operand_1 != operand_2) functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    else functional_units[instruction->functional_unit].result[0] = PC.data;
                }    
                break;
                case I_LUI:
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_SL, instruction->i_instruction.immediate, 16);
                    break;
                case I_ORI:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_OR, operand_1, instruction->i_instruction.immediate);
                }    
                break;
                case I_XORI:
                {
                    int operand_1 = read_register(functional_units[instruction->functional_unit].Fj);
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_XOR, operand_1, instruction->i_instruction.immediate);
                }   
                break;
            }        
    }
    instruction->stage = WRITE_BACK;
}

// void check_WAR(int functional_unit){
//     for (int i = 0; i < 6 ; ++i){
//         if (i == functional_unit) continue;
//         while (functional_units[i].Fj == functional_units[functional_unit].Fi || functional_units[i].Fk == functional_units[functional_unit].Fi)
//     }
// }

void write_back(Instruction* instruction){
    printf("entrei write back\n");
    switch (instruction->opcode) {
        case SPECIAL:
            switch (instruction->r_instruction.funct){
                case DIV:
                    registers_buffer[LOW].data = functional_units[instruction->functional_unit].result[0];
                    registers_buffer[HIGH].data = functional_units[instruction->functional_unit].result[1];
                    registers[LOW].fu = NONE;
                    registers[HIGH].fu = NONE;
                    break;
                default:
                    registers_buffer[functional_units[instruction->functional_unit].Fi].data = functional_units[instruction->functional_unit].result[0];
                    if (functional_units[instruction->functional_unit].Fi != PC_BUFFER){
                        int dest_register = functional_units[instruction->functional_unit].Fi;
                        registers[dest_register].fu = NONE;
                    }
            }
            break ;
        case SPECIAL2:
            registers_buffer[LOW].data = functional_units[instruction->functional_unit].result[0];
            registers_buffer[HIGH].data = functional_units[instruction->functional_unit].result[1];
            registers[LOW].fu = NONE;
            registers[HIGH].fu = NONE;
            break;
        default:
            registers_buffer[functional_units[instruction->functional_unit].Fi].data = functional_units[instruction->functional_unit].result[0];
            if (functional_units[instruction->functional_unit].Fi != PC_BUFFER){
                int dest_register = functional_units[instruction->functional_unit].Fi;
                registers[dest_register].fu = NONE;
            }
    }

    reinitialize_unit(instruction->functional_unit);
    remove_scoreboarding(instruction);
    free(instruction);
}
