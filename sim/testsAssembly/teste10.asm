addi $t1, $zero, 0x0
addi $t2, $zero, 0x1    
madd $t1, $t2           # RAW
mflo $t0                # RAW
or $t0, $t1, $t2        # WAW
addi $t1, $zero, 0xc    # WAR
