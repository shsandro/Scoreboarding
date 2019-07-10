    # Implementar um tradutor de Assembly para binário


## Conjunto de instruções
- ADDI
    - Formato: ADDI rt, rs, immediate
    - Tradução 001000 rs rt immediate
    - Final: 001000 XXXXX XXXXX XXXXXXXXXXXXXXXX
    - Significado: rt = rs + immediate

- ANDI
    - Formato: ANDI rt, rs, immediate
    - Tradução: 001100 rs rt immediate
    - Final: 001100 XXXXX XXXXX XXXXXXXXXXXXXXXX
    - Significado: rt = rs & immediate

- B (Na verdade realiza BEQ $r0, $r0, offset)
    - Formato: B offset
    - Tradução: 000100 00000 00000 offset
    - Final: 000100 00000 00000 XXXXXXXXXXXXXXXX
    - Significado: PC += offset

- BEQ
    - Formato: BEQ rs, rt, offset
    - Tradução: 000100 rs rt offset
    - Final: 000100 XXXXX XXXXX XXXXXXXXXXXXXXXX
    - Significado: IF rs = rt PC += offset

- BEQL
    - Formato: BEQL rs, rt, offset
    - Tradução: 010100 rs rt offset
    - Final: 010100 XXXXX XXXXX XXXXXXXXXXXXXXXX
    - Significado: IF rs = rt PC += offset apenas se o salto é tomado

- BGTZ
    - Formato: BGTZ rs, offset
    - Tradução: 000111 rs 00000 offset
    - Final: 000111 XXXXX 00000 XXXXXXXXXXXXXXXX
    - Significado: IF rs > 0  PC += offset

- BLEZ
    - Formato: BLEZ rs, offset
    - Tradução: 000110 rs 00000 offset
    - Final: 000110 XXXXX 00000 XXXXXXXXXXXXXXXX
    - Significado: IF rs <= 0  PC += offset

- BNE
    - Formato: BNE rs, rt, offset
    - Tradução: 000101 rs rt offset
    - Final: 000101 XXXXX XXXXX XXXXXXXXXXXXXXXX
    - Significado: IF rs != rt PC += offset

- J
    - Formato: J target
    - Tradução: 000010 instr_index
    - Final: 000010 XXXXXXXXXXXXXXXXXXXXXXXXXX
    - Significado: PC = target

- LUI
    - Formato: LUI rt, immediate
    - Tradução: 001111 00000 rt immediate
    - Final: 001111 00000 XXXXX XXXXXXXXXXXXXXXX
    - Significado: rt = immediate << 16

- ORI
    - Formato: ORI rt, rs, immediate
    - Tradução: 001101 rs rt immediate
    - Final: 001101 XXXXX XXXXX XXXXXXXXXXXXXXXX
    - Significado: rt = rs | zero_extend(immediate)

- XORI
    - Formato: XOR rs, rt, immediate
    - Tradução: 001110 rs rt immediate
    - Final: 001110 XXXXX XXXXX XXXXXXXXXXXXXXXX
     - Significado: rt = rs XOR zero_extend(immediate)

## Instruções com regimm (000001)
- BGEZ
    - Formato: BGEZ rs, offset
    - Tradução: REGIMM rs 00001 offset
    - Final: 000001 XXXXX 00001 XXXXXXXXXXXXXXXX
    - Significado: IF rs >= 0  PC += offset

- BLTZ
    - Formato: BLTZ rs, offset
    - Tradução: 000001 rs 00000 offset
    - Final: 000001 XXXXX 00000 XXXXXXXXXXXXXXXX
    - Significado: IF rs < 0  PC += offset

## Instruções com special (000000)
- ADD
    - Formato: ADD rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 100000
    - Final: 000000 XXXXX XXXXX XXXXX 00000 100000
    - Significado: rd = rs + rt

- AND
    - Formato: AND rd, rs, rt
    - Tradução 000000 rs rt rd 00000 100100
    - Final: 000000 XXXXX XXXXX XXXXX 00000 100100
    - Significado: rd = rs & rt

