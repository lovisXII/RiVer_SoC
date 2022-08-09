#include "decoding_s2.h"


// ---------------------------------------------CHEKING DEPENDENCIES
// :---------------------------------------------

void decod::dependencies(){
    sc_uint<6> radr1_sd_s2 = RADR1_SD_S2 ; 
    sc_uint<6> radr2_sd_s2 = RADR2_SD_S2 ;
    // Need to add the case where there is store dependencies
    // addi x2,x0,10
    // sw x2, 0(x2)

    csr_in_progress_s1 = (CSR_WENABLE_RD_S1 && !DEC2EXE_EMPTY_SD_S1) || (CSR_WENABLE_RE_S1 && !EXE2MEM_EMPTY_SE_S1);
    csr_in_progress_s2 = (CSR_WENABLE_RD_S2 && !DEC2EXE_EMPTY_SD_S2) || (CSR_WENABLE_RE_S2 && !EXE2MEM_EMPTY_SE_S2);
  

    bool dependencies = (((adr_dest_sd_s1 == radr1_sd_s2) || (adr_dest_sd_s1 == radr2_sd_s2)) 
    && (adr_dest_sd_s1.read() != 0)) || (csr_wenable_sd_s1 && csr_wenable_sd_s2) ;
    
    if(dependencies)
        reg_dependencies_sd = true ;
    else
        reg_dependencies_sd = false ;
}


void decod::prio_pipeline_signal(){

    if  (reg_dependencies_sd && !stall_sd && !jump_sd_s1)
        prioritary_pipeline_sd = !prioritary_pipeline_rd.read();
    else if((jump_sd_s1 || jump_sd_s2) && !stall_sd)
        prioritary_pipeline_sd = 0;
    else
        prioritary_pipeline_sd = prioritary_pipeline_rd;
}

void decod::prio_pipeline_reg_gestion(){

    if(!RESET_N.read() || EXCEPTION_SM)
        prioritary_pipeline_rd = 0;
    else
        prioritary_pipeline_rd = prioritary_pipeline_sd ;

    PRIORITARY_PIPELINE_RD = prioritary_pipeline_sd ;
}


// ---------------------------------------------FIFO LOADING
// :---------------------------------------------

void decod::concat_dec2exe_s1() {
    sc_bv<dec2exe_size_s1> dec2exe_in_var;
    if (!EXCEPTION_SM.read()) {

        dec2exe_in_var.range(251,220) = pc_branch_value_sd_s1;  
        dec2exe_in_var[219] = mul_i_sd_s1 || mulh_i_sd_s1 || mulhsu_i_sd_s1 || mulhu_i_sd_s1;  
        dec2exe_in_var[218] = ebreak_i_sd_s1;
        dec2exe_in_var[217] = instruction_access_fault_sd_s1;  
        dec2exe_in_var[216] = mret_i_sd_s1;            
        dec2exe_in_var[215] = block_bp_sd_s1;
        dec2exe_in_var[214] = illegal_instruction_sd_s1 || instruction_adress_missaligned_sd_s1 || env_call_u_mode_sd_s1 ||
                              env_call_m_mode_sd_s1 || env_call_s_mode_sd_s1 || env_call_wrong_mode_s1 || mret_i_sd_s1 ||
                              instruction_access_fault_sd_s1 || ebreak_i_sd_s1;  // tells if there is an exception
        dec2exe_in_var[213]            = env_call_wrong_mode_s1.read();
        dec2exe_in_var[212]            = env_call_u_mode_sd_s1.read();
        dec2exe_in_var[211]            = illegal_instruction_sd_s1.read();
        dec2exe_in_var[210]            = instruction_adress_missaligned_sd_s1.read();
        dec2exe_in_var[209]            = env_call_m_mode_sd_s1.read();
        dec2exe_in_var[208]            = env_call_s_mode_sd_s1.read();
        dec2exe_in_var.range(207, 176) = CSR_RDATA_SC_S1.read();
        dec2exe_in_var[175]            = csr_wenable_sd_s1.read();
        dec2exe_in_var.range(174, 163) = csr_radr_sd_s1.read();
        dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI_S1.read();
        dec2exe_in_var[130]            = r1_valid_sd_s1.read();
        dec2exe_in_var[129]            = r2_valid_sd_s1.read();
        dec2exe_in_var.range(128, 123) = RADR1_SD_S1.read();
        dec2exe_in_var.range(122, 117) = RADR2_SD_S1.read();
        dec2exe_in_var.range(116, 115) = exe_cmd_sd_s1.read();
        dec2exe_in_var.range(114, 83)  = exe_op1_sd_s1.read();
        dec2exe_in_var.range(82, 51)   = exe_op2_sd_s1.read();
        dec2exe_in_var[50]             = exe_neg_op2_sd_s1.read();
        dec2exe_in_var[49]             = exe_wb_sd_s1.read();

        dec2exe_in_var.range(48, 17) = mem_data_sd_s1.read();

        dec2exe_in_var[16] = mem_load_sd_s1.read();
        dec2exe_in_var[15] = mem_store_sd_s1.read();

        dec2exe_in_var[14]           = mem_sign_extend_sd_s1.read();
        dec2exe_in_var.range(13, 12) = mem_size_sd_s1.read();
        dec2exe_in_var.range(11, 8)  = select_type_operations_sd_s1.read();
        dec2exe_in_var.range(7, 2)   = adr_dest_sd_s1.read();
        dec2exe_in_var[1]            = slt_i_sd_s1.read() | slti_i_sd_s1.read();
        dec2exe_in_var[0]            = sltu_i_sd_s1.read() | sltiu_i_sd_s1.read();
    } else {
        dec2exe_in_var[252]            = 0 ;
        dec2exe_in_var.range(251,220)  = 0;
        dec2exe_in_var[219]            = 0;
        dec2exe_in_var[218]            = 0; 
        dec2exe_in_var[217]            = 0;  
        dec2exe_in_var[216]            = 0;            
        dec2exe_in_var[215]            = 0;
        dec2exe_in_var[214]            = 0;  // tells if there is an exception
        dec2exe_in_var[213]            = 0;
        dec2exe_in_var[212]            = 0;
        dec2exe_in_var[211]            = 0;
        dec2exe_in_var[210]            = 0;
        dec2exe_in_var[209]            = 0;
        dec2exe_in_var[208]            = 0;
        dec2exe_in_var.range(207, 176) = 0;
        dec2exe_in_var[175]            = 0;
        dec2exe_in_var.range(174, 163) = 0;
        dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI_S1.read();
        dec2exe_in_var[130]            = 0;
        dec2exe_in_var[129]            = 0;
        dec2exe_in_var.range(128, 123) = 0;
        dec2exe_in_var.range(122, 117) = 0;
        dec2exe_in_var.range(116, 115) = 0;
        dec2exe_in_var.range(114, 83)  = 0;
        dec2exe_in_var.range(82, 51)   = 0;
        dec2exe_in_var[50]             = 0;
        dec2exe_in_var[49]             = 0;
        dec2exe_in_var.range(48, 17)   = 0;
        dec2exe_in_var[16]             = 0;
        dec2exe_in_var[15]             = 0;
        dec2exe_in_var[14]             = 0;
        dec2exe_in_var.range(13, 12)   = 0;
        dec2exe_in_var.range(11, 8)    = 0;
        dec2exe_in_var.range(7, 2)     = 0;
        dec2exe_in_var[1]              = 0;
        dec2exe_in_var[0]              = 0;
    }

    dec2exe_in_sd_s1= dec2exe_in_var;
}

