
.section .text
.global _start

_start :
         
    csrrwi x0,mscratch,0x1200
    j _bad
         