/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_TRANSLATER_TAB_H_INCLUDED
# define YY_YY_TRANSLATER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ADDI_TOKEN = 258,
    ANDI_TOKEN = 259,
    B_TOKEN = 260,
    BEQ_TOKEN = 261,
    BEQL_TOKEN = 262,
    BGTZ_TOKEN = 263,
    BLEZ_TOKEN = 264,
    BNE_TOKEN = 265,
    LUI_TOKEN = 266,
    ORI_TOKEN = 267,
    XORI_TOKEN = 268,
    JUMP_TOKEN = 269,
    ADD_TOKEN = 270,
    AND_TOKEN = 271,
    DIV_TOKEN = 272,
    JR_TOKEN = 273,
    MFHI_TOKEN = 274,
    MFLO_TOKEN = 275,
    MOVN_TOKEN = 276,
    MOVZ_TOKEN = 277,
    MTHI_TOKEN = 278,
    MTLO_TOKEN = 279,
    MULT_TOKEN = 280,
    NOP_TOKEN = 281,
    NOR_TOKEN = 282,
    OR_TOKEN = 283,
    SUB_TOKEN = 284,
    XOR_TOKEN = 285,
    MADD_TOKEN = 286,
    MSUB_TOKEN = 287,
    MUL_TOKEN = 288,
    BGEZ_TOKEN = 289,
    BLTZ_TOKEN = 290,
    REGISTER = 291,
    IMMEDIATE = 292,
    LABEL = 293,
    COMMA = 294,
    EOL = 295
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "translater.y" /* yacc.c:1921  */

  struct R{
    int opcode, rd, rs, rt, shamt, funct;
  }R_Instruction;
  struct I{
    int opcode, rs, rt, immediate;
  }I_instrucition;
  struct REGIMM_R{
    int opcode, rs, funct, offset;
  }REGIMM_Instruction;
  struct JUMP{
    int opcode, target;
  }J_Instruction;
  int value;
  char *str;

#line 116 "translater.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TRANSLATER_TAB_H_INCLUDED  */