void decod::concat_dec2exe_s2() {
    sc_bv<dec2exe_size_s2> dec2exe_in_var;
    if (!EXCEPTION_SM.read() && !reg_dependencies_sd.read()) {
        dec2exe_in_var.range(251,220) = pc_branch_value_sd_s2;  
        dec2exe_in_var[219] = mul_i_sd_s2 || mulh_i_sd_s2 || mulhsu_i_sd_s2 || mulhu_i_sd_s2;  
        dec2exe_in_var[218] = ebreak_i_sd_s2;
        dec2exe_in_var[217] = instruction_access_fault_sd_s2;  
        dec2exe_in_var[216] = mret_i_sd_s2;            
        dec2exe_in_var[215] = block_bp_sd_s2;
        dec2exe_in_var[214] = illegal_instruction_sd_s2 || instruction_adress_missaligned_sd_s2 || env_call_u_mode_sd_s2 ||
                              env_call_m_mode_sd_s2 || env_call_s_mode_sd_s2 || env_call_wrong_mode_s2 || mret_i_sd_s2 ||
                              instruction_access_fault_sd_s2 || ebreak_i_sd_s2;  // tells if there is an exception
        dec2exe_in_var[213]            = env_call_wrong_mode_s2.read();
        dec2exe_in_var[212]            = env_call_u_mode_sd_s2.read();
        dec2exe_in_var[211]            = illegal_instruction_sd_s2.read();
        dec2exe_in_var[210]            = instruction_adress_missaligned_sd_s2.read();
        dec2exe_in_var[209]            = env_call_m_mode_sd_s2.read();
        dec2exe_in_var[208]            = env_call_s_mode_sd_s2.read();
        dec2exe_in_var.range(207, 176) = CSR_RDATA_SC_S2.read();
        dec2exe_in_var[175]            = csr_wenable_sd_s2.read();
        dec2exe_in_var.range(174, 163) = csr_radr_sd_s2.read();
        dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI_S2.read();
        dec2exe_in_var[130]            = r1_valid_sd_s1.read();
        dec2exe_in_var[129]            = r2_valid_sd_s1.read();
        dec2exe_in_var.range(128, 123) = RADR1_SD_S2.read();
        dec2exe_in_var.range(122, 117) = RADR2_SD_S2.read();
        dec2exe_in_var.range(116, 115) = exe_cmd_sd_s2.read();
        dec2exe_in_var.range(114, 83)  = exe_op1_sd_s2.read();
        dec2exe_in_var.range(82, 51)   = exe_op2_sd_s2.read();
        dec2exe_in_var[50]             = exe_neg_op2_sd_s2.read();
        dec2exe_in_var[49]             = exe_wb_sd_s2.read();

        dec2exe_in_var.range(48, 17) = mem_data_sd_s2.read();

        dec2exe_in_var[16] = mem_load_sd_s2.read();
        dec2exe_in_var[15] = mem_store_sd_s2.read();

        dec2exe_in_var[14]           = mem_sign_extend_sd_s2.read();
        dec2exe_in_var.range(13, 12) = mem_size_sd_s2.read();
        dec2exe_in_var.range(11, 8)  = select_type_operations_sd_s2.read();
        dec2exe_in_var.range(7, 2)   = adr_dest_sd_s2.read();
        dec2exe_in_var[1]            = slt_i_sd_s2.read() | slti_i_sd_s2.read();
        dec2exe_in_var[0]            = sltu_i_sd_s2.read() | sltiu_i_sd_s2.read();
    } else {
        dec2exe_in_var.range(251,220)  = 0;
        dec2exe_in_var[219]            = 0;
        dec2exe_in_var[218]            = 0; 
        dec2exe_in_var[217]            = 0;  
        dec2exe_in_var[216]            = 0;            
        dec2exe_in_var[215]            = 0;
        dec2exe_in_var[214]            = 0;  // tells if there is an exception
        dec2exe_in_var[213]            = 0;
        dec2exe_in_var[212]            = 0;
        dec2exe_in_var[211]            = 0;
        dec2exe_in_var[210]            = 0;
        dec2exe_in_var[209]            = 0;
        dec2exe_in_var[208]            = 0;
        dec2exe_in_var.range(207, 176) = 0;
        dec2exe_in_var[175]            = 0;
        dec2exe_in_var.range(174, 163) = 0;
        reg_dependencies_sd.read() ? dec2exe_in_var.range(162, 131) = 0 : dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI_S2.read();
        dec2exe_in_var[130]            = 0;
        dec2exe_in_var[129]            = 0;
        dec2exe_in_var.range(128, 123) = 0;
        dec2exe_in_var.range(122, 117) = 0;
        dec2exe_in_var.range(116, 115) = 0;
        dec2exe_in_var.range(114, 83)  = 0;
        dec2exe_in_var.range(82, 51)   = 0;
        dec2exe_in_var[50]             = 0;
        dec2exe_in_var[49]             = 0;
        dec2exe_in_var.range(48, 17)   = 0;
        dec2exe_in_var[16]             = 0;
        dec2exe_in_var[15]             = 0;
        dec2exe_in_var[14]             = 0;
        dec2exe_in_var.range(13, 12)   = 0;
        dec2exe_in_var.range(11, 8)    = 0;
        dec2exe_in_var.range(7, 2)     = 0;
        dec2exe_in_var[1]              = 0;
        dec2exe_in_var[0]              = 0;
    }

    dec2exe_in_sd_s2= dec2exe_in_var;
}

void decod::unconcat_dec2exe_s1() {
    sc_bv<dec2exe_size_s1> dec2exe_out_var = dec2exe_out_sd_s1.read();

    PC_BRANCH_VALUE_RD_S1.write((sc_bv_base)dec2exe_out_var.range(251, 220));
    MULT_INST_RD_S1.write((bool)dec2exe_out_var[219]);
    EBREAK_RD_S1.write((bool)dec2exe_out_var[218]);
    INSTRUCTION_ACCESS_FAULT_RD_S1.write((bool)dec2exe_out_var[217]);
    MRET_RD_S1.write((bool)dec2exe_out_var[216]);
    BLOCK_BP_RD_S1.write((bool)dec2exe_out_var[215]);
    EXCEPTION_RD_S1.write((bool)dec2exe_out_var[214]);
    ENV_CALL_WRONG_MODE_RD_S1.write((bool)dec2exe_out_var[213]);
    ENV_CALL_U_MODE_RD_S1.write((bool)dec2exe_out_var[212]);
    ILLEGAL_INSTRUCTION_RD_S1.write((bool)dec2exe_out_var[211]);
    ADRESS_MISSALIGNED_RD_S1.write((bool)dec2exe_out_var[210]);
    ENV_CALL_M_MODE_RD_S1.write((bool)dec2exe_out_var[209]);
    ENV_CALL_S_MODE_RD_S1.write((bool)dec2exe_out_var[208]);
    CSR_RDATA_RD_S1.write((sc_bv_base)dec2exe_out_var.range(207, 176));
    CSR_WENABLE_RD_S1.write((bool)dec2exe_out_var[175]);
    CSR_WADR_RD_S1.write((sc_bv_base)dec2exe_out_var.range(174, 163));
    PC_DEC2EXE_RD_S1.write((sc_bv_base)dec2exe_out_var.range(162, 131));
    BP_R1_VALID_RD_S1.write((bool)dec2exe_out_var[130]);
    BP_R2_VALID_RD_S1.write((bool)dec2exe_out_var[129]);

    BP_RADR1_RD_S1.write((sc_bv_base)dec2exe_out_var.range(128, 123));
    BP_RADR2_RD_S1.write((sc_bv_base)dec2exe_out_var.range(122, 117));

    EXE_CMD_RD_S1.write((sc_bv_base)dec2exe_out_var.range(116, 115));
    OP1_RD_S1.write((sc_bv_base)dec2exe_out_var.range(114, 83));
    OP2_RD_S1.write((sc_bv_base)dec2exe_out_var.range(82, 51));
    NEG_OP2_RD_S1.write((bool)dec2exe_out_var[50]);
    WB_RD_S1.write((bool)dec2exe_out_var[49]);

    MEM_DATA_RD_S1.write((sc_bv_base)dec2exe_out_var.range(48, 17));

    MEM_LOAD_RD_S1.write((bool)dec2exe_out_var[16]);
    MEM_STORE_RD_S1.write((bool)dec2exe_out_var[15]);

    MEM_SIGN_EXTEND_RD_S1.write((bool)dec2exe_out_var[14]);
    MEM_SIZE_RD_S1.write((sc_bv_base)dec2exe_out_var.range(13, 12));
    SELECT_TYPE_OPERATIONS_RD_S1.write((sc_bv_base)dec2exe_out_var.range(11, 8));
    EXE_DEST_RD_S1.write((sc_bv_base)dec2exe_out_var.range(7, 2));
    SLT_RD_S1.write((bool)dec2exe_out_var[1]);
    SLTU_RD_S1.write((bool)dec2exe_out_var[0]);
}

void decod::unconcat_dec2exe_s2(){
    sc_bv<dec2exe_size_s2> dec2exe_out_var = dec2exe_out_sd_s2.read();

    PC_BRANCH_VALUE_RD_S2.write((sc_bv_base)dec2exe_out_var.range(251, 220));
    MULT_INST_RD_S2.write((bool)dec2exe_out_var[219]);
    EBREAK_RD_S2.write((bool)dec2exe_out_var[218]);
    INSTRUCTION_ACCESS_FAULT_RD_S2.write((bool)dec2exe_out_var[217]);
    MRET_RD_S2.write((bool)dec2exe_out_var[216]);
    BLOCK_BP_RD_S2.write((bool)dec2exe_out_var[215]);
    EXCEPTION_RD_S2.write((bool)dec2exe_out_var[214]);
    ENV_CALL_WRONG_MODE_RD_S2.write((bool)dec2exe_out_var[213]);
    ENV_CALL_U_MODE_RD_S2.write((bool)dec2exe_out_var[212]);
    ILLEGAL_INSTRUCTION_RD_S2.write((bool)dec2exe_out_var[211]);
    ADRESS_MISSALIGNED_RD_S2.write((bool)dec2exe_out_var[210]);
    ENV_CALL_M_MODE_RD_S2.write((bool)dec2exe_out_var[209]);
    ENV_CALL_S_MODE_RD_S2.write((bool)dec2exe_out_var[208]);
    CSR_RDATA_RD_S2.write((sc_bv_base)dec2exe_out_var.range(207, 176));
    CSR_WENABLE_RD_S2.write((bool)dec2exe_out_var[175]);
    CSR_WADR_RD_S2.write((sc_bv_base)dec2exe_out_var.range(174, 163));
    PC_DEC2EXE_RD_S2.write((sc_bv_base)dec2exe_out_var.range(162, 131));
    BP_R1_VALID_RD_S2.write((bool)dec2exe_out_var[130]);
    BP_R2_VALID_RD_S2.write((bool)dec2exe_out_var[129]);

    BP_RADR1_RD_S2.write((sc_bv_base)dec2exe_out_var.range(128, 123));
    BP_RADR2_RD_S2.write((sc_bv_base)dec2exe_out_var.range(122, 117));

    EXE_CMD_RD_S2.write((sc_bv_base)dec2exe_out_var.range(116, 115));
    OP1_RD_S2.write((sc_bv_base)dec2exe_out_var.range(114, 83));
    OP2_RD_S2.write((sc_bv_base)dec2exe_out_var.range(82, 51));
    NEG_OP2_RD_S2.write((bool)dec2exe_out_var[50]);
    WB_RD_S2.write((bool)dec2exe_out_var[49]);

    MEM_DATA_RD_S2.write((sc_bv_base)dec2exe_out_var.range(48, 17));

    MEM_LOAD_RD_S2.write((bool)dec2exe_out_var[16]);
    MEM_STORE_RD_S2.write((bool)dec2exe_out_var[15]);

    MEM_SIGN_EXTEND_RD_S2.write((bool)dec2exe_out_var[14]);
    MEM_SIZE_RD_S2.write((sc_bv_base)dec2exe_out_var.range(13, 12));
    SELECT_TYPE_OPERATIONS_RD_S2.write((sc_bv_base)dec2exe_out_var.range(11, 8));
    EXE_DEST_RD_S2.write((sc_bv_base)dec2exe_out_var.range(7, 2));
    SLT_RD_S2.write((bool)dec2exe_out_var[1]);
    SLTU_RD_S2.write((bool)dec2exe_out_var[0]);
}

void decod::concat_dec2if(){
    sc_bv<dec2if_size> dec2if_in_var;
    dec2if_in_var.range(63,32) = (sc_bv_base)dec2if_pc_sd_s1 ;
    dec2if_in_var.range(31,0)  = (sc_bv_base)dec2if_pc_sd_s2 ;
    dec2if_in_sd = dec2if_in_var ;
}


