.section .text
.global _start

_start :

    li x3,0x123
    li x4,0x91231451
    sw x3,0(x4)
<<<<<<< HEAD
<<<<<<< HEAD
    sw x6,0(x4)
=======
>>>>>>> dad5b688 (new test for csr dependencies and trying to debeug riscof)
=======
>>>>>>> 052abf6d (new test for csr dependencies and trying to debeug riscof)
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop