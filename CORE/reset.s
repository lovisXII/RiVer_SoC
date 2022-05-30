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
    la x28,0x10054       #loading main adress
    la x29,0x81000000    #loading exception handler adress
    csrrw x0, 0x341,x28 
    csrrw x0, 0x305,x29
    li x2,0x10000       # sp initialization
    mret