void decod::unconcat_dec2if(){
    sc_bv<dec2if_size> dec2if_out_var = dec2if_out_sd;

    PC_DEC2IF_RD_S1 = (sc_bv_base)dec2if_out_var.range(63,32) ;
    PC_DEC2IF_RD_S2 = (sc_bv_base)dec2if_out_var.range(31,0)  ;

}

//---------------------------------------------PC GESTION
//:---------------------------------------------

void decod::pc_inc() {
    sc_uint<32> pc                   = READ_PC_SR.read();
    sc_uint<32> pc_out_s1            = pc;
    sc_uint<32> pc_out_s2            = pc;
    sc_uint<32> offset_branch_var_s1 = offset_branch_sd_s1.read();
    sc_uint<32> offset_branch_var_s2 = offset_branch_sd_s2.read();
    
    add_offset_to_pc_s1 = jump_sd_s1.read() && !IF2DEC_EMPTY_SI_S1 ;
    add_offset_to_pc_s2 = jump_sd_s2.read() && !IF2DEC_EMPTY_SI_S2 && !reg_dependencies_sd;
    

    // PC Incrementation
    // 3 possibilities :
    //  - no branch
    //  - branch only in  s1
    //  - branch only in s2
    //  - branch in s1 and s2 -> s1 prio
    
    if (!add_offset_to_pc_s1 && !add_offset_to_pc_s2 && !dec2if_full_sd ) {//no branch
        pc_out_s1 = pc + 4;
        pc_out_s2 = pc + 8;
        WRITE_PC_ENABLE_SD  = 1;
        dec2if_push_sd      = 1;
    } else if (add_offset_to_pc_s1 && !dec2if_full_sd && !stall_sd) {//branch in s1 -> prio
        pc_out_s1 = PC_IF2DEC_RI_S1.read() + offset_branch_var_s1;
        pc_out_s2 = PC_IF2DEC_RI_S1.read() + offset_branch_var_s1 + 4;
        WRITE_PC_ENABLE_SD  = 1;
        dec2if_push_sd      = 1;
    } 
    else if(add_offset_to_pc_s2 && !add_offset_to_pc_s1 && !dec2if_full_sd && !stall_sd){// branch only in s2
        pc_out_s1 = PC_IF2DEC_RI_S2.read() + offset_branch_var_s2;
        pc_out_s2 = PC_IF2DEC_RI_S2.read() + offset_branch_var_s2 + 4 ;
        WRITE_PC_ENABLE_SD = 1 ;
        dec2if_push_sd = 1;
    }
    else {
        WRITE_PC_ENABLE_SD  = 0;
        dec2if_push_sd      = 0;
    }

        DEC2IF_EMPTY_SD     = dec2if_empty_sd ;
    
    if (EXCEPTION_SM.read() == 0 ) {
        dec2if_pc_sd_s1 = pc_out_s1 ;
        dec2if_pc_sd_s2 = pc_out_s2 ;

        WRITE_PC_SD    = pc_out_s2 ; // PC sent to REG
        
        pc_branch_value_sd_s1 = pc_out_s1 ; // sent to mem for exception
        pc_branch_value_sd_s2 = pc_out_s2 ;

        if (pc_out_s1 > KERNEL_ADR_SC && CURRENT_MODE_SM.read() != 3) {
            instruction_access_fault_sd_s1 = 1;
        } else {
            instruction_access_fault_sd_s1 = 0;
        }
        if (pc_out_s2 > KERNEL_ADR_SC && CURRENT_MODE_SM.read() != 3) {
            instruction_access_fault_sd_s2 = 1;
        } else {
            instruction_access_fault_sd_s2 = 0;
        }
        
    }
    //Instruction adress missaligned exception :
    if ((pc_out_s1 & 0b11) != 0 || (((RETURN_ADRESS_SM.read() & 0b11) != 0) && EXCEPTION_SM.read()))
    {
        instruction_adress_missaligned_sd_s1 = 1;
    } 
    else{
        instruction_adress_missaligned_sd_s1 = 0;
    }
    //Instruction adress missaligned exception :
    if ((pc_out_s2 & 0b11) != 0 || (((RETURN_ADRESS_SM.read() & 0b11) != 0) && EXCEPTION_SM.read()))
    {
        instruction_adress_missaligned_sd_s2 = 1;
    } 
    else{
        instruction_adress_missaligned_sd_s2 = 0;
    }

    // Exception & fifo gestion

    if (EXCEPTION_SM.read() == 1) 
    {
        if (!MRET_SM) {
            // Need to check MTVEC value, bits 1,0 indicate :
            // =0 -> direct type, so pc just get value of mtvec.range(31,2)
            // =1 -> vectorise, so pc get value of mtvec.range(31,2) + 4*mcause
            sc_uint<32> dec2if_var;
            sc_uint<32> WRITE_PC_VAR;
            sc_uint<32> MTVEC_VALUE_VAR;

            MTVEC_VALUE_VAR.range(31, 2) = MTVEC_VALUE_RC.read().range(31, 2);
            MTVEC_VALUE_VAR.range(1, 0)  = 0;

            if (MTVEC_VALUE_RC.read().range(1, 0) == 0) {  // direct
                dec2if_pc_sd_s1 = MTVEC_VALUE_VAR;
                dec2if_pc_sd_s2 = MTVEC_VALUE_VAR+4;
                WRITE_PC_SD  = MTVEC_VALUE_VAR + 4;
                WRITE_PC_ENABLE_SD= 1;
            } else if (MTVEC_VALUE_RC.read().range(1, 0) == 1) {  // vectorise
                sc_uint<32> MCAUSE_VAR;
                // MCAUSE * 4 :
                MCAUSE_VAR.range(31, 2) = MCAUSE_WDATA_SM_S1.read().range(29, 0);
                MCAUSE_VAR.range(1, 0)  = 0;
                dec2if_pc_sd_s1 = (MCAUSE_VAR + MTVEC_VALUE_VAR + 4);
                dec2if_pc_sd_s2 = (MCAUSE_VAR + MTVEC_VALUE_VAR + 4);
                WRITE_PC_SD.write(MCAUSE_VAR + MTVEC_VALUE_VAR + 4);
                WRITE_PC_ENABLE_SD= 1;
            }

        } else {
            if(MRET_SM){
            dec2if_pc_sd_s1= RETURN_ADRESS_SM.read();
            dec2if_pc_sd_s2= RETURN_ADRESS_SM.read() + 4;
            WRITE_PC_SD= RETURN_ADRESS_SM.read() + 4;
            WRITE_PC_ENABLE_SD= 1;
            }
        }          
            
        
        // IF2DEC Gestion
        
        IF2DEC_POP_SD_S1= 1;
        IF2DEC_POP_SD_S2= 1;
        IF2DEC_FLUSH_SD= 1;

        // DEC2EXE Gestion

        dec2exe_push_sd_s1= 1;
        dec2exe_push_sd_s2= 1;
            
    }
    else{
        
        // IF2DEC Gestion
        /*
        2 cases are possible :
        - S1 prio on S2, this is the "normal" case.
        If there is no data dependencies : 
            - If the 1st or the 2nd inst is a successfull branch we must pop both buffer and flush it
            - If there is no jump we pop both inst
            - In any other case we do not pop
        If there is a data dependcie :
            - If there is a jump on s1, we flush
            - If there is no jump we pop only S1
        */
            if ((add_offset_to_pc_s1.read() || add_offset_to_pc_s2) && !stall_sd && !reg_dependencies_sd ) 
            // If one one the 2 inst jump and no data dependencies and no stall we flush  
            {
                IF2DEC_POP_SD_S1 = 1;
                IF2DEC_POP_SD_S2= 1;
                IF2DEC_FLUSH_SD= 1;
            } 
            else if(add_offset_to_pc_s1 && !stall_sd && reg_dependencies_sd 
            && PRIORITARY_PIPELINE_RD.read() == 0
            || add_offset_to_pc_s2 && !stall_sd && reg_dependencies_sd 
            && PRIORITARY_PIPELINE_RD.read() == 1)
            // S1 prio and jump or S2 prio and jump
            {
                IF2DEC_POP_SD_S1 = 1;
                IF2DEC_POP_SD_S2= 1;
                IF2DEC_FLUSH_SD= 1;
            }
            else if (!add_offset_to_pc_s1 && !stall_sd && !reg_dependencies_sd) 
            // Case where no jump && no dependencies
            {
                IF2DEC_POP_SD_S1= 1;
                IF2DEC_POP_SD_S2= 1;
                IF2DEC_FLUSH_SD= 0;
            } 
            else if(!add_offset_to_pc_s1  && reg_dependencies_sd.read() 
            && !stall_sd && PRIORITARY_PIPELINE_RD.read() == 0) 
            // no jump && data dependencies and S1 prio S2
            // we must pop S1 but not S2 cause S1 is prio
            {
                IF2DEC_POP_SD_S1= 1;
                IF2DEC_POP_SD_S2= 0;
                IF2DEC_FLUSH_SD= 0;
            } 
            else if(!add_offset_to_pc_s1 && reg_dependencies_sd.read() 
            && !stall_sd && PRIORITARY_PIPELINE_RD.read() == 1) 
            // no jump && data dependencies and S2 prio S1
            // we must pop S2 but not S1 cause S2 is prio
            {
                IF2DEC_POP_SD_S1= 0;
                IF2DEC_POP_SD_S2= 1;
                IF2DEC_FLUSH_SD= 0;
            } 
            else //any case of stall is the same 
            {
                IF2DEC_POP_SD_S1= 0;
                IF2DEC_POP_SD_S2= 0;
                IF2DEC_FLUSH_SD= 0;
            }
        }
        
        



        // DEC2EXE_S1 Gestion
        
        if (stall_sd) 
        {
            dec2exe_push_sd_s1= 0;
            dec2exe_push_sd_s2 = 0;
        } else if(jump_sd_s1 && !stall_sd)
        {
            dec2exe_push_sd_s1=  1;
            dec2exe_push_sd_s2 = 0;
        }
        else{
            dec2exe_push_sd_s1=  1;
            dec2exe_push_sd_s2 = 1;
        }
}

void decod::bypasses() {

    // ###############################
    // BYPASS ON rs1_s1 :
    // ###############################

    if (RADR1_SD_S1.read() == 0) //case rs1 = 0, no bypass needed 
    {  
        rdata1_sd_s1= RDATA1_SR_S1.read();
        r1_valid_sd_s1= true;
    } 
    else if ((RADR1_SD_S1.read() == EXE_DEST_RD_S1.read() && !DEC2EXE_EMPTY_SD_S1.read() )
    || (RADR1_SD_S1.read() == EXE_DEST_RD_S2.read() && !DEC2EXE_EMPTY_SD_S2.read())) 
    {  
        r1_valid_sd_s1= false;
    } 
    else if ((RADR1_SD_S1.read() == DEST_RE_S1.read() && MEM_LOAD_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1)
    || ((RADR1_SD_S1.read() == DEST_RE_S2.read()) && MEM_LOAD_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2)) 
    {  // dont bypass if load instr is currently in mem
        r1_valid_sd_s1= false;
    } 
    else if(RADR1_SD_S1.read() == DEST_RE_S1.read() && MULT_INST_RE_S1 && !EXE2MEM_EMPTY_SE_S1.read())
    { // dont bypass if mul instruction didnt finish
        r1_valid_sd_s1= false;
    } 
    else if(RADR1_SD_S1.read() == DEST_RM_S1.read() && MULT_INST_RM_S1 && !MEM2WBK_EMPTY_SM_S1.read())
    { // dont bypass if mul instruction didnt finish
        r1_valid_sd_s1= false;
    } 
    // BYPASS E2->D

    else if (RADR1_SD_S1.read() == DEST_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2) 
    {  
        r1_valid_sd_s1= true;
        if (CSR_WENABLE_RE_S2.read())
            rdata1_sd_s1= CSR_RDATA_RE_S2.read();
        else
            rdata1_sd_s1= EXE_RES_RE_S2.read();
    }
    // BYPASS E1->D

    else if (RADR1_SD_S1.read() == DEST_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1) 
    {  
        r1_valid_sd_s1= true;
        if (CSR_WENABLE_RE_S1.read())
            rdata1_sd_s1= CSR_RDATA_RE_S1.read();
        else
            rdata1_sd_s1= EXE_RES_RE_S1.read();
    }  

    // BYPASS M2->D

    else if (RADR1_SD_S1.read() == DEST_RM_S2.read() && !MEM2WBK_EMPTY_SM_S2.read()) 
    {  
        r1_valid_sd_s1= true;
        if (CSR_WENABLE_RM_S2.read())
            rdata1_sd_s1 = CSR_RDATA_RM_S2.read();
        else
            rdata1_sd_s1 = MEM_RES_RM_S2.read();
    } 
    // BYPASS M1->D

    else if (RADR1_SD_S1.read() == DEST_RM_S1.read() && !MEM2WBK_EMPTY_SM_S1.read()) 
    {  
        r1_valid_sd_s1= true;
        if (CSR_WENABLE_RM_S1.read())
            rdata1_sd_s1= CSR_RDATA_RM_S1.read();
        else
            rdata1_sd_s1= MEM_RES_RM_S1.read();
    } 


    // NO BYPASS

    else {  
        r1_valid_sd_s1= true;
        rdata1_sd_s1 = RDATA1_SR_S1.read();
    }

    // ###############################
    // BYPASS ON rs2_s1 :
    // ###############################

    if (RADR2_SD_S1.read() == 0) 
    {  
        rdata2_sd_s1= RDATA2_SR_S1.read();
        r2_valid_sd_s1= true;
    } 
    else if ((RADR2_SD_S1.read() == EXE_DEST_RD_S1.read() && !DEC2EXE_EMPTY_SD_S1.read())
    || (RADR2_SD_S1.read() == EXE_DEST_RD_S2.read() && !DEC2EXE_EMPTY_SD_S2.read())) 
    {  
        r2_valid_sd_s1= false;
    } 
    else if ((RADR2_SD_S1.read() == DEST_RE_S1.read() && MEM_LOAD_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1)
    || (RADR2_SD_S1.read() == DEST_RE_S2.read() && MEM_LOAD_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2)) 
    {  
        r2_valid_sd_s1= false;
    }
    else if(RADR2_SD_S1.read() == DEST_RE_S1.read() && MULT_INST_RE_S1 && !EXE2MEM_EMPTY_SE_S1.read())
    { // dont bypass if mul instruction didnt finish
        r2_valid_sd_s1= false;
    }
    else if(RADR2_SD_S1.read() == DEST_RM_S1.read() && MULT_INST_RM_S1 && !MEM2WBK_EMPTY_SM_S1.read())
    { // dont bypass if mul instruction didnt finish
        r2_valid_sd_s1= false;
    }

    // bypass E2->D

    else if (RADR2_SD_S1.read() == DEST_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2) 
    {  
        r2_valid_sd_s1= true;
        if (CSR_WENABLE_RE_S2.read())
            rdata2_sd_s1= CSR_RDATA_RE_S2.read();
        else
            rdata2_sd_s1= EXE_RES_RE_S2.read();
    }  
    // bypass E1->D

    else if (RADR2_SD_S1.read() == DEST_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1) 
    {  
        r2_valid_sd_s1= true;
        if (CSR_WENABLE_RE_S1.read())
            rdata2_sd_s1= CSR_RDATA_RE_S1.read();
        else
            rdata2_sd_s1= EXE_RES_RE_S1.read();
    }
 
    // bypass M2->D

    else if (RADR2_SD_S1.read() == DEST_RM_S2.read() && !MEM2WBK_EMPTY_SM_S2.read()) 
    {  
        r2_valid_sd_s1= true;
        if (CSR_WENABLE_RM_S2.read())
            rdata2_sd_s1= CSR_RDATA_RM_S2.read();
        else
            rdata2_sd_s1= MEM_RES_RM_S2.read();
    } 
    // bypass M1->D

    else if (RADR2_SD_S1.read() == DEST_RM_S1.read() && !MEM2WBK_EMPTY_SM_S1.read()) 
    {  
        r2_valid_sd_s1= true;
        if (CSR_WENABLE_RM_S1.read())
            rdata2_sd_s1= CSR_RDATA_RM_S1.read();
        else
            rdata2_sd_s1= MEM_RES_RM_S1.read();
    } 


    else 
    {  // no bypass
        r2_valid_sd_s1= true;
        rdata2_sd_s1= RDATA2_SR_S1.read();
    }
    // When a load is in exe, we can block the pipeline now
    // Avoid an issue with load - load - add sequence


    // ###############################
    // BYPASS ON rs1_s2 :
    // ###############################

     if (RADR1_SD_S2.read() == 0) 
    {  
        rdata1_sd_s2= RDATA1_SR_S2.read();
        r1_valid_sd_s2= true;
    } 
    else if (((RADR1_SD_S2.read() == EXE_DEST_RD_S2.read() && !DEC2EXE_EMPTY_SD_S2.read())
    || (RADR1_SD_S2.read() == EXE_DEST_RD_S1.read() && !DEC2EXE_EMPTY_SD_S1.read())) && !reg_dependencies_sd) 
    {  
        r1_valid_sd_s2= false;
    } 
    else if (((RADR1_SD_S2.read() == DEST_RE_S2.read() && MEM_LOAD_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2)
    || (RADR1_SD_S2.read() == DEST_RE_S1.read() && MEM_LOAD_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1)) && !reg_dependencies_sd) 
    {  
        r1_valid_sd_s2= false;
    } 

    // bypass E1->D

    else if (RADR1_SD_S2.read() == DEST_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1 && !reg_dependencies_sd) 
    {  
        r1_valid_sd_s2= true;
        if (CSR_WENABLE_RE_S1.read())
            rdata1_sd_s2= CSR_RDATA_RE_S1.read();
        else
            rdata1_sd_s2= EXE_RES_RE_S1.read();
    } 

    // bypass E2->D

    else if (RADR1_SD_S2.read() == DEST_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2 && !reg_dependencies_sd) 
    {  
        r1_valid_sd_s2= true;
        if (CSR_WENABLE_RE_S2.read())
            rdata1_sd_s2= CSR_RDATA_RE_S2.read();
        else
            rdata1_sd_s2= EXE_RES_RE_S2.read();
    } 

    // bypass M1->D

    else if (RADR1_SD_S2.read() == DEST_RM_S1.read() && !MEM2WBK_EMPTY_SM_S1.read() && !reg_dependencies_sd) 
    {  
        r1_valid_sd_s2= true;
        if (CSR_WENABLE_RM_S1.read())
            rdata1_sd_s2 = CSR_RDATA_RM_S1.read();
        else
            rdata1_sd_s2 = MEM_RES_RM_S1.read();
    } 

    // bypass M2->D

    else if (RADR1_SD_S2.read() == DEST_RM_S2.read() && !MEM2WBK_EMPTY_SM_S2.read() && !reg_dependencies_sd) 
    {  
        r1_valid_sd_s2= true;
        if (CSR_WENABLE_RM_S2.read())
            rdata1_sd_s2= CSR_RDATA_RM_S2.read();
        else
            rdata1_sd_s2= MEM_RES_RM_S2.read();
    } 

    // no bypass

    else {  
        r1_valid_sd_s2= true;
        rdata1_sd_s2= RDATA1_SR_S2.read();
    }

    // ###############################
    // BYPASS ON rs2_s2 :
    // ###############################

    if (RADR2_SD_S2.read() == 0) 
    {  
        rdata2_sd_s2= RDATA2_SR_S2.read();
        r2_valid_sd_s2= true;
    } 
    else if (((RADR2_SD_S2.read() == EXE_DEST_RD_S1.read() && !DEC2EXE_EMPTY_SD_S1.read())
    || (RADR2_SD_S2.read() == EXE_DEST_RD_S2.read() && !DEC2EXE_EMPTY_SD_S2.read())) && !reg_dependencies_sd) 
    {  
        r2_valid_sd_s2= false;
    } 
    else if (((RADR2_SD_S2.read() == DEST_RE_S1.read() && MEM_LOAD_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1)
    || (RADR2_SD_S2.read() == DEST_RE_S2.read() && MEM_LOAD_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2)) && !reg_dependencies_sd) 
    {  
        r2_valid_sd_s2= false;
    }
    // bypass E1->D

    else if (RADR2_SD_S2.read() == DEST_RE_S1.read() && !EXE2MEM_EMPTY_SE_S1 && !reg_dependencies_sd) 
    {  
        r2_valid_sd_s2= true;
        if (CSR_WENABLE_RE_S1.read())
            rdata2_sd_s2= CSR_RDATA_RE_S1.read();
        else
            rdata2_sd_s2= EXE_RES_RE_S1.read();
    }

    // bypass E2->D

    else if (RADR2_SD_S2.read() == DEST_RE_S2.read() && !EXE2MEM_EMPTY_SE_S2 && !reg_dependencies_sd) 
    {  
        r2_valid_sd_s2= true;
        if (CSR_WENABLE_RE_S2.read())
            rdata2_sd_s2= CSR_RDATA_RE_S2.read();
        else
            rdata2_sd_s2= EXE_RES_RE_S2.read();
    }   

    // bypass M1->D

    else if (RADR2_SD_S2.read() == DEST_RM_S1.read() && !MEM2WBK_EMPTY_SM_S1.read() && !reg_dependencies_sd) 
    {  
        r2_valid_sd_s2= true;
        if (CSR_WENABLE_RM_S1.read())
            rdata2_sd_s2= CSR_RDATA_RM_S1.read();
        else
            rdata2_sd_s2= MEM_RES_RM_S1.read();
    } 

    // bypass M2->D

    else if (RADR2_SD_S2.read() == DEST_RM_S2.read() && !MEM2WBK_EMPTY_SM_S2.read() && !reg_dependencies_sd) 
    {  
        r2_valid_sd_s2= true;
        if (CSR_WENABLE_RM_S2.read())
            rdata2_sd_s2= CSR_RDATA_RM_S2.read();
        else
            rdata2_sd_s2= MEM_RES_RM_S2.read();
    } 
    else 
    {  // no bypass
        r2_valid_sd_s2= true;
        rdata2_sd_s2= RDATA2_SR_S2.read();
    }

    block_in_dec.write((RADR1_SD_S1.read() == EXE_DEST_RD_S1.read() && MEM_LOAD_RD_S1 && !DEC2EXE_EMPTY_SD_S1.read()) ||
                       (RADR2_SD_S1.read() == EXE_DEST_RD_S1.read() && MEM_LOAD_RD_S1 && !DEC2EXE_EMPTY_SD_S1.read()));
}
void decod::stall_method() {
  
    stall_sd        = ((csr_in_progress_s1   || csr_in_progress_s2) && (csr_wenable_sd_s1 || csr_wenable_sd_s2) 
    || ((!r1_valid_sd_s1 || !r2_valid_sd_s1)    && (b_type_inst_sd_s1 || jalr_type_inst_sd_s1 || j_type_inst_sd_s1 || block_in_dec))
    || (((!r1_valid_sd_s2 || !r2_valid_sd_s2)   && (b_type_inst_sd_s2 || jalr_type_inst_sd_s2 || j_type_inst_sd_s2 || block_in_dec)))
    || (IF2DEC_EMPTY_SI_S1 && IF2DEC_EMPTY_SI_S2) || dec2exe_full_sd_s1 || dec2exe_full_sd_s2);


}

