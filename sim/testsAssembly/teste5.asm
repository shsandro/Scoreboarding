addi $t0, $zero, 0x6   #fetch N
addi $t1, $zero, 0x400 #fetch MAX_SIZE
addi $t2, $zero, 0x0   #fetch i = 0
addi $ra, $zero, 0x0   #fetch A
begin:
beq $t2, $t0, end
addi $t2, $t2, 0x1
add $ra, $ra, $t1
j begin
end:
