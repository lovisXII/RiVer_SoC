.section .text
.global _start

_start :
    addi x1,x0,-20
    addi x2,x0,-10

    sw   x1,0(x0)
    lw   x5,0(x0)
    
    sw   x2, 4(x0)
    lw   x6,4(x0)
    
    bne x5,x1,_bad
    bne x6,x2,_bad
    
    j _good