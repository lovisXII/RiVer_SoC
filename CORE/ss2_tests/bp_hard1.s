.section .text
.global _start 


_start :
    li x20,970
    li x21,362
    li x22,485
    li x1,12
    li x2,23
    li x3,25
    li x4,256
    li x5,362
    add x1,x2,x3 # x1 = 48
    sw x5,0(x1) # 0(x1) = 362 
    lw x6,0(x1) # x6 = 362
    bne x6,x21,_bad
    sub x1,x1,x2 # x1 = 48-23=25
    addi x6,x0,123 # x6 = 362+123=485
    bne x6,x22,_bad
    slli x6,x6,1 #x6 = 970
    bne x6,x20,_bad
    j _good
    