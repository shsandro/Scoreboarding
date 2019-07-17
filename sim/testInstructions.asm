ADDI $a1, $zero, 0xf
BGTZ $a1, ola2
AndI $a1, $t1, 0xf
beq $a1, $t1, ola
beql $a1, $t1, ola
beq $a1, $t1, ola
J ola
ola:
ori $a1, $t1, 0xf
XORI $a1, $t1, 0xf
ADD $a1, $a2, $a3
sub $a0, $gp, $k1
bgez $a1, ola2
ola2: