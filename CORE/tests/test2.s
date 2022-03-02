.section .rodata
msg:
    .string "Hello World\n"

.section .data
num:
    .word 97 

.section .text
.global _start

_start:

    li a1, 100
    la x0, num
    sw a1, 0(x0)

    loop:
        jal r12 loop

    nop
_good:
    nop

_bad: 
    nop