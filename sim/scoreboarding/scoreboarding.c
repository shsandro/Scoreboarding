#include "scoreboarding.h"

int ISSUED = false;

int get_functional_unit(int operation){
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

void scoreboarding(Instruction* instruction, int data){
    int instrucition_type = (data >> 26) & MASK_TYPE;
    instruction->opcode = instrucition_type;
    int functional_unit;

    switch (instrucition_type) {
        case SPECIAL:
        {
            instruction->r_instruction.rs = (data >> 21) & MASK_REGISTER;
            instruction->r_instruction.rt = (data >> 16) & MASK_REGISTER;
            instruction->r_instruction.rd = (data >> 11) & MASK_REGISTER;
            instruction->r_instruction.shamt = (data >> 6) & MASK_REGISTER;
            instruction->r_instruction.funct = data & MASK_TYPE;
            printf("SPECIAL: ");
            printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
        }
            break;
        case SPECIAL2:
        {
            instruction->r_instruction.rs = (data >> 21) & MASK_REGISTER;
            instruction->r_instruction.rt = (data >> 16) & MASK_REGISTER;
            instruction->r_instruction.rd = (data >> 11) & MASK_REGISTER;
            instruction->r_instruction.shamt = (data >> 6) & MASK_REGISTER;
            instruction->r_instruction.funct = data & MASK_TYPE;
            printf("SPECIAL2: ");
            printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
        }
            break;
        case REGIMM:
        {
            instruction->regimm_instruction.rs = (data >> 21) & MASK_REGISTER;
            instruction->regimm_instruction.funct = (data >> 16) & MASK_REGISTER;
            instruction->regimm_instruction.offset = data & MASK_OFFSET;
            printf("REGIMM: ");
            printf("%d %d %d %d\n", instruction->opcode, instruction->regimm_instruction.rs, instruction->regimm_instruction.funct, instruction->regimm_instruction.offset);
        }
            break;
        case J:
        {
            instruction->j_instruction.target = data & MASK_TARGET;
            printf("J: ");
            printf("%d %d\n", instruction->opcode, instruction->j_instruction.target);
        }
            break;
        default:
        {
            instruction->i_instruction.rs = (data >> 21) & MASK_REGISTER;
            instruction->i_instruction.rt = (data >> 16) & MASK_REGISTER;
            instruction->i_instruction.immediate = data & MASK_OFFSET;
            printf("IMMEDIATE: ");
            printf("%d %d %d %d\n", instruction->opcode, instruction->i_instruction.rs, instruction->i_instruction.rt, instruction->i_instruction.immediate);
        }
    }
}