//---------------------------------------------METHOD TO TRACE SIGNALS
//:---------------------------------------------

void decod::trace(sc_trace_file* tf) {
    dec2if.trace(tf);
    dec2exe_s1.trace(tf);
    dec2exe_s2.trace(tf);
    sc_trace(tf, RDATA1_SR_S1, GET_NAME(RDATA1_SR_S1));
    sc_trace(tf, RDATA2_SR_S1, GET_NAME(RDATA2_SR_S1));

    sc_trace(tf, RDATA1_SR_S2, GET_NAME(RDATA1_SR_S2));
    sc_trace(tf, RDATA2_SR_S2, GET_NAME(RDATA2_SR_S2));

    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));  // value of r32 which is pc coming from REG

    sc_trace(tf, RADR1_SD_S1, GET_NAME(RADR1_SD_S1));  // adress of rs
    sc_trace(tf, RADR2_SD_S1, GET_NAME(RADR2_SD_S1));  // adress of rt

    sc_trace(tf, RADR1_SD_S2, GET_NAME(RADR1_SD_S2));  // adress of rs
    sc_trace(tf, RADR2_SD_S2, GET_NAME(RADR2_SD_S2));  // adress of rt

    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));

    // Interface with EXE :

    sc_trace(tf, OP1_RD_S1, GET_NAME(OP1_RD_S1));            // value of op1
    sc_trace(tf, OP2_RD_S1, GET_NAME(OP2_RD_S1));            // value of op2
    sc_trace(tf, EXE_CMD_RD_S1, GET_NAME(EXE_CMD_RD_S1));    // value of the command sent to exe
    sc_trace(tf, NEG_OP2_RD_S1, GET_NAME(NEG_OP2_RD_S1));    // say if we take the opposite of the op1 to do a
                                                       // substraction for example
    sc_trace(tf, WB_RD_S1, GET_NAME(WB_RD_S1));              // say if we plan to wbk the value of rd or no
    sc_trace(tf, EXE_DEST_RD_S1, GET_NAME(EXE_DEST_RD_S1));  // the destination register
    sc_trace(tf, SELECT_TYPE_OPERATIONS_RD_S1, GET_NAME(SELECT_TYPE_OPERATIONS_RD_S1));  // taille fifo entrée : 110
    sc_trace(tf, SLT_RD_S1, GET_NAME(SLT_RD_S1));
    sc_trace(tf, SLTU_RD_S1, GET_NAME(SLTU_RD_S1));
    sc_trace(tf, PC_DEC2EXE_RD_S1, GET_NAME(PC_DEC2EXE_RD_S1));  // PC link to the current decoded instruction

    sc_trace(tf, MEM_DATA_RD_S1, GET_NAME(MEM_DATA_RD_S1));    // data sent to mem for storage
    sc_trace(tf, MEM_LOAD_RD_S1, GET_NAME(MEM_LOAD_RD_S1));    // say to mem if we do a load
    sc_trace(tf, MEM_STORE_RD_S1, GET_NAME(MEM_STORE_RD_S1));  // say to mem if we do a store
    sc_trace(tf, MEM_SIGN_EXTEND_RD_S1, GET_NAME(MEM_SIGN_EXTEND_RD_S1));
    sc_trace(tf, MEM_SIZE_RD_S1, GET_NAME(MEM_SIZE_RD_S1));  // tells to mem if we do an acces in word, hw or byte

    sc_trace(tf, CSR_WENABLE_RD_S1, GET_NAME(CSR_WENABLE_RD_S1));  // indicate if we do a csr operation,
                                                             // if so need to WBK CSR in rd
    sc_trace(tf, CSR_WADR_RD_S1, GET_NAME(CSR_WADR_RD_S1));        // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_trace(tf, CSR_RDATA_RD_S1, GET_NAME(CSR_RDATA_RD_S1));      // CSR read data to be wb in register

    // Interface with DEC2IF :

    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));  // Ifecth say to decod if it wants a pop or no
    sc_trace(tf, DEC2IF_EMPTY_SD, GET_NAME(DEC2IF_EMPTY_SD));
    sc_trace(tf, PC_DEC2IF_RD_S1, GET_NAME(PC_DEC2IF_RD_S1));  // this value must also be sent to REG
    sc_trace(tf, PC_DEC2IF_RD_S2, GET_NAME(PC_DEC2IF_RD_S2));  // this value must also be sent to REG
    sc_trace(tf, dec2if_pc_sd_s1, GET_NAME(dec2if_pc_sd_s1));  
    sc_trace(tf, dec2if_pc_sd_s2, GET_NAME(dec2if_pc_sd_s2));  
    sc_trace(tf, dec2if_in_sd, GET_NAME(dec2if_in_sd));  
    
    // Interface with IF2DEC :

    sc_trace(tf, PC_IF2DEC_RI_S1, GET_NAME(PC_IF2DEC_RI_S1));
    sc_trace(tf, PC_IF2DEC_RI_S2, GET_NAME(PC_IF2DEC_RI_S2));
    sc_trace(tf, INSTR_RI_S1, GET_NAME(INSTR_RI_S1));
    sc_trace(tf, INSTR_RI_S2, GET_NAME(INSTR_RI_S2));
    sc_trace(tf, IF2DEC_EMPTY_SI_S1, GET_NAME(IF2DEC_EMPTY_SI_S1));
    sc_trace(tf, IF2DEC_EMPTY_SI_S2, GET_NAME(IF2DEC_EMPTY_SI_S2));
    sc_trace(tf, IF2DEC_POP_SD_S1, GET_NAME(IF2DEC_POP_SD_S1));  // Decod says to IFETCH if it wants a pop or no
    sc_trace(tf, IF2DEC_POP_SD_S2, GET_NAME(IF2DEC_POP_SD_S2));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    // Interface with DEC2EXE

    sc_trace(tf, DEC2EXE_POP_SE_S1, GET_NAME(DEC2EXE_POP_SE_S1));
    sc_trace(tf, DEC2EXE_EMPTY_SD_S1, GET_NAME(DEC2EXE_EMPTY_SD_S1));
    sc_trace(tf, DEC2EXE_EMPTY_SD_S2, GET_NAME(DEC2EXE_EMPTY_SD_S2));
    sc_trace(tf, dec2exe_out_sd_s1, GET_NAME(dec2exe_out_sd_s1));
    sc_trace(tf, MULT_INST_RD_S1, GET_NAME(MULT_INST_RD_S1));
    
    // Interface with CSR :

    sc_trace(tf, CSR_RADR_SD_S1, GET_NAME(CSR_RADR_SD_S1));    // CSR adress sent to CSR to get data
    sc_trace(tf, CSR_RDATA_SC_S1, GET_NAME(CSR_RDATA_SC_S1));  // data read from CSR

    // Bypasses

    sc_trace(tf, DEST_RE_S1, GET_NAME(DEST_RE_S1));
    sc_trace(tf, DEST_RE_S2, GET_NAME(DEST_RE_S2));
    sc_trace(tf, EXE_RES_RE_S1, GET_NAME(EXE_RES_RE_S1));
    sc_trace(tf, EXE_RES_RE_S2, GET_NAME(EXE_RES_RE_S2));
    sc_trace(tf, MEM_LOAD_RE_S1, GET_NAME(MEM_LOAD_RE_S1));
    sc_trace(tf, MEM_LOAD_RE_S2, GET_NAME(MEM_LOAD_RE_S2));
    sc_trace(tf, EXE2MEM_EMPTY_SE_S1, GET_NAME(EXE2MEM_EMPTY_SE_S1));
    sc_trace(tf, EXE2MEM_EMPTY_SE_S2, GET_NAME(EXE2MEM_EMPTY_SE_S2));
    sc_trace(tf, MEM2WBK_EMPTY_SM_S1, GET_NAME(MEM2WBK_EMPTY_SM_S1));
    sc_trace(tf, DEST_RM_S1, GET_NAME(DEST_RM_S1));
    sc_trace(tf, DEST_RM_S2, GET_NAME(DEST_RM_S2));
    sc_trace(tf, MEM_RES_RM_S1, GET_NAME(MEM_RES_RM_S1));
    sc_trace(tf, MULT_INST_RE_S1, GET_NAME(MULT_INST_RE_S1));
    sc_trace(tf, MULT_INST_RM_S1, GET_NAME(MULT_INST_RM_S1));
    
    sc_trace(tf, CSR_WENABLE_RE_S1, GET_NAME(CSR_WENABLE_RE_S1));
    sc_trace(tf, CSR_RDATA_RE_S1, GET_NAME(CSR_RDATA_RE_S1));
    sc_trace(tf, CSR_WENABLE_RM_S1, GET_NAME(CSR_WENABLE_RM_S1));
    sc_trace(tf, CSR_RDATA_RM_S1, GET_NAME(CSR_RDATA_RM_S1));

    sc_trace(tf, BP_R1_VALID_RD_S1, GET_NAME(BP_R1_VALID_RD_S1));
    sc_trace(tf, BP_R2_VALID_RD_S1, GET_NAME(BP_R2_VALID_RD_S1));
    sc_trace(tf, BP_RADR1_RD_S1, GET_NAME(BP_RADR1_RD_S1));
    sc_trace(tf, BP_RADR2_RD_S1, GET_NAME(BP_RADR2_RD_S1));

    // Exception :

    sc_trace(tf,
             EXCEPTION_RI,
             GET_NAME(EXCEPTION_RI));  // this signal will be at 0 considering there is no exception in IFETCH

    sc_trace(tf, ENV_CALL_U_MODE_RD_S1, GET_NAME(ENV_CALL_U_MODE_RD_S1));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RD_S1, GET_NAME(ENV_CALL_WRONG_MODE_RD_S1));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RD_S1, GET_NAME(ILLEGAL_INSTRUCTION_RD_S1));  // instruction doesnt exist
    sc_trace(tf, ADRESS_MISSALIGNED_RD_S1, GET_NAME(ADRESS_MISSALIGNED_RD_S1));    // branch offset is misaligned
    sc_trace(tf, ENV_CALL_S_MODE_RD_S1, GET_NAME(ENV_CALL_S_MODE_RD_S1));
    sc_trace(tf, ENV_CALL_M_MODE_RD_S1, GET_NAME(ENV_CALL_M_MODE_RD_S1));

    sc_trace(tf, EXCEPTION_RD_S1, GET_NAME(EXCEPTION_RD_S1));
    // General Interface :

    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, MTVEC_VALUE_RC, GET_NAME(MTVEC_VALUE_RC));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, INTERRUPTION_SE_S1, GET_NAME(INTERRUPTION_SE_S1));
    sc_trace(tf, rdata1_sd_s1, GET_NAME(rdata1_sd_s1));
    sc_trace(tf, rdata2_sd_s1, GET_NAME(rdata2_sd_s1));
    sc_trace(tf, r1_valid_sd_s1, GET_NAME(r1_valid_sd_s1));
    sc_trace(tf, r2_valid_sd_s1, GET_NAME(r2_valid_sd_s1));
    sc_trace(tf, r1_valid_sd_s2, GET_NAME(r1_valid_sd_s2));
    sc_trace(tf, r2_valid_sd_s2, GET_NAME(r2_valid_sd_s2));
    sc_trace(tf, stall_sd, GET_NAME(stall_sd));
    sc_trace(tf, block_in_dec, GET_NAME(block_in_dec));
    sc_trace(tf, dec2if_push_sd, GET_NAME(dec2if_push_sd));
    sc_trace(tf, dec2if_empty_sd, GET_NAME(dec2if_empty_sd));
    sc_trace(tf, dec2if_full_sd, GET_NAME(dec2if_full_sd));
    sc_trace(tf, dec2if_out_sd, GET_NAME(dec2if_out_sd));
    sc_trace(tf, dec2exe_in_sd_s1, GET_NAME(dec2exe_in_sd_s1));
    sc_trace(tf, dec2exe_push_sd_s1, GET_NAME(dec2exe_push_sd_s1));
    sc_trace(tf, dec2exe_push_sd_s2, GET_NAME(dec2exe_push_sd_s2));
    sc_trace(tf, dec2exe_full_sd_s1, GET_NAME(dec2exe_full_sd_s1));
    sc_trace(tf, dec2exe_full_sd_s2, GET_NAME(dec2exe_full_sd_s2));
    sc_trace(tf, r_type_inst_sd_s1, GET_NAME(r_type_inst_sd_s1));        // R type format
    sc_trace(tf, i_type_inst_sd_s1, GET_NAME(i_type_inst_sd_s1));        // I type format
    sc_trace(tf, s_type_inst_sd_s1, GET_NAME(s_type_inst_sd_s1));        // S type format
    sc_trace(tf, b_type_inst_sd_s1, GET_NAME(b_type_inst_sd_s1));        // B type format
    sc_trace(tf, u_type_inst_sd_s1, GET_NAME(u_type_inst_sd_s1));        // U type format
    sc_trace(tf, j_type_inst_sd_s1, GET_NAME(j_type_inst_sd_s1));        // J type format
    sc_trace(tf, m_type_inst_sd_s1, GET_NAME(m_type_inst_sd_s1));        // M type format
    sc_trace(tf, jalr_type_inst_sd_s1, GET_NAME(jalr_type_inst_sd_s1));  // JALR has a specific opcode
    sc_trace(tf, system_type_inst_sd_s1, GET_NAME(system_type_inst_sd_s1));
    sc_trace(tf, add_i_sd_s1, GET_NAME(add_i_sd_s1));
    sc_trace(tf, slt_i_sd_s1, GET_NAME(slt_i_sd_s1));
    sc_trace(tf, sltu_i_sd_s1, GET_NAME(sltu_i_sd_s1));
    sc_trace(tf, and_i_sd_s1, GET_NAME(and_i_sd_s1));
    sc_trace(tf, or_i_sd_s1, GET_NAME(or_i_sd_s1));
    sc_trace(tf, xor_i_sd_s1, GET_NAME(xor_i_sd_s1));
    sc_trace(tf, sll_i_sd_s1, GET_NAME(sll_i_sd_s1));
    sc_trace(tf, srl_i_sd_s1, GET_NAME(srl_i_sd_s1));
    sc_trace(tf, sub_i_sd_s1, GET_NAME(sub_i_sd_s1));
    sc_trace(tf, sra_i_sd_s1, GET_NAME(sra_i_sd_s1));
    sc_trace(tf, addi_i_sd_s1, GET_NAME(addi_i_sd_s1));
    sc_trace(tf, slti_i_sd_s1, GET_NAME(slti_i_sd_s1));
    sc_trace(tf, sltiu_i_sd_s1, GET_NAME(sltiu_i_sd_s1));
    sc_trace(tf, andi_i_sd_s1, GET_NAME(andi_i_sd_s1));
    sc_trace(tf, ori_i_sd_s1, GET_NAME(ori_i_sd_s1));
    sc_trace(tf, xori_i_sd_s1, GET_NAME(xori_i_sd_s1));
    sc_trace(tf, jalr_i_sd_s1, GET_NAME(jalr_i_sd_s1));
    sc_trace(tf, slli_i_sd_s1, GET_NAME(slli_i_sd_s1));
    sc_trace(tf, srli_i_sd_s1, GET_NAME(srli_i_sd_s1));
    sc_trace(tf, srai_i_sd_s1, GET_NAME(srai_i_sd_s1));
    sc_trace(tf, lw_i_sd_s1, GET_NAME(lw_i_sd_s1));
    sc_trace(tf, lh_i_sd_s1, GET_NAME(lh_i_sd_s1));
    sc_trace(tf, lhu_i_sd_s1, GET_NAME(lhu_i_sd_s1));
    sc_trace(tf, lb_i_sd_s1, GET_NAME(lb_i_sd_s1));
    sc_trace(tf, lbu_i_sd_s1, GET_NAME(lbu_i_sd_s1));
    sc_trace(tf, beq_i_sd_s1, GET_NAME(beq_i_sd_s1));
    sc_trace(tf, bne_i_sd_s1, GET_NAME(bne_i_sd_s1));
    sc_trace(tf, blt_i_sd_s1, GET_NAME(blt_i_sd_s1));
    sc_trace(tf, bge_i_sd_s1, GET_NAME(bge_i_sd_s1));
    sc_trace(tf, bltu_i_sd_s1, GET_NAME(bltu_i_sd_s1));
    sc_trace(tf, bgeu_i_sd_s1, GET_NAME(bgeu_i_sd_s1));
    sc_trace(tf, lui_i_sd_s1, GET_NAME(lui_i_sd_s1));
    sc_trace(tf, auipc_i_sd_s1, GET_NAME(auipc_i_sd_s1));
    sc_trace(tf, jal_i_sd_s1, GET_NAME(jal_i_sd_s1));
    sc_trace(tf, sw_i_sd_s1, GET_NAME(sw_i_sd_s1));
    sc_trace(tf, sh_i_sd_s1, GET_NAME(sh_i_sd_s1));
    sc_trace(tf, sb_i_sd_s1, GET_NAME(sb_i_sd_s1));
    sc_trace(tf, mul_i_sd_s1, GET_NAME(mul_i_sd_s1));
    sc_trace(tf, mulh_i_sd_s1, GET_NAME(mulh_i_sd_s1));
    sc_trace(tf, mulhsu_i_sd_s1, GET_NAME(mulhsu_i_sd_s1));
    sc_trace(tf, mulhu_i_sd_s1, GET_NAME(mulhu_i_sd_s1));
    sc_trace(tf, div_i_sd_s1, GET_NAME(div_i_sd_s1));
    sc_trace(tf, divu_i_sd_s1, GET_NAME(divu_i_sd_s1));
    sc_trace(tf, rem_i_sd_s1, GET_NAME(rem_i_sd_s1));
    sc_trace(tf, remu_i_sd_s1, GET_NAME(remu_i_sd_s1));
    sc_trace(tf, csrrw_i_sd_s1, GET_NAME(csrrw_i_sd_s1));
    sc_trace(tf, csrrs_i_sd_s1, GET_NAME(csrrs_i_sd_s1));
    sc_trace(tf, MEM2WBK_EMPTY_SM_S2, GET_NAME(MEM2WBK_EMPTY_SM_S2));
    sc_trace(tf, csrrc_i_sd_s1, GET_NAME(csrrc_i_sd_s1));
    sc_trace(tf, csrrwi_i_sd_s1, GET_NAME(csrrwi_i_sd_s1));
    sc_trace(tf, csrrsi_i_sd_s1, GET_NAME(csrrsi_i_sd_s1));
    sc_trace(tf, csrrci_i_sd_s1, GET_NAME(csrrci_i_sd_s1));
    sc_trace(tf, csr_wenable_sd_s1, GET_NAME(csr_wenable_sd_s1));
    sc_trace(tf, csr_radr_sd_s1, GET_NAME(csr_radr_sd_s1));
    sc_trace(tf, offset_branch_sd_s1, GET_NAME(offset_branch_sd_s1));
    sc_trace(tf, csr_in_progress_s1, GET_NAME(csr_in_progress_s1));
    sc_trace(tf, mret_i_sd_s1, GET_NAME(mret_i_sd_s1));
    sc_trace(tf, sret_i_sd_s1, GET_NAME(sret_i_sd_s1));
    sc_trace(tf, reg_dependencies_sd, GET_NAME(reg_dependencies_sd));

    // PC gestion :

    sc_trace(tf, inc_pc_sd_s1, GET_NAME(inc_pc_sd_s1));
    sc_trace(tf, jump_sd_s1, GET_NAME(jump_sd_s1));

    // Internal signals :

    sc_trace(tf, adr_dest_sd_s1, GET_NAME(adr_dest_sd_s1));
    sc_trace(tf, env_call_u_mode_sd_s2, GET_NAME(env_call_u_mode_sd_s2));
    sc_trace(tf, exe_op1_sd_s1, GET_NAME(exe_op1_sd_s1));
    sc_trace(tf, exe_op2_sd_s1, GET_NAME(exe_op2_sd_s1));
    sc_trace(tf, mem_data_sd_s1, GET_NAME(mem_data_sd_s1));

    sc_trace(tf, mem_size_sd_s1, GET_NAME(mem_size_sd_s1));
    sc_trace(tf, mem_load_sd_s1, GET_NAME(mem_load_sd_s1));
    sc_trace(tf, mem_store_sd_s1, GET_NAME(mem_store_sd_s1));

    sc_trace(tf, exe_cmd_sd_s1, GET_NAME(exe_cmd_sd_s1));
    sc_trace(tf, select_type_operations_sd_s1, GET_NAME(select_type_operations_sd_s1));
    sc_trace(tf, exe_neg_op2_sd_s1, GET_NAME(exe_neg_op2_sd_s1));
    sc_trace(tf, exe_wb_sd_s1, GET_NAME(exe_wb_sd_s1));
    sc_trace(tf, mem_sign_extend_sd_s1, GET_NAME(mem_sign_extend_sd_s1));

    // Exception :

    sc_trace(tf, ecall_i_sd_s1, GET_NAME(ecall_i_sd_s1));
    sc_trace(tf, ebreak_i_sd_s1, GET_NAME(ebreak_i_sd_s1));
    sc_trace(tf, illegal_instruction_sd_s1, GET_NAME(illegal_instruction_sd_s1));  // instruction doesnt exist
    sc_trace(tf,
             instruction_adress_missaligned_sd_s1,
             GET_NAME(instruction_adress_missaligned_sd_s1));  // branch offset is misaligned
    sc_trace(tf, env_call_m_mode_sd_s1, GET_NAME(env_call_m_mode_sd_s1));
    sc_trace(tf, env_call_s_mode_sd_s1, GET_NAME(env_call_s_mode_sd_s1));
