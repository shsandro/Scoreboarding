#include "sim.h"

int num_instructions = 0;
int finish_PC = 0;

/*Cria arquivo inicial para teste de memória*/
void init(FILE* arq){
   int x = 539099144, y = 6563874, z = 17825818, a = 1900765186, b = 807534600;
   /*x=addi 8 1 2 8  y=sub 0 3 4 5 34 z=div 0 8 16 26 a=mul 28 10 11 12 2 b=andi 12 1 2 8*/
   fwrite(&x, sizeof(int), 1, arq);
   fwrite(&b, sizeof(int), 1, arq);
   fwrite(&y, sizeof(int), 1, arq);
   fwrite(&z, sizeof(int), 1, arq);
   fwrite(&a, sizeof(int), 1, arq);
   fseek(arq, 0, SEEK_SET);
   // num_instructions = 5;
   // finish_PC = num_instructions*4;
}

int main(int argc, char **argv){
   FILE* instructions;
   instructions = fopen("instructions.txt", "wr+");
   init(instructions);
   load_memory(instructions);
   initialize_registers();
   initialize_queue();
   initialize_functional_units();
   //printf("%u\n%u\n%u\n", read_mem(0), read_mem(4), read_mem(8));
   for(int i = 0; i<=num_instructions; ++i){
      execution_stage();
      fetch_stage();
      // printf("\n");
      // for (int j = 0; j<6; j++){
      //    printf("%d->%d ",j, functional_units[j].operation);
      // }
   }
   fclose(instructions);
}