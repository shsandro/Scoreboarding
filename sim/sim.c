#include "sim.h"

int num_instructions = 0;
int finish_PC = 0;

/*Cria arquivo inicial para teste de memória*/
void init(FILE* arq){
   int x = 539099144, y = 6563874, z = 17825818, a = 1900765186, b = 807534600, j = 134217748;
   /*x=addi 8 1 2 8  y=sub 0 3 4 5 34 z=div 0 8 16 26 a=mul 28 10 11 12 2 b=andi 12 1 2 8*/
   fwrite(&x, sizeof(int), 1, arq);
   fwrite(&j, sizeof(int), 1, arq);
   fwrite(&b, sizeof(int), 1, arq);
   fwrite(&y, sizeof(int), 1, arq);
   fwrite(&z, sizeof(int), 1, arq);
   fwrite(&a, sizeof(int), 1, arq);
   fseek(arq, 0, SEEK_SET);
}

int main(int argc, char **argv){
   FILE* instructions;
   char file[100];
   printf("Digite o nome do arquivo de instruções: ");
	fgets(file, 99, stdin);
	file[strlen(file)-1] = '\0';
   instructions = fopen(file, "r");
   //init(instructions);
   load_memory(instructions);
   initialize_registers();
   initialize_queue();
   initialize_functional_units();
   initialize_list();

   do {
       execution_stage();
      fetch_stage();
      increase_clock();
      printf("Ciclo: %d\n", get_clock());
   } while(get_status_queue() == NOT_EMPTY || get_status_list() == NOT_EMPTY);
     
   fclose(instructions);
}