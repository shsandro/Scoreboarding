#include "sim.h"

int num_instructions = 0;
int finish_PC = 0;

void initialize_simulator(){
   output = fopen("output.bin", "r");
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
   printf("\n");
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
      translater(argv[2]);
      initialize_simulator();
      do {
      execution_stage();
      fetch_stage();
      increase_clock();
      print_registers();
      printf("\n Instruções emitidas => %d, instruções efetivadas => %d", instructions_issued, instructions_written);
      printf("\n Total saltos => %d, saltos acertados => %d, saltos errados => %d", branches_taken, branches_hit, branches_miss);
      printf("\nCiclo: %d PC: %d\n", get_clock(), PC.data);
      // getchar();
      } while(get_status_queue() == NOT_EMPTY || get_status_list() == NOT_EMPTY);

   } else{
      printf("Por favor, rode simulação completa.\n");
   }
}