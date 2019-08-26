# i = N*N + 3*N
addi    $t0, $zero, 0xa          # N=10
mul   $t0, $t0, $t0     # N*N
addi    $t1, $zero, 0xa          # fetch N
ori    $t2, $zero, 0x3   # 3
mul   $t1, $t1, $t2     # 3*N
add    $t2, $t0, $t1     # N*N + 3*N
add    $t3, $t2, $zero   # i = ...
