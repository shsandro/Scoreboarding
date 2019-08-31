# Simulador MIPS32

Simulador de arquitetura MIPS32 desenvolvido como trabalho da disciplina de Arquitetura e Organização de Computadores II.

## Dependências

As dependêcias do simulador são:
- `flex 2.6.1`
- `bison 3.0.4`

## Funcionamento

### Instruções

O simulador suporta a execução de 33 instruções MIPS32:

| ADDI | ANDI | B    | BEQ  | BEQL | BGTZ |
|------|------|------|------|------|------|
| BLEZ | BNE  | J    | LUI  | ORI  | XORI |
| BGEZ | BLTZ | ADD  | AND  | DIV  | JR   |
| MFHI | MFLO | MOVN | MOVZ | MTHI | MTLO |
| MULT | NOP  | NOR  | OR   | SUB  | XOR  |
| MADD | MSUB | MUL  |      |      |      |

Uma especificação detalhada das instruções pode ser encontrada por meio do caminho `./sim/instructions.md`.

### Execução

O simulador deve ser executado seguindo os seguintes passos:

1) `cd sim/`
2) `make`
3) `./mips32sim <parametros>`

É possível passar três parâmetros para o simulador:
- `-i`: caminho do arquivo assembly MIPS32 a ser executado
- `-o`: caminho para a saída do simulador
- `-h`: ajuda

Recomenda-se colocar os resultados da simulação no diretório `./sim/output`. Para isso, deverá ser passado como parâmetro `-o ./output/<nome_programa>.s`.

Exemplo:

`./mips32sim -i <caminho_programa>.asm -o ./output/<nome_programa>.s`


