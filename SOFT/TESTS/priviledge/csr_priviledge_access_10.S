
.section .text
.global _start

_start :
         
    csrrwi x0,mtval,0x1200
    j _bad
         