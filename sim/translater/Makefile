all:
	@bison -d translater.y
	@flex translater.l
	@cc util.c translater.tab.c lex.yy.c -lfl