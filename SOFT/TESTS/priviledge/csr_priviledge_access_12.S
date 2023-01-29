
.section .text
.global _start

_start :
         
    csrrwi x0,mvendor_id,0x1200
    j _bad
         