#include "./include/sim.h"

int num_instructions = 0;
int finish_PC = 0;

/*Inicializa tudo o que é necessário para o simulador rodar*/
void initialize_simulator(){
   output = fopen("output.bin", "r");
   load_memory(output);
   initialize_registers();
   initialize_queue();
   initialize_functional_units();
   initialize_list();
   initialize_clock();
   fclose(output);
}

/*Printa o conteúdo de todos os registradores*/
void print_registers(){
   printf("\n");
   for (int i = 0; i < 34; ++i){
      printf("Register %d -> %d\t", i, registers[i].data);
   }
   printf("\n");
}

int main(int argc, char **argv){
   check_help(argc, argv);
   int run = check_run(argc, argv);
   if(run){
      translater(argv[run]);
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
   }
   int output = check_output(argc, argv);
   if(output){
      rename("output.bin", argv[output]);
      printf("%s\n", strtok(argv[output], "."));
   }
}