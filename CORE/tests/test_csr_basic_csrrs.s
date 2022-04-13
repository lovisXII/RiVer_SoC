.section .text
.global _start

_start:
    addi x3, x0,0x1
    addi x4, x0, 0x5
    csrrwi x3,0x300,0x4 # put 0x4 inside 0x300
    csrrs x1, 0x300, x3 # 0x300 <=  (0x4 or 0x1 = 0x5) 
    beq x3,x4, _good
    beq x3,x4, _bad
    nop
    nop
_bad: 
    nop
_good:
    nop


