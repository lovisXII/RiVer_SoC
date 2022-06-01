.section text
.global _good
.global _bad
.global _exception_occur
_good :
    nop
_bad :
    nop
_exception_occur :
    nop
.section .kernel
.global _exception
.global _instruction_address_misagligned 
.global _instruction_address_fault 
.global _illegal_instruction 
.global _load_adress_missaligned 
.global _load_access_fault 
.global _ecall_m_mode 
.global _ecall_s_mode 
.global _exception 

_exception :

# Exception implemented : mcause value
# INSTRUCTION_ADRESS_MISSALIGNED_RE : 0 
# LOAD_ACCESS_FAULT_RE : 1
# ILLEGAL_INSTRUCTION_RE : 2
# LOAD_ADRESS_MISSALIGNED_RE : 4
# BUS_ERROR_SX : 5
# ENV_CALL_S_MODE_RE : 9 
# ENV_CALL_M_MODE_RE : 11
# EBREAK_I_RE : ?
# ECALL_I_RE : ?
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
    la x1, _exception_occur
    jalr x0, 0(x1) 
_load_access_fault :
    nop
_ecall_m_mode :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 
_ecall_s_mode :
    nop
    la x1, _exception_occur
    jalr x0, 0(x1) 



    