
.section .text
.global _start

_start :
         
    csrrwi x0,mimpid,0x1200
    j _bad
         