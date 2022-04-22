.section .good_bad
.global _bad
.global _good

_bad :
    nop
_good :
    nop

.section .kernel
.global _exception
.global _instruction_address_misagligned 
.global _instruction_address_fault 
.global _illegal_instruction 
.global _load_adress_missaligned 
.global _load_access_fault 
.global _ecall_u_mode 
.global _ecall_s_mode 
.global _exception 

_exception :
    csrrwi x10, 0x342, 0x0 #reading mstatus value
    slli x10,x10,2
    la x11, _instruction_address_misagligned
    add x11,x11,x10
    jalr x0,0(x11) #jumping to the right status

_instruction_address_misagligned :
    nop
_instruction_address_fault :
    nop
_illegal_instruction :
    nop
_load_adress_missaligned :
    nop
    la x1, _good
    jalr x0, 0(x1) 
_load_access_fault :
    nop
_ecall_u_mode :
    nop
_ecall_s_mode :
    nop

    