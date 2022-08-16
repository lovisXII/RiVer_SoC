.section .text.reset
.global _reset
_reset: 
    nop
    addi x1, x1, 4;
    sub  x2, x2, x1;
    jal  x5, main;


    nop
    nop
.global _good
_good:
    nop
    nop
.global _bad
_bad:
    nop
    nop
