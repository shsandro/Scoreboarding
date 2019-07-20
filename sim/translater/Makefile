all:
	@bison -d translater.y
	@flex translater.l
	@gcc -w util.c hash.c translater.tab.c lex.yy.c -lfl
	@echo "Sucesso! Tradutor compilado!"