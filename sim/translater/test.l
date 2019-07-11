%{
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
%}

%%

"#".*.[\n]
"," {}
[\n]+ {}

[A-a][D-d][D-d][I-i] {printf("instrução: %s\n", yytext); }
[A-a][N-n][D-d][I-i] {printf("instrução: %s\n", yytext); }
[B-b] {printf("instrução: %s\n", yytext); }
[B-b][E-e][Q-q] {printf("instrução: %s\n", yytext); }
[B-b][E-e][Q-q][L-l] {printf("instrução: %s\n", yytext); }
[B-b][G-g][T-t][Z-z] {printf("instrução: %s\n", yytext); }
[B-b][L-l][E-e][Z-z] {printf("instrução: %s\n", yytext); }
[B-b][N-n][E-e] {printf("instrução: %s\n", yytext); }
[L-l][U-u][I-i] {printf("instrução: %s\n", yytext); }
[O-o][R-r][I-i] {printf("instrução: %s\n", yytext); }
[X-x][O-o][R-r][I-i] {printf("instrução: %s\n", yytext); }

[A-a][D-d][D-d] {printf("instrução: %s\n", yytext); }
[A-a][N-n][D-d] {printf("instrução: %s\n", yytext); }
[D-d][I-i][V-v] {printf("instrução: %s\n", yytext); }
[J-j][R-r] {printf("instrução: %s\n", yytext); }
[M-m][F-f][H-h][I-i] {printf("instrução: %s\n", yytext); }
[M-m][F-f][L-l][O-o] {printf("instrução: %s\n", yytext); }
[M-m][O-o][V-v][N-n] {printf("instrução: %s\n", yytext); }
[M-m][O-o][V-v][Z-z] {printf("instrução: %s\n", yytext); }
[M-m][T-t][H-h][I-i] {printf("instrução: %s\n", yytext); }
[M-m][T-t][L-l][O-o] {printf("instrução: %s\n", yytext); }
[M-m][U-u][L-l][T-t] {printf("instrução: %s\n", yytext); }
[N-n][O-o][P-p] {printf("instrução: %s\n", yytext); }
[N-n][O-o][R-r] {printf("instrução: %s\n", yytext); }
[O-o][R-r] {printf("instrução: %s\n", yytext); }
[S-s][U-u][B-b] {printf("instrução: %s\n", yytext); }
[X-x][O-o][R-r] {printf("instrução: %s\n", yytext); }

[M-m][A-a][D-d] {printf("instrução: %s\n", yytext); }
[M-m][S-s][U-u][B-b] {printf("instrução: %s\n", yytext); }
[M-m][U-u][L-l] {printf("instrução: %s\n", yytext); }

[B-b][G-g][E-e][Z-z] {printf("instrução: %s\n", yytext); }
[B-b][L-l][T-t][Z-z] {printf("instrução: %s\n", yytext); }

[J-j] {printf("instrução: %s\n", yytext); }

0[xX][0-9a-fA-F]+ { printf("número: %d\n", (int)strtol(yytext, NULL, 0)); }
[a-zA-Z_][a-zA-Z0-9_]* { printf("label: %s", yytext); }
[ \t]


"$zero" {printf("registrador: %s\n", yytext); }
"$at" { printf("registrador: %s\n", yytext);  }
"$"[v][0-1] { int value =  get_register(yytext[1], yytext[2]); printf("registrador: %d\n", value);  }
"$"[a][0-3] { int value =  get_register(yytext[1], yytext[2]); printf("registrador: %d\n", value);  }
"$"[t][0-9] { int value =  get_register(yytext[1], yytext[2]); printf("registrador: %d\n", value);  }
"$"[s][0-7] { int value =  get_register(yytext[1], yytext[2]); printf("registrador: %d\n", value);  }
"$"[k][0-1] { int value =  get_register(yytext[1], yytext[2]); printf("registrador: %d\n", value);  }
"$gp" { printf("registrador: %s\n", yytext);  }
"$sp" { printf("registrador: %s\n", yytext);  }
"$fp" { printf("registrador: %s\n", yytext);  }
"$ra" { printf("registrador: %s\n", yytext);  }

%%

main(int argc, char **argv)
{
    FILE *myfile = fopen("testInstructions.txt", "r");
    yyin = myfile;
    while(yylex());
}
