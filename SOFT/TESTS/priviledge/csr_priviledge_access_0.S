
.section .text
.global _start

_start :
         
    csrrwi x0,marchid,0x1200
    j _bad
         