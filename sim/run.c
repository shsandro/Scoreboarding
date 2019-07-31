#include "./include/run.h"

void check_help(int argc, char **argv){
    for (int i = 0; i < argc; ++i){
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")){
            printf("\n\n-i: arquivo assembly contendo programa a ser simulado\n-h | --help: ajuda\n-o: arquivo de saída contendo programa convertido para binário\n\nRODE './mips32sim -i <arquivo assembly> [-o <arquivo saída>] [-h]' PARA EXECUÇÃO COMPLETA.\n\n");
            printf("Caso o arquivo de saída não seja especificado, o arquivo binário estará em 'output.bin'\n");
        }
    }
}

int check_run(int argc, char **argv){
    for (int i = 0; i < argc; ++i){
        if (!strcmp(argv[i], "-i")) return ++i;
    }
    return 0;
}

int check_output(int argc, char **argv){
    for (int i = 0; i < argc; ++i){
        if (!strcmp(argv[i], "-o")) return ++i;
    }
    return 0;
}