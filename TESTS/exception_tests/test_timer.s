.section .text
.global _start

_start:
    li x2, 0x1
    slli x2, x2, 7
    csrrw x0, 0x304, x2
    li x1, 0x50
    csrrw x0, 0x5C1, x1
    csrrwi x0, 0x5C0, 0x3
    li x1, 200
loop:
    addi x1, x1, -1
    beq x1, x0, _good
    j loop
