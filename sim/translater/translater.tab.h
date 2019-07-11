/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
    ADDI = 258,
    ANDI = 259,
    B = 260,
    BEQ = 261,
    BEQL = 262,
    BGTZ = 263,
    BLEZ = 264,
    BNE = 265,
    LUI = 266,
    ORI = 267,
    XORI = 268,
    J = 269,
    ADD = 270,
    AND = 271,
    DIV = 272,
    JR = 273,
    MFHI = 274,
    MFLO = 275,
    MOVN = 276,
    MOVZ = 277,
    MTHI = 278,
    MTLO = 279,
    MULT = 280,
    NOP = 281,
    NOR = 282,
    OR = 283,
    SUB = 284,
    XOR = 285,
    MADD = 286,
    MSUB = 287,
    MUL = 288,
    BGEZ = 289,
    BLTZ = 290,
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
#line 13 "translater.y" /* yacc.c:1909  */

  typedef struct{
    int opcode, rd, rs, rt, shamt, funct;
  }R_Instruction;

  typedef struct{
    int opcode, rs, rt, immediate;
  }I_instrucition;

  typedef struct{
    int opcode, rs, funct, offset;
  }REGIMM_Instruction;

  typedef struct{
    int opcode, target;
  }J_Instruction;

  int value;
  char *str;

#line 116 "translater.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TRANSLATER_TAB_H_INCLUDED  */
