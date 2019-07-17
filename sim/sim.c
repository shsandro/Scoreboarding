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

void initialize_simulator(){
   output = fopen("instructions.txt", "r");
   load_memory(output);
   initialize_registers();
   initialize_queue();
   initialize_functional_units();
   initialize_list();
   fclose(output);
}

void print_registers(){
   printf("\n");
   for (int i = 0; i < 34; ++i){
      printf("Register %d -> %d\t", i, registers[i].data);
   }
}

int main(int argc, char **argv){
   if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")){
      printf("\n\n -t: traduzir arquivo assembly\n -r | --run: executa o simulador a partir do arquivo traduzido\n\n RODE './sim -t <arquivo assembly> --run' PARA EXECUÇÃO COMPLETA.\n\n");
      exit(EXIT_FAILURE);
   }
   if(argc < 4){
      printf("Por favor, rode simulação completa.\n");
      exit(EXIT_FAILURE);
   } else if (!strcmp(argv[1], "-t") && (!strcmp(argv[3], "-r") || !strcmp(argv[3], "--run"))){
      printf("cai no if\n");
      translater(argv[2]);
      initialize_simulator();
      do {
      execution_stage();
      fetch_stage();
      increase_clock();
      print_registers();
      printf("\nCiclo: %d PC: %d\n", get_clock(), PC.data);
      getchar();
      } while(get_status_queue() == NOT_EMPTY || get_status_list() == NOT_EMPTY);

   } else{
      printf("Por favor, rode simulação completa.\n");
   }
}