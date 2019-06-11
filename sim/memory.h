#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE 536870912 /*512M*/
#define WORD_SIZE 4 /*4 bytes*/

typedef unsigned char Byte;
typedef unsigned int Word;

/*Definição da estrutura de memória*/
typedef struct{
    Byte *vector;
}Memory;

/*Declaração da memória*/
Memory memory;

/*Assinatura das funções de memória*/
void load_memory(FILE* instructions, int num_instructions);
int write_mem(Word w, int address);
void next_mem();
Word read_mem(int address);

#endif