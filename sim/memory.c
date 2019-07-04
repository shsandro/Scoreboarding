#include "memory.h"
int num_instructions;
int finish_PC;

/*Carrega todas as instruções à memória*/
void load_memory(FILE* instructions){
    memory.vector = (Byte*) malloc(MEMORY_SIZE * sizeof(Byte));
    int word, address = 0;
    while(fread(&word, sizeof(int), 1, instructions) == 1){
        address = write_mem(word, address);
        num_instructions++;
    }
    finish_PC = num_instructions*4;
}

/*Escreve uma palavra em um edereço específico da memória*/
int write_mem(Word w, int address){
    Byte *ptr = (Byte*) &w;
    int position, i = 0;

    for(position = address; position < address + WORD_SIZE; position++){
        memory.vector[position] = ptr[i++];
    }

    return position;
}

/*Retorna um palavra lida de um endereço específico da memória*/
Word read_mem(int address){
    return *((Word*)(memory.vector + address));
}