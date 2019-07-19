%{
#include "util.h"
#include <stdio.h>
#include "translater.tab.h"
#include "hash.h"
extern int second_pass;
extern int instructions_count;
%}

%%

"#".*.[\n]
"," { return COMMA;}
[\n]+ {return EOL;}

[B|b][G|g][E|e][Z|z] {yylval.value = 0b000001; return BGEZ_TOKEN;}
[B|b][L|l][T|t][Z|z] {yylval.value = 0b000000; return BLTZ_TOKEN;}

[A|a][D|d][D|d][I|i] {yylval.value = 0b001000; return ADDI_TOKEN;}
[A|a][N|n][D|d][I|i] {yylval.value = 0b001100; return ANDI_TOKEN;}
[B|b] {yylval.value = 0b000100; return B_TOKEN;}
[B|b][E|e][Q|q] {yylval.value = 0b000100; return BEQ_TOKEN;}
[B|b][E|e][Q|q][L|l] {yylval.value = 0b010100; return BEQL_TOKEN;}
[B|b][G|g][T|t][Z|z] {yylval.value = 0b000111;return BGTZ_TOKEN;}
[B|b][L|l][E|e][Z|z] {yylval.value = 0b000110;return BLEZ_TOKEN;}
[B|b][N|n][E|e] {yylval.value = 0b000101; printf("cheguei no bne\n"); return BNE_TOKEN;}
[L|l][U|u][I|i] {yylval.value = 0b001111; return LUI_TOKEN;}
[O|o][R|r][I|i] {yylval.value = 0b001101; return ORI_TOKEN;}
[X|x][O|o][R|r][I|i] {yylval.value = 0b001110; return XORI_TOKEN;}

[A|a][D|d][D|d] {yylval.value = 0b100000; return ADD_TOKEN;}
[A|a][N|n][D|d] {yylval.value = 0b100100; return AND_TOKEN;}
[D|d][I|i][V|v] {yylval.value = 0b011010; return DIV_TOKEN;}
[J|j][R|r] {yylval.value = 0b001001; return JR_TOKEN;}
[M|m][F|f][H|h][I|i] {yylval.value = 0b010000; return MFHI_TOKEN;}
[M|m][F|f][L|l][O|o] {yylval.value = 0b010010; return MFLO_TOKEN;}
[M|m][O|o][V|v][N|n] {yylval.value = 0b001011; return MOVN_TOKEN;}
[M|m][O|o][V|v][Z|z] {yylval.value = 0b001010; return MOVZ_TOKEN;}
[M|m][T|t][H|h][I|i] {yylval.value = 0b010001; return MTHI_TOKEN;}
[M|m][T|t][L|l][O|o] {yylval.value = 0b010011; return MTLO_TOKEN;}
[M|m][U|u][L|l][T|t] {yylval.value = 0b011000; return MULT_TOKEN;}
[N|n][O|o][P|p] {yylval.value = 0b000000; return NOP_TOKEN;}
[N|n][O|o][R|r] {yylval.value = 0b100111; return NOR_TOKEN;}
[O|o][R|r] {yylval.value = 0b100101; return OR_TOKEN;}
[S|s][U|u][B|b] {yylval.value = 0b100010; return SUB_TOKEN;}
[X|x][O|o][R|r] {yylval.value = 0b100110; return XOR_TOKEN;}

[M|m][A|a][D|d][D|d] {yylval.value = 0b000000; return MADD_TOKEN;}
[M|m][S|s][U|u][B|b] {yylval.value = 0b000100; return MSUB_TOKEN;}
[M|m][U|u][L|l] {yylval.value = 0b000010; return MUL_TOKEN;}

[J|j] {yylval.value = 0b000010; return JUMP_TOKEN;}

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

[a-zA-Z_][a-zA-Z0-9_]* { if(second_pass){yylval.value = get_data(strdup(yytext)); printf("Endereço: %d\n", yylval.value); return LABEL; }else{yylval.value = 0; return LABEL;}}
[a-zA-Z_][a-zA-Z0-9_]*[:] { if(!second_pass){insert_label(strdup(yytext), (instructions_count)*4);printf("achei label %s\n", yytext);} return LABEL;}
0[xX][0-9a-fA-F]+ { yylval.value = (int)strtol(yytext, NULL, 0); return IMMEDIATE; }

[ \t]

. { printf("Mystery character %c\n", *yytext); }

%%
