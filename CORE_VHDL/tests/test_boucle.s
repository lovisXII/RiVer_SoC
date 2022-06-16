.section .text
.global _start

_start: 
    ori x1, x0, 10
    ori x2, x0, 2
    xor x3, x0, x0
_loop: 
    addi x2, x2, 2
    addi x3, x3, 1
    bne x3, x1, _loop
    ori x10, x0, 10
    nop 
    nop
    nop 