- DIV
    - Formato: DIV rs, rt
    - Tradução: 000000 rs rt 00 0000 0000 011010 (SPECIAL rs rt HI|LO DIV)
    - Final: 000000 XXXXX XXXXX 0000000000 011010
    - Significado: Lo = rs / rt Hi = rs mod rt

- JR
    - Formato: JR rs
    - Tradução 000000 rs 00000 00000 hint 001001
    - Final: 000000 XXXXX 00000 00000 00000 001001
    - Significado: PC = rs

- MFHI
    - Formato: MFHI rd
    - Tradução: 000000 0 rd 0 MFHI (SPECIAL HI|LO rd 0 MFHI)
    - Final: 000000 0000000000 XXXXX 00000 010000
    - Significado: rd = Hi

- MFLO
    - Formato: MFLO rd
    - Tradução: 000000 0 rd 00000 MFLO (SPECIAL HI|LO rd 0 MFLO)
    - Final: 000000 0000000000 XXXXX 00000 010010
    - Significado: rd = Lo

- MOVN
    - Formato: MOVN rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 001011
    - Final: 000000 XXXXX XXXXX XXXXX 00000 001011
    - Significado: IF rt != 0 rd = rs

- MOVZ
    - Formato: MOVZ rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 001010
    - Final: 000000 XXXXX XXXXX XXXXX 00000 001010
    - Significado: IF rt = 0 rd = rs

- MTHI
    - Formato: MTHI rs
    - Tradução: 000000 rs 000000000000000 010001 (SPECIAL rs 0 MTHI)
    - Final: 000000 XXXXX 000000000000000 010001
    - Significado: Hi = rs

- MTLO
    - Formato: MTLO rs
    - Tradução: 000000 rs 000000000000000 010011 (SPECIAL rs 0 MTLO)
    - Final: 000000 XXXXX 000000000000000 010011
    - Significado: Lo = rs

- MULT
    - Formato: MULT rs, rt
    - Tradução: 000000 rs rt 0000000000 011000 (SPECIAL rs rt HI|LO MULT)
    - Final: 000000 XXXXX XXXXX 0000000000 011000
    - Significado:  (Hi, Lo) = rs x rt

- NOP
    - Formato: NOP
    - Tradução 0
    - Final: 00000000000000000000000000000000

- NOR
    - Formato: NOR rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 100111
    - Final: 000000 XXXXX XXXXX XXXXX 00000 100111
    - Significado: rd = ~(rs | rt)

- OR
    - Formato: OR rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 100101
    - Final: 000000 XXXXX XXXXX XXXXX 00000 100101
    - Significado: rd = rs | rt
- SUB
    - Formato: SUB rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 100010
    - Final: 000000 XXXXX XXXXX XXXXX 00000 100010
    - Significado: rd = rs - rt

- XOR
    - Formato: XOR rd, rs, rt
    - Tradução: 000000 rs rt rd 00000 100110
    - Final: 000000 XXXXX XXXXX XXXXX 00000 100110
    - Significado:  rd = rs XOR rt

## Instruções com special2 (011100)
- MADD
    - Formato: MADD rs, rt
    - Tradução: 011100 rs rt 0 0 MADD (SPECIAL2 rs rt HI|LO MADD)
    - Final: 011100 XXXXX XXXXX 00000 00000 000000
    - Significado: (Hi, Lo) = (Hi, Lo) + rs x rt

- MSUB
    - Formato: MSUB rs, rt
    - Tradução: 011100 rs rt 00000 00000 000100
    - Final: 011100 XXXXX XXXXX 00000 00000 000100
    - Significado: (Hi, Lo) = (Hi, Lo) - rs x rt

- MUL
    - Formato: MUL rd, rs, rt
    - Tradução: 011100 rs rt rd 00000 000010
    - Final: 011100 XXXXX XXXXX XXXXX 00000 000010
    - Significado: rd = rs x rt