sc_trace(tf, rdata1_sd_s2, GET_NAME(rdata1_sd_s2));
    sc_trace(tf, rdata2_sd_s2, GET_NAME(rdata2_sd_s2));
    sc_trace(tf, r_type_inst_sd_s2, GET_NAME(r_type_inst_sd_s2));        // R type format
    sc_trace(tf, i_type_inst_sd_s2, GET_NAME(i_type_inst_sd_s2));        // I type format
    sc_trace(tf, s_type_inst_sd_s2, GET_NAME(s_type_inst_sd_s2));        // S type format
    sc_trace(tf, b_type_inst_sd_s2, GET_NAME(b_type_inst_sd_s2));        // B type format
    sc_trace(tf, u_type_inst_sd_s2, GET_NAME(u_type_inst_sd_s2));        // U type format
    sc_trace(tf, j_type_inst_sd_s2, GET_NAME(j_type_inst_sd_s2));        // J type format
    sc_trace(tf, m_type_inst_sd_s2, GET_NAME(m_type_inst_sd_s2));        // M type format
    sc_trace(tf, jalr_type_inst_sd_s2, GET_NAME(jalr_type_inst_sd_s2));  // JALR has a specific opcode
    sc_trace(tf, system_type_inst_sd_s2, GET_NAME(system_type_inst_sd_s2));
    sc_trace(tf, add_i_sd_s2, GET_NAME(add_i_sd_s2));
    sc_trace(tf, slt_i_sd_s2, GET_NAME(slt_i_sd_s2));
    sc_trace(tf, sltu_i_sd_s2, GET_NAME(sltu_i_sd_s2));
    sc_trace(tf, and_i_sd_s2, GET_NAME(and_i_sd_s2));
    sc_trace(tf, or_i_sd_s2, GET_NAME(or_i_sd_s2));
    sc_trace(tf, xor_i_sd_s2, GET_NAME(xor_i_sd_s2));
    sc_trace(tf, sll_i_sd_s2, GET_NAME(sll_i_sd_s2));
    sc_trace(tf, srl_i_sd_s2, GET_NAME(srl_i_sd_s2));
    sc_trace(tf, sub_i_sd_s2, GET_NAME(sub_i_sd_s2));
    sc_trace(tf, sra_i_sd_s2, GET_NAME(sra_i_sd_s2));
    sc_trace(tf, addi_i_sd_s2, GET_NAME(addi_i_sd_s2));
    sc_trace(tf, slti_i_sd_s2, GET_NAME(slti_i_sd_s2));
    sc_trace(tf, sltiu_i_sd_s2, GET_NAME(sltiu_i_sd_s2));
    sc_trace(tf, andi_i_sd_s2, GET_NAME(andi_i_sd_s2));
    sc_trace(tf, ori_i_sd_s2, GET_NAME(ori_i_sd_s2));
    sc_trace(tf, xori_i_sd_s2, GET_NAME(xori_i_sd_s2));
    sc_trace(tf, jalr_i_sd_s2, GET_NAME(jalr_i_sd_s2));
    sc_trace(tf, slli_i_sd_s2, GET_NAME(slli_i_sd_s2));
    sc_trace(tf, srli_i_sd_s2, GET_NAME(srli_i_sd_s2));
    sc_trace(tf, srai_i_sd_s2, GET_NAME(srai_i_sd_s2));
    sc_trace(tf, lw_i_sd_s2, GET_NAME(lw_i_sd_s2));
    sc_trace(tf, lh_i_sd_s2, GET_NAME(lh_i_sd_s2));
    sc_trace(tf, lhu_i_sd_s2, GET_NAME(lhu_i_sd_s2));
    sc_trace(tf, lb_i_sd_s2, GET_NAME(lb_i_sd_s2));
    sc_trace(tf, lbu_i_sd_s2, GET_NAME(lbu_i_sd_s2));
    sc_trace(tf, beq_i_sd_s2, GET_NAME(beq_i_sd_s2));
    sc_trace(tf, bne_i_sd_s2, GET_NAME(bne_i_sd_s2));
    sc_trace(tf, blt_i_sd_s2, GET_NAME(blt_i_sd_s2));
    sc_trace(tf, bge_i_sd_s2, GET_NAME(bge_i_sd_s2));
    sc_trace(tf, bltu_i_sd_s2, GET_NAME(bltu_i_sd_s2));
    sc_trace(tf, bgeu_i_sd_s2, GET_NAME(bgeu_i_sd_s2));
    sc_trace(tf, lui_i_sd_s2, GET_NAME(lui_i_sd_s2));
    sc_trace(tf, auipc_i_sd_s2, GET_NAME(auipc_i_sd_s2));
    sc_trace(tf, jal_i_sd_s2, GET_NAME(jal_i_sd_s2));
    sc_trace(tf, sw_i_sd_s2, GET_NAME(sw_i_sd_s2));
    sc_trace(tf, sh_i_sd_s2, GET_NAME(sh_i_sd_s2));
    sc_trace(tf, sb_i_sd_s2, GET_NAME(sb_i_sd_s2));
    sc_trace(tf, mul_i_sd_s2, GET_NAME(mul_i_sd_s2));
    sc_trace(tf, mulh_i_sd_s2, GET_NAME(mulh_i_sd_s2));
    sc_trace(tf, mulhsu_i_sd_s2, GET_NAME(mulhsu_i_sd_s2));
    sc_trace(tf, mulhu_i_sd_s2, GET_NAME(mulhu_i_sd_s2));
    sc_trace(tf, div_i_sd_s2, GET_NAME(div_i_sd_s2));
    sc_trace(tf, divu_i_sd_s2, GET_NAME(divu_i_sd_s2));
    sc_trace(tf, rem_i_sd_s2, GET_NAME(rem_i_sd_s2));
    sc_trace(tf, remu_i_sd_s2, GET_NAME(remu_i_sd_s2));
    sc_trace(tf, csrrw_i_sd_s2, GET_NAME(csrrw_i_sd_s2));
    sc_trace(tf, csrrs_i_sd_s2, GET_NAME(csrrs_i_sd_s2));
    sc_trace(tf, csrrc_i_sd_s2, GET_NAME(csrrc_i_sd_s2));
    sc_trace(tf, csrrwi_i_sd_s2, GET_NAME(csrrwi_i_sd_s2));
    sc_trace(tf, csrrsi_i_sd_s2, GET_NAME(csrrsi_i_sd_s2));
    sc_trace(tf, csrrci_i_sd_s2, GET_NAME(csrrci_i_sd_s2));
    sc_trace(tf, csr_wenable_sd_s2, GET_NAME(csr_wenable_sd_s2));
    sc_trace(tf, csr_radr_sd_s2, GET_NAME(csr_radr_sd_s2));
    sc_trace(tf, offset_branch_sd_s2, GET_NAME(offset_branch_sd_s2));
    sc_trace(tf, csr_in_progress_s2, GET_NAME(csr_in_progress_s2));
    sc_trace(tf, mret_i_sd_s2, GET_NAME(mret_i_sd_s2));
    sc_trace(tf, sret_i_sd_s2, GET_NAME(sret_i_sd_s2));

    // PC gestion :

    sc_trace(tf, inc_pc_sd_s2, GET_NAME(inc_pc_sd_s2));
    sc_trace(tf, jump_sd_s2, GET_NAME(jump_sd_s2));

    // Internal signals :

    sc_trace(tf, adr_dest_sd_s2, GET_NAME(adr_dest_sd_s2));
    sc_trace(tf, exe_op1_sd_s2, GET_NAME(exe_op1_sd_s2));
    sc_trace(tf, exe_op2_sd_s2, GET_NAME(exe_op2_sd_s2));
    sc_trace(tf, mem_data_sd_s2, GET_NAME(mem_data_sd_s2));

    sc_trace(tf, mem_size_sd_s2, GET_NAME(mem_size_sd_s2));
    sc_trace(tf, mem_load_sd_s2, GET_NAME(mem_load_sd_s2));
    sc_trace(tf, mem_store_sd_s2, GET_NAME(mem_store_sd_s2));

    sc_trace(tf, exe_cmd_sd_s2, GET_NAME(exe_cmd_sd_s2));
    sc_trace(tf, select_type_operations_sd_s2, GET_NAME(select_type_operations_sd_s2));
    sc_trace(tf, exe_neg_op2_sd_s2, GET_NAME(exe_neg_op2_sd_s2));
    sc_trace(tf, exe_wb_sd_s2, GET_NAME(exe_wb_sd_s2));
    sc_trace(tf, mem_sign_extend_sd_s2, GET_NAME(mem_sign_extend_sd_s2));

    // Exception :

    sc_trace(tf, ecall_i_sd_s2, GET_NAME(ecall_i_sd_s2));
    sc_trace(tf, ebreak_i_sd_s2, GET_NAME(ebreak_i_sd_s2));
    sc_trace(tf, illegal_instruction_sd_s2, GET_NAME(illegal_instruction_sd_s2));  // instruction doesnt exist
    sc_trace(tf,
             instruction_adress_missaligned_sd_s2,
             GET_NAME(instruction_adress_missaligned_sd_s2));  // branch offset is misaligned
    sc_trace(tf, env_call_m_mode_sd_s2, GET_NAME(env_call_m_mode_sd_s2));
    sc_trace(tf, env_call_s_mode_sd_s2, GET_NAME(env_call_s_mode_sd_s2));
    sc_trace(tf, CURRENT_MODE_SM, GET_NAME(CURRENT_MODE_SM));
    sc_trace(tf, MRET_RD_S1, GET_NAME(MRET_RD_S1));
    sc_trace(tf, RETURN_ADRESS_SM, GET_NAME(RETURN_ADRESS_SM));
    sc_trace(tf, env_call_u_mode_sd_s1, GET_NAME(env_call_u_mode_sd_s1));
    sc_trace(tf, instruction_access_fault_sd_s1, GET_NAME(instruction_access_fault_sd_s1));
    sc_trace(tf, INSTRUCTION_ACCESS_FAULT_RD_S1, GET_NAME(INSTRUCTION_ACCESS_FAULT_RD_S1));
    sc_trace(tf, MCAUSE_WDATA_SM_S1, GET_NAME(MCAUSE_WDATA_SM_S1));
    sc_trace(tf, env_call_wrong_mode_s1, GET_NAME(env_call_wrong_mode_s1));
    sc_trace(tf, PC_BRANCH_VALUE_RD_S1, GET_NAME(PC_BRANCH_VALUE_RD_S1));
    sc_trace(tf, pc_branch_value_sd_s1, GET_NAME(pc_branch_value_sd_s1));
    sc_trace(tf,PC_BRANCH_VALUE_RD_S2, GET_NAME(PC_BRANCH_VALUE_RD_S2));
    sc_trace(tf,MULT_INST_RD_S2, GET_NAME(MULT_INST_RD_S2));
    sc_trace(tf,EBREAK_RD_S2, GET_NAME(EBREAK_RD_S2));
    sc_trace(tf,INSTRUCTION_ACCESS_FAULT_RD_S2, GET_NAME(INSTRUCTION_ACCESS_FAULT_RD_S2));
    sc_trace(tf,MRET_RD_S2, GET_NAME(MRET_RD_S2));
    sc_trace(tf,MRET_SM, GET_NAME(MRET_SM));
    sc_trace(tf,BLOCK_BP_RD_S2, GET_NAME(BLOCK_BP_RD_S2));
    sc_trace(tf,EXCEPTION_RD_S2, GET_NAME(EXCEPTION_RD_S2));
    sc_trace(tf,ENV_CALL_WRONG_MODE_RD_S2, GET_NAME(ENV_CALL_WRONG_MODE_RD_S2));
    sc_trace(tf,ENV_CALL_U_MODE_RD_S2, GET_NAME(ENV_CALL_U_MODE_RD_S2));
    sc_trace(tf,ILLEGAL_INSTRUCTION_RD_S2, GET_NAME(ILLEGAL_INSTRUCTION_RD_S2));
    sc_trace(tf,ADRESS_MISSALIGNED_RD_S2, GET_NAME(ADRESS_MISSALIGNED_RD_S2));
    sc_trace(tf,ENV_CALL_M_MODE_RD_S2, GET_NAME(ENV_CALL_M_MODE_RD_S2));
    sc_trace(tf,ENV_CALL_S_MODE_RD_S2, GET_NAME(ENV_CALL_S_MODE_RD_S2));
    sc_trace(tf,CSR_RDATA_RD_S2, GET_NAME(CSR_RDATA_RD_S2));
    sc_trace(tf,CSR_WENABLE_RD_S2, GET_NAME(CSR_WENABLE_RD_S2));
    sc_trace(tf,CSR_WADR_RD_S2, GET_NAME(CSR_WADR_RD_S2));
    sc_trace(tf,PC_DEC2EXE_RD_S2, GET_NAME(PC_DEC2EXE_RD_S2));
    sc_trace(tf,BP_R1_VALID_RD_S2, GET_NAME(BP_R1_VALID_RD_S2));
    sc_trace(tf,BP_R2_VALID_RD_S2, GET_NAME(BP_R2_VALID_RD_S2));

    sc_trace(tf,BP_RADR1_RD_S2, GET_NAME(BP_RADR1_RD_S2));
    sc_trace(tf,BP_RADR2_RD_S2, GET_NAME(BP_RADR2_RD_S2));

    sc_trace(tf,EXE_CMD_RD_S2, GET_NAME(EXE_CMD_RD_S2));
    sc_trace(tf,OP1_RD_S2, GET_NAME(OP1_RD_S2));
    sc_trace(tf,OP2_RD_S2, GET_NAME(OP2_RD_S2));
    sc_trace(tf,NEG_OP2_RD_S2, GET_NAME(NEG_OP2_RD_S2));
    sc_trace(tf,WB_RD_S2, GET_NAME(WB_RD_S2));

    sc_trace(tf,MEM_DATA_RD_S2, GET_NAME(MEM_DATA_RD_S2));

    sc_trace(tf,MEM_LOAD_RD_S2, GET_NAME(MEM_LOAD_RD_S2));
    sc_trace(tf,MEM_STORE_RD_S2, GET_NAME(MEM_STORE_RD_S2));
    sc_trace(tf,PRIORITARY_PIPELINE_RD, GET_NAME(PRIORITARY_PIPELINE_RD));

    sc_trace(tf,MEM_SIGN_EXTEND_RD_S2, GET_NAME(MEM_SIGN_EXTEND_RD_S2));
    sc_trace(tf,MEM_SIZE_RD_S2, GET_NAME(MEM_SIZE_RD_S2));
    sc_trace(tf,SELECT_TYPE_OPERATIONS_RD_S2, GET_NAME(SELECT_TYPE_OPERATIONS_RD_S2));
    sc_trace(tf,EXE_DEST_RD_S2, GET_NAME(EXE_DEST_RD_S2));
    sc_trace(tf,SLT_RD_S2, GET_NAME(SLT_RD_S2));
    sc_trace(tf,SLTU_RD_S2, GET_NAME(SLTU_RD_S2));
    sc_trace(tf,add_offset_to_pc_s1, GET_NAME(add_offset_to_pc_s1));
    sc_trace(tf,add_offset_to_pc_s2, GET_NAME(add_offset_to_pc_s2));
    sc_trace(tf,prioritary_pipeline_sd, GET_NAME(prioritary_pipeline_sd));
    sc_trace(tf,prioritary_pipeline_rd, GET_NAME(prioritary_pipeline_rd));
}