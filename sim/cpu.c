#include"cpu.h"

/*Decodifica uma instrução*/
Instruction* decode(int data){
    printf("instruction %d\n", data);
    Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
    int instruction_type = (data >> 26) & MASK_TYPE;
    instruction->opcode = instruction_type;
    instruction->stage = NONE;
    instruction->execution_begin = NONE;
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
            printf("-----------------Decodificação-------------------\n");
            printf("SPECIAL: ");
            printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
            printf("FU: %d\n", functional_unit);
            printf("-------------------------------------------------\n");
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
            printf("-----------------Decodificação-------------------\n");
            printf("SPECIAL2: ");
            printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
            printf("FU: %d\n", functional_unit);
            printf("-------------------------------------------------\n");
        }
            break;
        case REGIMM:
        {
            instruction->regimm_instruction.rs = (data >> 21) & MASK_REGISTER;
            instruction->regimm_instruction.funct = (data >> 16) & MASK_REGISTER;
            instruction->regimm_instruction.offset = data & MASK_OFFSET;
            functional_unit = get_functional_unit(instruction->opcode, instruction->regimm_instruction.funct);
            instruction->functional_unit = functional_unit;
            printf("-----------------Decodificação-------------------\n");
            printf("REGIMM: ");
            printf("%d %d %d %d\n", instruction->opcode, instruction->regimm_instruction.rs, instruction->regimm_instruction.funct, instruction->regimm_instruction.offset);
            printf("FU: %d\n", functional_unit);
            printf("-------------------------------------------------\n");
        }
            break;
        case J:
        {
            instruction->j_instruction.target = data & MASK_TARGET;
            functional_unit = get_functional_unit(instruction->opcode, instruction->opcode);
            instruction->functional_unit = functional_unit;
            printf("-----------------Decodificação-------------------\n");
            printf("J: ");
            printf("%d %d\n", instruction->opcode, instruction->j_instruction.target);
            printf("FU: %d\n", functional_unit);
            printf("-------------------------------------------------\n");
        }
            break;
        default:
        {
            instruction->i_instruction.rs = (data >> 21) & MASK_REGISTER;
            instruction->i_instruction.rt = (data >> 16) & MASK_REGISTER;
            instruction->i_instruction.immediate = data & MASK_OFFSET;
            functional_unit = get_functional_unit(instruction->opcode, instruction->opcode);
            instruction->functional_unit = functional_unit;
            printf("-----------------Decodificação-------------------\n");
            printf("IMMEDIATE: ");
            printf("%d %d %d %d\n", instruction->opcode, instruction->i_instruction.rs, instruction->i_instruction.rt, instruction->i_instruction.immediate);
            printf("FU: %d\n", functional_unit);
            printf("-------------------------------------------------\n");
        }
    }
    return instruction;
}

/*Busca próxima instrução, marmazena em IR, decodifica e coloca na fila de instruções*/
void fetch_stage(){
    if (get_status_queue() == EMPTY){
        int instructions_fetched = 0;
        Instruction* instruction = (Instruction*)malloc(sizeof(Instruction));
        while(instructions_fetched < 4 && PC.data < finish_PC) {
            IR.data = read_mem(PC.data);
            instruction = decode(IR.data);
            insert_instruction(instruction);
            switch (instruction->opcode) {
                case REGIMM:
                    AR.data = PC.data;
                    PC.data = instruction->regimm_instruction.offset; //salto é tomado aqui
                    break;
                case J:
                    PC.data = instruction->j_instruction.target; //salto é tomado aqui
                    break;
                case I_BEQ:
                case I_BEQL:
                case I_BGTZ:
                case I_BLEZ:
                case I_BNE:
                    AR.data = PC.data;
                    PC.data = instruction->i_instruction.immediate; //salto é tomado aqui
                    break;
                default:
                    PC.data += 4;
            }
            ++instructions_fetched;
        }
    }
}

void execution_stage(){
    if (get_status_queue() == NOT_EMPTY || get_status_list() == NOT_EMPTY){
        scoreboarding();
    }
}

void write_back_stage(){

}