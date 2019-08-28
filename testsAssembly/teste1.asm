addi $t0, $zero, 0xa    #t0 = 10
add $t1, $t0, $t0       #t1 = 10
b logical
arithmetic:
mul $s0, $t0, $t1       #s0 = 100
mult $t0, $t1           
mfhi $s1                #s1 = 0
mflo $s2                #s2 = 100
mtlo $s1                #lo = 100
mthi $s2                #hi = 0
div $t1, $t0
sub $s3, $s1, $s2
blez $s3, final
logical:
andi $t2, $t0, 0x1      #t2 = 1
xori $t3, $zero, 0x2    #t3 = 2
ori $t4, $zero, 0x3     #t4 = 3
and $t2, $t0, $t0       #t2 = 10
xor $t3, $zero, $t1     #t3 = 10
nor $t5, $t0, $t0
or $t6, $t4, $t3        #t6 = 3
beq $t0, $t1, arithmetic
final:
lui $t4, 0x1
bgtz $t3, end
madd $t0, $t1
bne $t0, $t0, end
movn $s0, $zero, $zero
movz $s1, $zero, $zero
msub $t0, $t1
end:
J skip
bltz $t0, end
JR $t9
bgez $s6, end
skip:

