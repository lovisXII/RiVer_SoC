.section .text
.global _start

_start :

    li x3,0x123
    li x4,0x91231451
    sw x3,0(x4)
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    sw x6,0(x4)
=======
>>>>>>> dad5b688 (new test for csr dependencies and trying to debeug riscof)
=======
>>>>>>> 052abf6d (new test for csr dependencies and trying to debeug riscof)
=======
    sw x6,0(x4)
>>>>>>> 3fc7ad98 (7 tests left to pass. Issue comes from current mode i think)
=======
    sw x6,0(x4)
>>>>>>> f660c8c5 (editing issue during merge)
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