#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE 536870912 /*512M*/
#define WORD_SIZE 4 /*4 bytes*/

typedef unsigned char Byte;
typedef unsigned int Word;
extern int num_instructions;
extern int finish_PC;

/*Definição da estrutura de memória*/
typedef struct{
    Byte *vector;
}Memory;

/*Declaração da memória*/
Memory memory;

void load_memory(FILE* instructions);
int write_mem(Word w, int address);
void next_mem();
Word read_mem(int address);

#endif