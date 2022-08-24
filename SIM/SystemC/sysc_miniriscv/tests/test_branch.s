.section .text
.global main

main:
    addi x1,x0,1254
    bge x0,x0,_good
    nop