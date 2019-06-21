#include"cpu.h"
#include "registers.h"
#include "memory.h"

typedef struct{
    unsigned x: 6; // 10 bits
}Opcode;

/*Busca próxima instrução e marmazena em IR*/
void fetch_stage(){
    IR.data = read_mem(PC.data);
    PC.data += 4;
}

void execution_stage(){
    int instrucition_type = (IR.data >> 26) & MASK_TYPE;
    Instruction *instruction = (Instruction*)malloc(sizeof(Instruction));
    instruction->opcode = instrucition_type;
    switch (instrucition_type) {
        case SPECIAL:
        {
            instruction->r_instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction->r_instruction.rt = (IR.data >> 16) & MASK_REGISTER;
            instruction->r_instruction.rd = (IR.data >> 11) & MASK_REGISTER;
            instruction->r_instruction.shamt = (IR.data >> 6) & MASK_REGISTER;
            instruction->r_instruction.funct = IR.data & MASK_TYPE;
            printf("SPECIAL: ");
            printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
        }
            break;
        case SPECIAL2:
        {
            instruction->r_instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction->r_instruction.rt = (IR.data >> 16) & MASK_REGISTER;
            instruction->r_instruction.rd = (IR.data >> 11) & MASK_REGISTER;
            instruction->r_instruction.shamt = (IR.data >> 6) & MASK_REGISTER;
            instruction->r_instruction.funct = IR.data & MASK_TYPE;
            printf("SPECIAL2: ");
            printf("%d %d %d %d %d\n", instruction->opcode, instruction->r_instruction.rs, instruction->r_instruction.rt, instruction->r_instruction.rd, instruction->r_instruction.funct);
        }
            break;
        case REGIMM:
        {
            instruction->regimm_instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction->regimm_instruction.funct = (IR.data >> 16) & MASK_REGISTER;
            instruction->regimm_instruction.offset = IR.data & MASK_OFFSET;
            printf("REGIMM: ");
            printf("%d %d %d %d\n", instruction->opcode, instruction->regimm_instruction.rs, instruction->regimm_instruction.funct, instruction->regimm_instruction.offset);
        }
            break;
        case J:
        {
            instruction->j_instruction.target = IR.data & MASK_TARGET;
            printf("J: ");
            printf("%d %d\n", instruction->opcode, instruction->j_instruction.target);
        }
            break;
        default:
        {
            instruction->i_instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction->i_instruction.rt = (IR.data >> 16) & MASK_REGISTER;
            instruction->i_instruction.immediate = IR.data & MASK_OFFSET;
            printf("IMMEDIATE: ");
            printf("%d %d %d %d\n", instruction->opcode, instruction->i_instruction.rs, instruction->i_instruction.rt, instruction->i_instruction.immediate);
        }
    }
}