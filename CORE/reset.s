.section .reset
.global _reset

# What's need to be initialize :
# CPU start in kernel mode
# It must load :
# * main adress
# * exception handler adress
# * 

_reset:
    nop
    nop
    la x1,0x10054       #loading main adress
    la x2,0x81000000    #loading exception handler adress
    csrrw x0, 0x341,x1 
    csrrw x0, 0x305,x2
    mret
