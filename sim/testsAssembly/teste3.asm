addi    $t0, $zero, 0xe    # fetch i
addi    $t1, $zero, 0xa    # fetch N
sub     $t2, $t0, $t1
blez    $t2, skip          # branch 
add     $t2, $zero, $zero  # A = 0
skip: