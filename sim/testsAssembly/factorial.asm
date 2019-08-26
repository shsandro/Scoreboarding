addi $t0, $zero, 0x5
addi $t1, $zero, 0x1
addi $t2, $zero, 0x2
addi $t3, $zero, 0x0
beq $t0, $t1, end
beq $t0, $zero, end
loop:
mul $t1, $t2, $t1
addi $t2, $t2, 0x1
addi $t3, $t3, 0x1
bne $t3, $t0, loop
end: