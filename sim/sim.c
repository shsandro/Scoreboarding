#include "memory.h"
#include "registers.h"

int num_instructions;

/*Cria arquivo inicial para teste de memória*/
void init(FILE* arq){
   int x = 15, y = 26465, z = 3649434;
   fwrite(&x, sizeof(int), 1, arq);
   fwrite(&y, sizeof(int), 1, arq);
   fwrite(&z, sizeof(int), 1, arq);
   fseek(arq, 0, SEEK_SET);
   num_instructions = 3;
}

int main(int argc, char **argv){
    FILE* instructions;
    instructions = fopen("instructions.txt", "wr+");
    init(instructions);
    load_memory(instructions, num_instructions);
    printf("%u\n%u\n%u\n", read_mem(0), read_mem(4), read_mem(8));
    fclose(instructions);
}