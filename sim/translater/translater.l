%{
#include "util.h"
#include <stdio.h>
%}

%%

"#".*.[\n]
"," { return COMMA;}
[\n]+ {return EOL;}

[A-a][D-d][D-d][I-i] {yylval.value = 0b001000; return ADDI;}
[A-a][N-n][D-d][I-i] {yylval.value = 0b001100; return ANDI;}
[B-b] {yylval.value = 0b000100; return B;}
[B-b][E-e][Q-q] {yylval.value = 0b000100; return BEQ;}
[B-b][E-e][Q-q][L-l] {yylval.value = 0b010100; return BEQL;}
[B-b][G-g][T-t][Z-z] {yylval.value = 0b000111; return BGTZ;}
[B-b][L-l][E-e][Z-z] {yylval.value = 0b000110; return BLEZ;}
[B-b][N-n][E-e] {yylval.value = 0b000101; return BNE;}
[L-l][U-u][I-i] {yylval.value = 0b001111; return LUI;}
[O-o][R-r][I-i] {yylval.value = 0b001101; return ORI;}
[X-x][O-o][R-r][I-i] {yylval.value = 0b001110; return XORI;}

[A-a][D-d][D-d] {yylval.value = 0b100000; return ADD;}
[A-a][N-n][D-d] {yylval.value = 0b100100; return AND;}
[D-d][I-i][V-v] {yylval.value = 0b011010; return DIV;}
[J-j][R-r] {yylval.value = 0b001001; return JR;}
[M-m][F-f][H-h][I-i] {yylval.value = 0b010000; return MFHI;}
[M-m][F-f][L-l][O-o] {yylval.value = 0b010010; return MFLO;}
[M-m][O-o][V-v][N-n] {yylval.value = 0b001011; return MOVN;}
[M-m][O-o][V-v][Z-z] {yylval.value = 0b001010; return MOVZ;}
[M-m][T-t][H-h][I-i] {yylval.value = 0b010001; return MTHI;}
[M-m][T-t][L-l][O-o] {yylval.value = 0b010011; return MTLO;}
[M-m][U-u][L-l][T-t] {yylval.value = 0b011000; return MULT;}
[N-n][O-o][P-p] {yylval.value = 0b000000; return NOP;}
[N-n][O-o][R-r] {yylval.value = 0b100111; return NOR;}
[O-o][R-r] {yylval.value = 0b100101; return OR;}
[S-s][U-u][B-b] {yylval.value = 0b100010; return SUB;}
[X-x][O-o][R-r] {yylval.value = 0b100110; return XOR;}

[M-m][A-a][D-d] {yylval.value = 0b000000; return MAD;}
[M-m][S-s][U-u][B-b] {yylval.value = 0b000100; return MSUB;}
[M-m][U-u][L-l] {yylval.value = 0b000010; return MUL;}

[B-b][G-g][E-e][Z-z] {yylval.value = 0b000001; return BGEZ;}
[B-b][L-l][T-t][Z-z] {yylval.value = 0b000000; return BLTZ;}

[J-j] {yylval.value = 0b000010; return J;}

"$zero" {yylval.value = 0; return REGISTER; }
"$at" { yylval.value = 1; return REGISTER; }
"$"[v][0-1] { yylval.value = get_register(yytext[1], yytext[2]); return REGISTER; }
"$"[a][0-3] { yylval.value = get_register(yytext[1], yytext[2]); return REGISTER; }
"$"[t][0-9] { yylval.value = get_register(yytext[1], yytext[2]); return REGISTER; }
"$"[s][0-7] { yylval.value = get_register(yytext[1], yytext[2]); return REGISTER; }
"$"[k][0-1] { yylval.value = get_register(yytext[1], yytext[2]); return REGISTER; }
"$gp" { yylval.value = 28; return REGISTER; }
"$sp" { yylval.value = 29; return REGISTER; }
"$fp" { yylval.value = 30; return REGISTER; }
"$ra" { yylval.value = 31; return REGISTER; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.str = strdup(yytext); return LABEL; }
0[xX][0-9a-fA-F]+ { yylval.value = (int)strtol(yytext, NULL, 0)); return IMMEDIATE; }

[ \t]

%%

main(int argc, char **argv)
{
  yylex();
}
