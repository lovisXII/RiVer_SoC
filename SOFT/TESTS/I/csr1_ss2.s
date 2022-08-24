.section .text
.global _start

_start:
    li x1,0x80000120
    li x2,0x90002314
    j _test
    csrrw x0,mepc, x1


_return:
    csrrw x3, mepc, x0
    beq x3, x1, _good
    j _bad

_test:
    j _return
    nop