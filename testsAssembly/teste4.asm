addi $t0, $zero, 0x3    # i = 3
addi $ra, $zero, 0xa    # valor inicial de a
add $t5, $zero, $ra     # auxiliar para valor inicial de a
addi $t1, $zero, 0x0    #switch = 0
addi $t2, $zero, 0x1    #switch = 1
addi $t3, $zero, 0x2    #switch = 2
add $ra, $zero, $t1     # a = 0
beq $t0, $t1, end
add $ra, $zero, $t5     # a = valor inicial
beq $t0, $t2, end
add $ra, $zero, $t2     # a = 1
beq $t0, $t3, end
add $ra, $zero, $t3     # a = 2
end: