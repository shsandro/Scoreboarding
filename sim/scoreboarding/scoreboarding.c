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
    if (ISSUED){
        int data = get_instruction();
        printf("instruction %d\n", data);
        Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
        int instruction_type = (data >> 26) & MASK_TYPE;
        instruction->opcode = instruction_type;
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
        issue(instruction);
    }
    
}

/*Espera até que uma uf correspondente esteja disponível e o registrador de destino também*/
void wait(int functional_unit, int destiny){
    switch (functional_unit) {
        case MUL:
            if (destiny == PC_BUFFER ) while(!functional_units[UF_MUL1].busy || !functional_units[UF_MUL2].busy){}
            else while((!functional_units[UF_MUL1].busy || !functional_units[UF_MUL2].busy) && registers[destiny].fu != NONE){printf("esperando");}
            if (!functional_units[UF_MUL1].busy) functional_units[UF_MUL1].busy = true;
            else functional_units[UF_MUL2].busy = true;
            break;
        case DIV:
            if (destiny == PC_BUFFER) while(!functional_units[UF_DIV1].busy || !functional_units[UF_DIV2].busy){}
            else while((!functional_units[UF_DIV1].busy || !functional_units[UF_DIV2].busy) && registers[destiny].fu != NONE){printf("esperando");}
            if (!functional_units[UF_DIV1].busy) functional_units[UF_DIV1].busy = true;
            else functional_units[UF_DIV2].busy = true;
            break;
        case SUB:
            if (destiny == PC_BUFFER) while(!functional_units[UF_SUB].busy){}
            else while(!functional_units[UF_SUB].busy && registers[destiny].fu != NONE){printf("esperando");}
            functional_units[UF_SUB].busy = true;
            break;
        case ADD:
            if (destiny == PC_BUFFER) while(!functional_units[UF_ADD].busy){}
            else while(!functional_units[UF_ADD].busy && registers[destiny].fu != NONE){printf("esperando");}
            functional_units[UF_ADD].busy = true;
    }
}

void issue(Instruction* instruction){
    instruction->stage = ISSUE;
    switch (instruction->opcode){
        case SPECIAL2:
            switch (instruction->r_instruction.funct){
                case I_MADD:
                    wait(instruction->functional_unit, HIGH);
                    while(registers[LOW].fu != NONE){}; // também espera pelo registrador LOW
                    instruction->functional_unit = (functional_units[UF_MUL1].busy)? UF_MUL2 : UF_MUL1;
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    break;
                case I_MSUB:
                    wait(instruction->functional_unit, HIGH);
                    while(registers[LOW].fu != NONE){}; // também espera pelo registrador LOW
                    instruction->functional_unit = (functional_units[UF_MUL1].busy)? UF_MUL2 : UF_MUL1;
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    break;
                case I_MUL:
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = (functional_units[UF_MUL1].busy)? UF_MUL2 : UF_MUL1;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, LOW);
                    while(registers[HIGH].fu != NONE){}; // também espera pelo registrador HIGH
                    instruction->functional_unit = (functional_units[UF_DIV1].busy)? UF_DIV2 : UF_DIV1;
                    functional_units[instruction->functional_unit].operation = OP_DIV;
                    functional_units[instruction->functional_unit].Fi = LOW;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    registers[HIGH].fu = instruction->functional_unit; //também tem que colocar que o HIGH estará ocupado
                    break;
                case I_JR:
                    wait(instruction->functional_unit, PC_BUFFER);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, HIGH);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, LOW);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, HIGH);
                    while(registers[LOW].fu != NONE){}; // também espera pelo registrador LOW
                    instruction->functional_unit = (functional_units[UF_MUL1].busy)? UF_MUL2 : UF_MUL1;
                    functional_units[instruction->functional_unit].operation = OP_MUL;
                    functional_units[instruction->functional_unit].Fi = HIGH;
                    functional_units[instruction->functional_unit].Fj = instruction->r_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->r_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->r_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->r_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->r_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->r_instruction.rt].fu == NONE)? true : false;
                    registers[LOW].fu = instruction->functional_unit; //também tem que colocar que o LOW estará ocupado
                    break;
                case I_NOP:
                    wait(instruction->functional_unit, NONE);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_SUB;
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
                    wait(instruction->functional_unit, instruction->r_instruction.rd);
                    instruction->functional_unit = UF_ADD;
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
            wait(instruction->functional_unit, PC_BUFFER);
            instruction->functional_unit = UF_ADD;
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
            wait(instruction->functional_unit, PC_BUFFER);
            instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->i_instruction.rt);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->i_instruction.rt);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, PC_BUFFER);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, PC_BUFFER);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, PC_BUFFER);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, PC_BUFFER);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, PC_BUFFER);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->i_instruction.rt);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->i_instruction.rt);
                    instruction->functional_unit = UF_ADD;
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
                    wait(instruction->functional_unit, instruction->i_instruction.rt);
                    instruction->functional_unit = UF_ADD;
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
    read_operands(instruction);
}

void read_operands(Instruction* instruction){
    instruction->stage = READ_OPERANDS;
    if (functional_units[instruction->functional_unit].Fj != NONE && functional_units[instruction->functional_unit].Fk != NONE){
        while (registers[functional_units[instruction->functional_unit].Fj].fu != NONE && registers[functional_units[instruction->functional_unit].Fk].fu != NONE){}
    } else if (functional_units[instruction->functional_unit].Fj != NONE){
        while (registers[functional_units[instruction->functional_unit].Fj].fu != NONE){}
    } else {
        while (registers[functional_units[instruction->functional_unit].Fk].fu != NONE){}
    }
    functional_units[instruction->functional_unit].Rj = true;
    functional_units[instruction->functional_unit].Rk = true;
    execute(instruction);
}

void execute(Instruction* instruction){
    instruction->stage = EXECUTION;
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

    write_back(instruction);
}

// void check_WAR(int functional_unit){
//     for (int i = 0; i < 6 ; ++i){
//         if (i == functional_unit) continue;
//         while (functional_units[i].Fj == functional_units[functional_unit].Fi || functional_units[i].Fk == functional_units[functional_unit].Fi)
//     }
// }

void write_back(Instruction* instruction){
    instruction->stage = WRITE_BACK;
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
    free(instruction);
}
