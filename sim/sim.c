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

// char *str_replace(char *orig, char *rep, char *with) {
//     char *result; // the return string
//     char *ins;    // the next insert point
//     char *tmp;    // varies
//     int len_rep;  // length of rep (the string to remove)
//     int len_with; // length of with (the string to replace rep with)
//     int len_front; // distance between rep and end of last rep
//     int count;    // number of replacements

//     // sanity checks and initialization
//     if (!orig || !rep)
//         return NULL;
//     len_rep = strlen(rep);
//     if (len_rep == 0)
//         return NULL; // empty rep causes infinite loop during count
//     if (!with)
//         with = "";
//     len_with = strlen(with);

//     // count the number of replacements needed
//     ins = orig;
//     for (count = 0; tmp = strstr(ins, rep); ++count) {
//         ins = tmp + len_rep;
//     }

//     tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

//     if (!result)
//         return NULL;

//     // first time through the loop, all the variable are set correctly
//     // from here on,
//     //    tmp points to the end of the result string
//     //    ins points to the next occurrence of rep in orig
//     //    orig points to the remainder of orig after "end of rep"
//     while (count--) {
//         ins = strstr(orig, rep);
//         len_front = ins - orig;
//         tmp = strncpy(tmp, orig, len_front) + len_front;
//         tmp = strcpy(tmp, with) + len_with;
//         orig += len_front + len_rep; // move to next "end of rep"
//     }
//     strcpy(tmp, orig);
//     return result;
// }

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
   print_registers();
   printf("\n Instruções emitidas => %d, instruções efetivadas => %d", instructions_issued, instructions_written);
   printf("\n Total saltos => %d, saltos acertados => %d, saltos errados => %d", branches_taken, branches_hit, branches_miss);
   printf("\nCiclo: %d PC: %d\n", get_clock(), PC.data);
   // getchar();
   } while(get_status_queue() == NOT_EMPTY || get_status_list() == NOT_EMPTY);

   float branches_hit_percent = ((float)branches_hit/(float)branches_taken)*100;
   float branches_miss_percent = ((float)branches_miss/(float)branches_taken)*100;
   float instructions_issued_percent = ((float)instructions_written/(float)instructions_issued)*100;

   fprintf(out_stats, "\nPrevisão:\n \tTotal de saltos: %d\n \tAcertos: %d (%.2f%)\n \tErros: %d (%.2f%)\n", branches_taken, branches_hit, branches_hit_percent, branches_miss, branches_miss_percent);
   fprintf(out_stats, "\nCiclos:\n \t%d ciclos\n", get_clock());
   fprintf(out_stats, "\nInstruções:\n \tEmitidas: %d\n \tEfetivadas: %d (%.2f%)\n", instructions_issued, instructions_written, instructions_issued_percent);
   fclose(out_stats);
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
         printf("\nPARÂMETRO DESCONHECIDO.\n");
         help();
         exit(EXIT_FAILURE);
      }
      ++i;
   }
  
   //Se um nome de arquivo de saída é passado, renomeio ele
   int output = check_output(argc, argv);
   if(output){
      // char new_name[80];
      // strcpy(new_name, "./output/");
      // strcat(new_name, argv[output]);
      rename("./output/output.bin", argv[output]);

      argv[output][(strlen(argv[output])-1)] = '\0';
      strcat(argv[output], "out");
      
      rename("./output/output.out", argv[output]);
   }
}