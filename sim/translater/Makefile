all:
	@bison -d translater.y
	@flex translater.l
	@gcc util.c hash.c translater.tab.c lex.yy.c -lfl