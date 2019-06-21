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
    switch (instrucition_type) {
        case SPECIAL:
        {
            R_Instruction instruction;
            instruction.opcode = instrucition_type;
            instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction.rt = (IR.data >> 16) & MASK_REGISTER;
            instruction.rd = (IR.data >> 11) & MASK_REGISTER;
            instruction.shamt = (IR.data >> 6) & MASK_REGISTER;
            instruction.funct = IR.data & MASK_TYPE;
            printf("SPECIAL: ");
            printf("%d %d %d %d %d\n", instruction.opcode, instruction.rs, instruction.rt, instruction.rd, instruction.funct);
        }
            break;
        case SPECIAL2:
        {
            R_Instruction instruction;
            instruction.opcode = instrucition_type;
            instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction.rt = (IR.data >> 16) & MASK_REGISTER;
            instruction.rd = (IR.data >> 11) & MASK_REGISTER;
            instruction.shamt = (IR.data >> 6) & MASK_REGISTER;
            instruction.funct = IR.data & MASK_TYPE;
            printf("SPECIAL2: ");
            printf("%d %d %d %d %d\n", instruction.opcode, instruction.rs, instruction.rt, instruction.rd, instruction.funct);
        }
            break;
        case REGIMM:
        {
            REGIMM_Instruction instruction;
            instruction.opcode = instrucition_type;
            instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction.funct = (IR.data >> 16) & MASK_REGISTER;
            instruction.offset = IR.data & MASK_OFFSET;
            printf("REGIMM: ");
            printf("%d %d %d %d\n", instruction.opcode, instruction.rs, instruction.funct, instruction.offset);
        }
            break;
        case J:
        {
            J_Instruction instruction;
            instruction.opcode = instrucition_type;
            instruction.target = IR.data & MASK_TARGET;
            printf("J: ");
            printf("%d %d\n", instruction.opcode, instruction.target);
        }
            break;
        default:
        {
            I_Instruction instruction;
            instruction.opcode = instrucition_type;
            instruction.rs = (IR.data >> 21) & MASK_REGISTER;
            instruction.rt = (IR.data >> 16) & MASK_REGISTER;
            instruction.immediate = IR.data & MASK_OFFSET;
            printf("IMMEDIATE: ");
            printf("%d %d %d %d\n", instruction.opcode, instruction.rs, instruction.rt, instruction.immediate);
        }
    }
}