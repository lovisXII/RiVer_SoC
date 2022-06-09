.section .text
.global _start

_start :

    li  s7,0x10002230
    li  x1, 0xdeffbeef
    sw  x1,-4(s7)
    li  s8,4         # s8 = 4
    sb  s8,-2(s7)    
    lw  x30,-4(s7)
    # s8 = 4
    # s7 = 10002230
    # @ = 10002228