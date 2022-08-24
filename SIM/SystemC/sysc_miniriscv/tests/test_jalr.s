.section .text
.global main

main :
    auipc x10,0
    addi x1,x0,120
    jalr ra,16(x10)
    j _good
_label :
    add x6,x1,x1
    lw x2,0(x6)
    ret 