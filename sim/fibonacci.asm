
addi $t0, $zero, 0x4
addi $t1, $zero, 0x0
add $a0, $zero, $zero
add $a1, $zero, $zero
addi $a2, $zero, 0x1
beq $t0, $zero, end
loop:
addi $t1, $t1, 0x1
add $a0, $a1, $a2
addi $a1, $a2, 0x0
addi $a2, $a0, 0x0
bne $t0, $t1, loop
end: