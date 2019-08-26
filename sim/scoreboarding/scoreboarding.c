#include "./include/scoreboarding.h"

int instructions_written = 0;
int branches_taken = 0;
int branches_hit = 0;
int branches_miss = 0;
bool ISSUED = true;

/*Recebe uma operação e devolve que tipo de unidade funcional pode executá-la*/
int get_functional_unit(int opcode, int operation){
    switch (opcode) {
        case J:
            return ADD; //jump é executado em uma uf de ADD
        case SPECIAL2:
            return MUL; //todas instruções SPECIAL2 utilizam a UF MUL
        default:
            switch (operation) {
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

void clear_list(Instruction* instruction){
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        if (scoreboarding_list.list[i]->issue_clock > instruction->issue_clock){
            reinitialize_unit(scoreboarding_list.list[i]->functional_unit);
            if (functional_units[scoreboarding_list.list[i]->functional_unit].Fi != REGISTER_PC){
                int dest_register = functional_units[scoreboarding_list.list[i]->functional_unit].Fi;
                registers[dest_register].fu = NONE;
            } 
            free(scoreboarding_list.list[i]);
            scoreboarding_list.list[i] = NULL;
            scoreboarding_list.num_instructions--;
            ISSUED = true;
        }
    }
}

/*Executa os estágios do scoreboarding para cada instrução dentro dele*/
void run_scoreboarding(){
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        if (scoreboarding_list.list[i]->stage == WRITE_BACK) write_back(scoreboarding_list.list[i]);
    }

    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        if (scoreboarding_list.list[i]->stage == EXECUTION) execute(scoreboarding_list.list[i]);
    }

    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        if (scoreboarding_list.list[i]->stage == READ_OPERANDS) read_operands(scoreboarding_list.list[i]);
    }

    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        if (scoreboarding_list.list[i]->stage == ISSUE) issue(scoreboarding_list.list[i]);
    }
}

/*Recebe uma instrução e a manda para o estágio de issue se a anterior já foi emitida. Para cada instrução dentro do scoreboarding, chama pra sua fase*/
void scoreboarding(){
    if (ISSUED && get_status_queue() == NOT_EMPTY){
        Instruction* instruction = get_instruction();
        instruction->stage = ISSUE;
        instruction->issue_clock = get_clock();
        ISSUED = false;
        insert_scoreboardig(instruction);
    }

    printf("\n--------------SCOREBOARDING -----------------");
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        printf("\nInstrução: %d Estagio: %d\t", scoreboarding_list.list[i]->opcode, scoreboarding_list.list[i]->stage);
        printf("\nPC = %d", PC.data);
    }
    printf("\n");

    run_scoreboarding();
}

