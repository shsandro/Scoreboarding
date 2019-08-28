# Simulador
O diretório sim/  contém o código fonte do simulador. Dentro do diretório contém um Makefile para compilar o simulador. Após compilado, o executável estará nomeado como mips32sim.

Recomenda-se colocar os resultados da simulação no diretório ./output. Para isso, deverá ser passado como parâmetro -o ./output/<nome_programa>.s

Portanto, execute o programa como:
    ./mips32sim -i <caminho_programa>.asm -o ./output/<nome_programa>.s
