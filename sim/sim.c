#include "memory.h"
#include "registers.h"
#include "cpu.h"

int num_instructions;

/*Cria arquivo inicial para teste de memória*/
void init(FILE* arq){
   int x = 539099144, y = 34080804, z = 134217735, a = 100728840;
   /*x=addi 8 1 2 8  y=and 0 16 8 1 36 z=j 2 7 a=bgez 1 16 1 8*/
   fwrite(&x, sizeof(int), 1, arq);
   fwrite(&y, sizeof(int), 1, arq);
   fwrite(&z, sizeof(int), 1, arq);
   fwrite(&a, sizeof(int), 1, arq);
   fseek(arq, 0, SEEK_SET);
   num_instructions = 4;
}

int main(int argc, char **argv){
   FILE* instructions;
   instructions = fopen("instructions.txt", "wr+");
   init(instructions);
   load_memory(instructions, num_instructions);
   //printf("%u\n%u\n%u\n", read_mem(0), read_mem(4), read_mem(8));
   for(int i = 0; i<4; ++i){
      fetch_stage();
      execution_stage();
   }
   fclose(instructions);
}