/*Espera até que uma uf correspondente esteja disponível e o registrador de destino também*/
int wait(int functional_unit, int destiny){
    switch (functional_unit) {
        case MUL:
            if (destiny == REGISTER_PC ){
                if (functional_units[UF_MUL1].busy && functional_units[UF_MUL2].busy) return UNAVAILABLE;
                else {
                    if (!functional_units[UF_MUL1].busy){
                        functional_units[UF_MUL1].busy = true;
                        registers[destiny].fu = UF_MUL1;
                        return UF_MUL1;
                    } else {
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
                } else {
                    functional_units[UF_MUL2].busy = true;
                    registers[destiny].fu = UF_MUL2;
                    return UF_MUL2;
                }
            }
            return UNAVAILABLE;
            break;
        case DIV:
            if (destiny == REGISTER_PC ){
                if (functional_units[UF_DIV1].busy && functional_units[UF_DIV2].busy) return UNAVAILABLE;
                else {
                    if (!functional_units[UF_DIV1].busy){
                        functional_units[UF_DIV1].busy = true;
                        registers[destiny].fu = UF_DIV1;
                        return UF_DIV1;
                    } else {
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
                } else {
                    functional_units[UF_DIV2].busy = true;
                    registers[destiny].fu = UF_DIV2;
                    return UF_DIV2;
                }
            }
            return UNAVAILABLE;
            break;
        case SUB:
            if (destiny == REGISTER_PC) {
                if (functional_units[UF_SUB].busy) return UNAVAILABLE;
                else {
                    functional_units[UF_SUB].busy = true;
                    registers[destiny].fu = UF_SUB;
                    return UF_SUB;
                }
            } else if (functional_units[UF_SUB].busy || registers[destiny].fu != NONE) {
                if (functional_units[UF_SUB].busy) printf("\n problema na UF");
                else printf("\nproblema no reg");
                printf("\n uf reg %d", registers[destiny].fu);
                return UNAVAILABLE;} 
            functional_units[UF_SUB].busy = true;
            registers[destiny].fu = UF_SUB;
            return UF_SUB;
            break;
        case ADD:
            if (destiny == REGISTER_PC) {
                if (functional_units[UF_ADD].busy) return UNAVAILABLE;
                else {
                    functional_units[UF_ADD].busy = true;
                    registers[destiny].fu = UF_ADD;
                    return UF_ADD;
                }
            } else if (functional_units[UF_ADD].busy || registers[destiny].fu != NONE) return UNAVAILABLE; 
            functional_units[UF_ADD].busy = true;
            registers[destiny].fu = (destiny == 0)? NONE : UF_ADD;
            return UF_ADD;
    }
}

/*Realiza a emissão de uma instrução dentro do scoreboarding e em seguida a manda para a leitura de operandos*/
void issue(Instruction* instruction){
    printf("\n %d", instruction->r_instruction.funct);
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
                    available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = REGISTER_PC;
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
                    functional_units[instruction->functional_unit].Fi = 0;
                    functional_units[instruction->functional_unit].Fj = 0;
                    functional_units[instruction->functional_unit].Fk = 0;
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
            available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
            if (available_functional_unit == UNAVAILABLE) return;
            instruction->functional_unit = available_functional_unit;
            functional_units[instruction->functional_unit].operation = OP_ADD;
            functional_units[instruction->functional_unit].Fi = REGISTER_PC;
            functional_units[instruction->functional_unit].Fj = instruction->regimm_instruction.rs;
            functional_units[instruction->functional_unit].Fk = NONE;
            functional_units[instruction->functional_unit].Qj = registers[instruction->regimm_instruction.rs].fu;
            functional_units[instruction->functional_unit].Qk = NONE;
            functional_units[instruction->functional_unit].Rj = (registers[instruction->regimm_instruction.rs].fu == NONE)? true : false;
            functional_units[instruction->functional_unit].Rk = true;
            break;
        case J:
            available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
            if (available_functional_unit == UNAVAILABLE) return;
            instruction->functional_unit = available_functional_unit;
            functional_units[instruction->functional_unit].operation = OP_ADD;
            functional_units[instruction->functional_unit].Fi = REGISTER_PC;
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
                    available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = REGISTER_PC;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->i_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->i_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_BEQL:
                    available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = REGISTER_PC;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = instruction->i_instruction.rt;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = registers[instruction->i_instruction.rt].fu;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = (registers[instruction->i_instruction.rt].fu == NONE)? true : false;
                    break;
                case I_BGTZ:
                    printf("\n fazendo bgtz");
                    available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = REGISTER_PC;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_BLEZ:
                    available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = REGISTER_PC;
                    functional_units[instruction->functional_unit].Fj = instruction->i_instruction.rs;
                    functional_units[instruction->functional_unit].Fk = NONE;
                    functional_units[instruction->functional_unit].Qj = registers[instruction->i_instruction.rs].fu;
                    functional_units[instruction->functional_unit].Qk = NONE;
                    functional_units[instruction->functional_unit].Rj = (registers[instruction->i_instruction.rs].fu == NONE)? true : false;
                    functional_units[instruction->functional_unit].Rk = true;
                    break;
                case I_BNE:
                    available_functional_unit = wait(instruction->functional_unit, REGISTER_PC);
                    if (available_functional_unit == UNAVAILABLE) return;
                    instruction->functional_unit = available_functional_unit;
                    functional_units[instruction->functional_unit].operation = OP_ADD;
                    functional_units[instruction->functional_unit].Fi = REGISTER_PC;
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

    if(functional_units[instruction->functional_unit].Fi != 0 && functional_units[instruction->functional_unit].Fi != REGISTER_PC)
        registers[functional_units[instruction->functional_unit].Fi].fu = instruction->functional_unit;
    ISSUED = true;
    instruction->stage = READ_OPERANDS;
}

/*Realiza a leitura dos registradores origem de uma instrução e em seguida a manda para a execução*/
void read_operands(Instruction* instruction){
    //Se vou ler do registrador que estou escrevendo, vai para o read_default
    if (functional_units[instruction->functional_unit].Fj != NONE && functional_units[instruction->functional_unit].Fk != NONE){
        if (functional_units[instruction->functional_unit].Fj == functional_units[instruction->functional_unit].Fi && functional_units[instruction->functional_unit].Fk == functional_units[instruction->functional_unit].Fi) goto read_default;
        else if (functional_units[instruction->functional_unit].Fj == functional_units[instruction->functional_unit].Fi){
            if (registers[functional_units[instruction->functional_unit].Fk].fu != NONE) return;
        } else if (functional_units[instruction->functional_unit].Fk == functional_units[instruction->functional_unit].Fi){
            if (registers[functional_units[instruction->functional_unit].Fj].fu != NONE) return;
        } else if (registers[functional_units[instruction->functional_unit].Fj].fu != NONE || registers[functional_units[instruction->functional_unit].Fk].fu != NONE) return;
    } else if (functional_units[instruction->functional_unit].Fj != NONE){
        if (functional_units[instruction->functional_unit].Fj == functional_units[instruction->functional_unit].Fi) goto read_default;
        if (registers[functional_units[instruction->functional_unit].Fj].fu != NONE) return;
    } else if (functional_units[instruction->functional_unit].Fk != NONE){
        if (functional_units[instruction->functional_unit].Fk == functional_units[instruction->functional_unit].Fi) goto read_default;
        if (registers[functional_units[instruction->functional_unit].Fk].fu != NONE) return;
    }
    read_default:
    functional_units[instruction->functional_unit].Rj = true;
    functional_units[instruction->functional_unit].Rk = true;
    functional_units[instruction->functional_unit].Vj = read_register(functional_units[instruction->functional_unit].Fj);
    functional_units[instruction->functional_unit].Vk = read_register(functional_units[instruction->functional_unit].Fk);
    instruction->stage = EXECUTION;
}

void bypass(int dest_register, int data){
    printf("entrei no bypass\n");
    for (int i = 0; i < scoreboarding_list.max_instructions; ++i){
        if (scoreboarding_list.list[i] == NULL) continue;
        if (functional_units[scoreboarding_list.list[i]->functional_unit].Fj == dest_register){
            functional_units[scoreboarding_list.list[i]->functional_unit].Rj = true;
            functional_units[scoreboarding_list.list[i]->functional_unit].Vj = data;
            printf("usei\n");
        }
        if (functional_units[scoreboarding_list.list[i]->functional_unit].Fk == dest_register) {
            functional_units[scoreboarding_list.list[i]->functional_unit].Rk = true;
            functional_units[scoreboarding_list.list[i]->functional_unit].Vk = data;
            printf("usei\n");
        }
    }
}

/*Executa uma instrução e em seguida a envia para o estagio de write back*/
void execute(Instruction* instruction){
    if (instruction->execution_begin == NONE) instruction->execution_begin = get_clock();
    switch (instruction->opcode){
        case SPECIAL:
            switch (instruction->r_instruction.funct){
                case I_ADD:
                {
                    if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }
                break;
                case I_AND:
                {
                    if (get_clock() != (instruction->execution_begin + AND_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_AND, operand_1, operand_2);
                }
                break;
                case I_DIV:
                {
                    if (get_clock() != (instruction->execution_begin + DIV_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = div_unit(OP_MOD, operand_1, operand_2);
                    functional_units[instruction->functional_unit].result[1] = div_unit(OP_DIV, operand_1, operand_2);
                }    
                break;
                case I_JR:
                {
                    ++branches_taken;
                    ++branches_hit;
                    //if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    PC.data = adder(operand_1, operand_2);
                    clear_queue();
                    clear_list(instruction);
                }
                break;
                case I_MFHI:
                {
                    if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MFLO:
                {
                    if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MOVN:
                {
                    if (functional_units[instruction->functional_unit].Vk != 0){
                        if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                        int operand_1 = functional_units[instruction->functional_unit].Vj;
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, 0);
                    } else functional_units[instruction->functional_unit].result[0] = read_register(functional_units[instruction->functional_unit].Fi);
                }    
                break;
                case I_MOVZ:
                {
                    if (functional_units[instruction->functional_unit].Vk == 0){
                        if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                        int operand_1 = functional_units[instruction->functional_unit].Vj;
                        int operand_2 = functional_units[instruction->functional_unit].Vk;
                        functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                    } else functional_units[instruction->functional_unit].result[0] = read_register(functional_units[instruction->functional_unit].Fi);
                }    
                break;
                case I_MTHI:
                {
                    if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MTLO:
                {
                    if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, operand_2);
                }    
                break;
                case I_MULT:
                {
                    if (get_clock() != (instruction->execution_begin + MUL_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = mul_unit(operand_1, operand_2) >> 16;
                    functional_units[instruction->functional_unit].result[1] = (mul_unit(operand_1, operand_2) << 16) >> 16;
                }    
                break;
                case I_NOP:
                {
                    if (get_clock() != (instruction->execution_begin + 1)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_SL, operand_1, operand_2);
                }    
                break;
                case I_NOR:
                {
                    if (get_clock() != (instruction->execution_begin + NOR_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_NOR, operand_1, operand_2);
                }    
                break;
                case I_OR:
                {
                    if (get_clock() != (instruction->execution_begin + OR_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_OR, operand_1, operand_2);
                }    
                break;
                case I_SUB:
                {
                    if (get_clock() != (instruction->execution_begin + SUB_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = sub_unit(operand_1, operand_2);
                }    
                break;
                case I_XOR:
                {
                    if (get_clock() != (instruction->execution_begin + XOR_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_XOR, operand_1, operand_2);
                }    
                break;
            }
            break;
        case SPECIAL2:
            switch (instruction->r_instruction.funct) {
                case I_MADD:
                {
                    if (get_clock() != (instruction->execution_begin + MUL_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, (mul_unit(operand_1, operand_2) >> 16), operand_1);
                    functional_units[instruction->functional_unit].result[1] = add_unit(OP_ADD, (mul_unit(operand_1, operand_2) << 16) >> 16, operand_2);
                }    
                break;
                case I_MSUB:
                {
                    if (get_clock() != (instruction->execution_begin + MUL_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = sub_unit((mul_unit(operand_1, operand_2) >> 16), operand_1);
                    functional_units[instruction->functional_unit].result[1] = sub_unit((mul_unit(operand_1, operand_2) << 16) >> 16, operand_2);
                }    
                break;
                case I_MUL:
                {
                    if (get_clock() != (instruction->execution_begin + MUL_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    functional_units[instruction->functional_unit].result[0] = mul_unit(operand_1, operand_2);
                }    
                break;
            }
            break;
        case REGIMM:
            switch (instruction->regimm_instruction.funct) {
                case I_BGEZ:
                    ++branches_taken;
                    if (functional_units[instruction->functional_unit].Vj < 0){//Salto é tomado se maior ou igual a zero. Se menor devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->regimm_instruction.offset, PC.data);
                    }else ++branches_hit;
                    break;
                case I_BLTZ:
                    ++branches_taken;
                    if (functional_units[instruction->functional_unit].Vj >= 0){//Salto é tomado se menor que zero. Se maior ou igual devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->regimm_instruction.offset, PC.data);
                    }else ++branches_hit;
            }
            break;
        case J:
            ++branches_taken;
            ++branches_hit;
            //PC já foi atualizado na fase de ISSUE
            // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
            break;
        default:
            switch (instruction->opcode) {
                case I_ADDI:
                {
                    if (get_clock() != (instruction->execution_begin + ADD_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, operand_1, instruction->i_instruction.immediate);
                }    
                break;
                case I_ANDI:
                {
                    if (get_clock() != (instruction->execution_begin + AND_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_AND, operand_1, instruction->i_instruction.immediate);
                }    
                break;
                case I_BEQ:
                {
                    ++branches_taken;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    printf("valores comparados %d %d\n", operand_1, operand_2);
                    if (operand_1 != operand_2){//salto é tomado se igual. Se diferente devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    }else ++branches_hit;
                }    
                break;
                case I_BEQL:
                {
                    ++branches_taken;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    if (operand_1 != operand_2){//salto é tomado se igual. Se diferente devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    }else ++branches_hit;
                }    
                break;
                case I_BGTZ:
                    ++branches_taken;
                     if (functional_units[instruction->functional_unit].Vj <= 0){//salto é tomado se maior que zero. Se é menor ou igual devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        // printf("\nAR DO SALTO = %d", AR.data);
                        // printf("\nPC DO SALTO = %d", PC.data);
                        // exit(-1);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    }else ++branches_hit;
                    break;
                case I_BLEZ:
                    ++branches_taken;
                    if (functional_units[instruction->functional_unit].Vj > 0){//salto é tomado se menor ou igual a zero. Se é maior devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    }else ++branches_hit;
                    break;
                case I_BNE:
                {
                    ++branches_taken;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    int operand_2 = functional_units[instruction->functional_unit].Vk;
                    if (operand_1 == operand_2){ //salto é tomado se diferente. Se é igual devo arrumar PC
                        PC.data = adder(AR.data, 0);
                        clear_queue();
                        clear_list(instruction);
                        ++branches_miss;
                        // if (get_clock() != (instruction->execution_begin + BRANCH_CYCLES)) return;
                        // functional_units[instruction->functional_unit].result[0] = add_unit(OP_ADD, instruction->i_instruction.immediate, PC.data);
                    }else ++branches_hit;
                }    
                break;
                case I_LUI:
                    if (get_clock() != (instruction->execution_begin + SL_CYCLES)) return;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_SL, instruction->i_instruction.immediate, 16);
                    break;
                case I_ORI:
                {
                    if (get_clock() != (instruction->execution_begin + OR_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_OR, operand_1, instruction->i_instruction.immediate);
                }    
                break;
                case I_XORI:
                {
                    if (get_clock() != (instruction->execution_begin + XOR_CYCLES)) return;
                    int operand_1 = functional_units[instruction->functional_unit].Vj;
                    functional_units[instruction->functional_unit].result[0] = add_unit(OP_XOR, operand_1, instruction->i_instruction.immediate);
                }   
                break;
            }        
    }
    // bypass(functional_units[instruction->functional_unit].Fi, functional_units[instruction->functional_unit].result[0]);
    instruction->stage = WRITE_BACK;
}

// bool check_WAR(Instruction* instruction){
//     for (int i = 0; i < scoreboarding_list.max_instructions ; ++i){
//         if (scoreboarding_list.list[i] == NULL) continue;
//         switch (scoreboarding_list.list[i]->stage) {
//         case ISSUE:
//         case READ_OPERANDS:
//             if(functional_units[instruction->functional_unit].Fi == functional_units[scoreboarding_list.list[i]->functional_unit].Fj || functional_units[instruction->functional_unit].Fi == functional_units[scoreboarding_list.list[i]->functional_unit].Fk) return true;
//         }
//     }
//     return false;
// }

/*Realiza a escrita da instrução no registrador destino e libera o registrador e a UF*/
void write_back(Instruction* instruction){
    switch (instruction->opcode) {
        case SPECIAL:
            switch (instruction->r_instruction.funct){
                case I_DIV:
                case I_MULT:
                    registers[HIGH].data = functional_units[instruction->functional_unit].result[0];
                    registers[LOW].data = functional_units[instruction->functional_unit].result[1];
                    registers[HIGH].fu = NONE;
                    registers[LOW].fu = NONE;
                    break;
                default:
                    if (functional_units[instruction->functional_unit].Fi != REGISTER_PC){
                        int dest_register = functional_units[instruction->functional_unit].Fi;
                        registers[dest_register].data = functional_units[instruction->functional_unit].result[0];
                        registers[dest_register].fu = NONE;
                    }
            }
            break ;
        case SPECIAL2:
            switch (instruction->r_instruction.funct){
                case I_MUL:
                {
                    int dest_register = functional_units[instruction->functional_unit].Fi;
                    registers[dest_register].data = functional_units[instruction->functional_unit].result[0];
                    registers[dest_register].fu = NONE;
                }
                    break;
                default:
                    registers[HIGH].data = functional_units[instruction->functional_unit].result[0];
                    registers[LOW].data = functional_units[instruction->functional_unit].result[1];
                    registers[LOW].fu = NONE;
                    registers[HIGH].fu = NONE;
                    break;
            }
        default:
            if (functional_units[instruction->functional_unit].Fi != REGISTER_PC){
                int dest_register = functional_units[instruction->functional_unit].Fi;
                registers[dest_register].data = functional_units[instruction->functional_unit].result[0];
                registers[dest_register].fu = NONE;
            }
    }
    reinitialize_unit(instruction->functional_unit);
    remove_scoreboarding(instruction);
    free(instruction);
    ++instructions_written;
}
