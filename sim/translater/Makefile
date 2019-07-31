CXX=gcc
CXXFLAGS=-W

all:
	@bison -d translater.y
	@flex translater.l
	gcc -W -g lex.yy.c -lfl util.c hash.c translater.tab.c
	@echo "Sucesso! Tradutor compilado!"