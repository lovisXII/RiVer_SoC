.section .text
.global _start

_start:
    li x2, 0x5E
    csrrwi x1, 0x341, 0x5E
    nop
    nop
    nop
    bne x1, x0, _bad
    csrrw x1, 0x341, x0
    nop
    nop
    nop
    beq x1, x2, _good
    nop

_bad: 
    nop
_good:
    
    nop