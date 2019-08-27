#include "./include/sim.h"

int num_instructions = 0;
int finish_PC = 0;

/*Inicializa tudo o que é necessário para o simulador rodar*/
void initialize_simulator(){
   output = fopen("./output/output.bin", "r");
   load_memory(output);
   initialize_registers();
   initialize_queue();
   initialize_functional_units();
   initialize_list();
   initialize_clock();
   fclose(output);
}

/*Copia arquivo assembly para output.out*/
void copy_file(char* file_name, FILE* target){
   input_assembly = fopen(file_name, "r");
   
   char ch;

   fputc('\t', target);

   while ((ch = fgetc(input_assembly)) != EOF){
      if (ch == '\n'){
         fputc(ch, target);
         fputc('\t', target);
      } else fputc(ch, target);
      
   } 
   fclose(input_assembly);
}

/*Escreve o arquivo binário para output.out*/
void write_binary(){
   output = fopen("./output/output.bin", "r");
   int word;
   while(fread(&word, sizeof(int), 1, output) == 1) fprintf(out_stats, "\t%X\n", word);
   fclose(output);
}

/*Escreve no arquivo de saída previsão de desvio e demais informações*/
void write_stats(FILE *target){
   float branches_hit_percent = ((float)branches_hit/(float)branches_taken)*100;
   float branches_miss_percent = ((float)branches_miss/(float)branches_taken)*100;
   float instructions_issued_percent = ((float)instructions_written/(float)instructions_issued)*100;

   fprintf(target, "\nPrevisão:\n \tTotal de saltos: %d\n \tAcertos: %d (%.2f%)\n \tErros: %d (%.2f%)\n", branches_taken, branches_hit, branches_hit_percent, branches_miss, branches_miss_percent);
   fprintf(target, "\nCiclos:\n \t%d ciclos\n", get_clock());
   fprintf(target, "\nInstruções:\n \tEmitidas: %d\n \tEfetivadas: %d (%.2f%)\n", instructions_issued, instructions_written, instructions_issued_percent);
}

/*Escreve informações sobre os registradores no arquivo de saída*/
void write_registers(FILE* target){
   fprintf(target, "\nRegistradores:\n");
   int i = 0;
   while (i < 33) {
      fprintf(target, "\tRegister %d -> %d \tRegister %d -> %d \tRegister %d -> %d\n", i, registers[i].data, i+1, registers[i+1].data, i+2, registers[i+2].data);
      i += 3;
   }
   fprintf(target, "\tRegister %d -> %d", 33, registers[33].data);
}

/*Roda o pipeline*/
void run(char *argv){
   translater(argv);
   initialize_simulator();

   out_stats = fopen("./output/output.out", "w");
   fputs("Programa:\n", out_stats);
   copy_file(argv, out_stats);
   fputs("\n\nBinário:\n", out_stats);
   write_binary();
   
   do {
   execution_stage();
   fetch_stage();
   increase_clock();
   } while(get_status_queue() == NOT_EMPTY || get_status_list() == NOT_EMPTY);

   write_stats(out_stats);
   write_registers(out_stats);
   fclose(out_stats);

   printf("Programa executado com sucesso!\n");
}

/*Printa ajuda*/
void help(){
   printf("\n\n-i: arquivo assembly contendo programa a ser simulado\n-h | --help: ajuda\n-o: arquivo de saída contendo programa convertido para binário\n\nRODE './mips32sim -i <arquivo assembly> [-o <arquivo saída>] [-h]' PARA EXECUÇÃO COMPLETA.\n\n");
   printf("Caso o arquivo de saída não seja especificado, o arquivo binário estará em './output/output.bin' e o arquivo de insformações em './output/output.out'\n");
}

/*Verifica se possui arquivo de saída especificado*/
int check_output(int argc, char **argv){
    for (int i = 0; i < argc; ++i){
        if (!strcmp(argv[i], "-o")) return ++i;
    }
    return 0;
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
   int i = 1;

   while(i < argc){
      if(strcmp(argv[i], "-i") == 0) run(argv[++i]);
      else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) help();
      else if(strcmp(argv[i], "-o") == 0) ++i; //Verifico o arquivo de saída apenas depois
      else{
         printf("\nPARÂMETRO DESCONHECIDO <%s>\n", argv[i]);
         help();
         exit(EXIT_FAILURE);
      }
      ++i;
   }
  
   //Se um nome de arquivo de saída é passado, renomeio ele
   int output = check_output(argc, argv);
   if(output){
      rename("./output/output.bin", argv[output]);

      argv[output][(strlen(argv[output])-1)] = '\0';
      strcat(argv[output], "out");
      
      rename("./output/output.out", argv[output]);
   }
}