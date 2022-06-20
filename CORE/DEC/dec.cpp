#include "decoding_s1.h"

// ---------------------------------------------DECODING INSTRUCTION
// :---------------------------------------------

void decod::concat_dec2exe() {
    sc_bv<dec2exe_size> dec2exe_in_var;
    if (EXCEPTION_SM.read() == 0) {
        dec2exe_in_var.range(251,220) = pc_branch_value_sd;  
        dec2exe_in_var[219] = mul_i_sd || mulh_i_sd || mulhsu_i_sd || mulhu_i_sd;  
        dec2exe_in_var[218] = ebreak_i_sd;
        dec2exe_in_var[217] = instruction_access_fault_sd;  
        dec2exe_in_var[216] = mret_i_sd;            
        dec2exe_in_var[215] = block_bp_sd;
        dec2exe_in_var[214] = illegal_instruction_sd || instruction_adress_missaligned_sd || env_call_u_mode_sd ||
                              env_call_m_mode_sd || env_call_s_mode_sd || env_call_wrong_mode || mret_i_sd ||
                              instruction_access_fault_sd || ebreak_i_sd;  // tells if there is an exception
        dec2exe_in_var[213]            = env_call_wrong_mode.read();
        dec2exe_in_var[212]            = env_call_u_mode_sd.read();
        dec2exe_in_var[211]            = illegal_instruction_sd.read();
        dec2exe_in_var[210]            = instruction_adress_missaligned_sd.read();
        dec2exe_in_var[209]            = env_call_m_mode_sd.read();
        dec2exe_in_var[208]            = env_call_s_mode_sd.read();
        dec2exe_in_var.range(207, 176) = CSR_RDATA_SC.read();
        dec2exe_in_var[175]            = csr_wenable_sd.read();
        dec2exe_in_var.range(174, 163) = csr_radr_sd.read();
        dec2exe_in_var.range(162, 131) = PC_IF2DEC_RI_S1.read();
        dec2exe_in_var[130]            = r1_valid_sd.read();
        dec2exe_in_var[129]            = r2_valid_sd.read();
        dec2exe_in_var.range(128, 123) = RADR1_SD.read();
        dec2exe_in_var.range(122, 117) = RADR2_SD.read();
        dec2exe_in_var.range(116, 115) = exe_cmd_sd.read();
        dec2exe_in_var.range(114, 83)  = exe_op1_sd.read();
        dec2exe_in_var.range(82, 51)   = exe_op2_sd.read();
        dec2exe_in_var[50]             = exe_neg_op2_sd.read();
        dec2exe_in_var[49]             = exe_wb_sd.read();

        dec2exe_in_var.range(48, 17) = mem_data_sd.read();

        dec2exe_in_var[16] = mem_load_sd.read();
        dec2exe_in_var[15] = mem_store_sd.read();

        dec2exe_in_var[14]           = mem_sign_extend_sd.read();
        dec2exe_in_var.range(13, 12) = mem_size_sd.read();
        dec2exe_in_var.range(11, 8)  = select_type_operations_sd.read();
        dec2exe_in_var.range(7, 2)   = adr_dest_sd.read();
        dec2exe_in_var[1]            = slt_i_sd.read() | slti_i_sd.read();
        dec2exe_in_var[0]            = sltu_i_sd.read() | sltiu_i_sd.read();
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

    dec2exe_in_sd.write(dec2exe_in_var);
}


void decod::unconcat_dec2exe() {
    sc_bv<dec2exe_size> dec2exe_out_var = dec2exe_out_sd.read();

    PC_BRANCH_VALUE_RD.write((sc_bv_base)dec2exe_out_var.range(251, 220));
    MULT_INST_RD.write((bool)dec2exe_out_var[219]);
    EBREAK_RD.write((bool)dec2exe_out_var[218]);
    INSTRUCTION_ACCESS_FAULT_RD.write((bool)dec2exe_out_var[217]);
    MRET_RD.write((bool)dec2exe_out_var[216]);
    BLOCK_BP_RD.write((bool)dec2exe_out_var[215]);
    EXCEPTION_RD.write((bool)dec2exe_out_var[214]);
    ENV_CALL_WRONG_MODE_RD.write((bool)dec2exe_out_var[213]);
    ENV_CALL_U_MODE_RD.write((bool)dec2exe_out_var[212]);
    ILLEGAL_INSTRUCTION_RD.write((bool)dec2exe_out_var[211]);
    ADRESS_MISSALIGNED_RD.write((bool)dec2exe_out_var[210]);
    ENV_CALL_M_MODE_RD.write((bool)dec2exe_out_var[209]);
    ENV_CALL_S_MODE_RD.write((bool)dec2exe_out_var[208]);
    CSR_RDATA_RD.write((sc_bv_base)dec2exe_out_var.range(207, 176));
    CSR_WENABLE_RD.write((bool)dec2exe_out_var[175]);
    CSR_WADR_RD.write((sc_bv_base)dec2exe_out_var.range(174, 163));
    PC_DEC2EXE_RD.write((sc_bv_base)dec2exe_out_var.range(162, 131));
    BP_R1_VALID_RD.write((bool)dec2exe_out_var[130]);
    BP_R2_VALID_RD.write((bool)dec2exe_out_var[129]);

    BP_RADR1_RD.write((sc_bv_base)dec2exe_out_var.range(128, 123));
    BP_RADR2_RD.write((sc_bv_base)dec2exe_out_var.range(122, 117));

    EXE_CMD_RD.write((sc_bv_base)dec2exe_out_var.range(116, 115));
    OP1_RD.write((sc_bv_base)dec2exe_out_var.range(114, 83));
    OP2_RD.write((sc_bv_base)dec2exe_out_var.range(82, 51));
    NEG_OP2_RD.write((bool)dec2exe_out_var[50]);
    WB_RD.write((bool)dec2exe_out_var[49]);

    MEM_DATA_RD.write((sc_bv_base)dec2exe_out_var.range(48, 17));

    MEM_LOAD_RD.write((bool)dec2exe_out_var[16]);
    MEM_STORE_RD.write((bool)dec2exe_out_var[15]);

    MEM_SIGN_EXTEND_RD.write((bool)dec2exe_out_var[14]);
    MEM_SIZE_RD.write((sc_bv_base)dec2exe_out_var.range(13, 12));
    SELECT_TYPE_OPERATIONS_RD.write((sc_bv_base)dec2exe_out_var.range(11, 8));
    EXE_DEST_SD.write((sc_bv_base)dec2exe_out_var.range(7, 2));
    SLT_RD.write((bool)dec2exe_out_var[1]);
    SLTU_RD.write((bool)dec2exe_out_var[0]);
}

void decod::concat_dec2if(){
    sc_bv<dec2if_size> dec2if_in_var;
    dec2if_in_var.range(63,32) = (sc_bv_base)dec2if_pc_sd_s1 ;
    dec2if_in_var.range(31,0)  = (sc_bv_base)dec2if_pc_sd_s2 ;
    dec2if_in_sd = dec2if_in_var ;
}


void decod::unconcat_dec2if(){
    sc_bv<dec2if_size> dec2if_out_var = dec2if_out_sd;

    PC_RD_S1 = (sc_bv_base)dec2if_out_var.range(63,32) ;
    PC_RD_S2 = (sc_bv_base)dec2if_out_var.range(31,0)  ;

}

//---------------------------------------------PC GESTION
//:---------------------------------------------

void decod::pc_inc() {
    sc_uint<32> pc                = READ_PC_SR.read();
    sc_uint<32> pc_out_s1            = pc;
    sc_uint<32> pc_out_s2            = pc;
    sc_uint<32> offset_branch_var = offset_branch_sd.read();
    bool add_offset_to_pc = jump_sd.read() && !IF2DEC_EMPTY_SI ;
    

    // PC Incrementation

    if (!add_offset_to_pc && !dec2if_full_sd ) {
        pc_out_s1 = pc + 4;
        pc_out_s2 = pc + 8;
        WRITE_PC_ENABLE_SD  = 1;
        dec2if_push_sd      = 1;
    } else if (add_offset_to_pc && !dec2if_full_sd && !stall_sd) {
        pc_out_s1 = PC_IF2DEC_RI_S1.read() + offset_branch_var;
        WRITE_PC_ENABLE_SD  = 1;
        dec2if_push_sd      = 1;
    } else {
        WRITE_PC_ENABLE_SD  = 0;
        dec2if_push_sd      = 0;
    }

        DEC2IF_EMPTY_SD     = dec2if_empty_sd ;
    
    if (EXCEPTION_SM.read() == 0 ) {
        dec2if_pc_sd_s1 = pc_out_s1 ;
        dec2if_pc_sd_s2 = pc_out_s2 ;

        WRITE_PC_SD    = pc_out_s2 ; // PC sent to REG
        
        pc_branch_value_sd = pc_out_s1 ; // sent to mem for exception
        
        if (pc_out_s2 > start_kernel_adress && CURRENT_MODE_SM.read() != 3) {
            instruction_access_fault_sd = 1;
        } else {
            instruction_access_fault_sd = 0;
        }
    }
    //Instruction adress missaligned exception :
    if ((pc_out_s1 & 0b11) != 0 || (((RETURN_ADRESS_SM.read() & 0b11) != 0) && EXCEPTION_SM.read()))
    {
        instruction_adress_missaligned_sd = 1;
    } 
    else{
        instruction_adress_missaligned_sd = 0;
    }

    // Exception & fifo gestion

    if (EXCEPTION_SM.read() == 1) {
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
                WRITE_PC_SD  = MTVEC_VALUE_VAR;
                WRITE_PC_ENABLE_SD.write(1);
            } else if (MTVEC_VALUE_RC.read().range(1, 0) == 1) {  // vectorise
                sc_uint<32> MCAUSE_VAR;
                // MCAUSE * 4 :
                MCAUSE_VAR.range(31, 2) = MCAUSE_WDATA_SM.read().range(29, 0);
                MCAUSE_VAR.range(1, 0)  = 0;
                dec2if_pc_sd_s1.write(MCAUSE_VAR + MTVEC_VALUE_VAR);
                WRITE_PC_SD.write(MCAUSE_VAR + MTVEC_VALUE_VAR);
                WRITE_PC_ENABLE_SD.write(1);
            }

        } else {
            dec2if_pc_sd_s1.write(RETURN_ADRESS_SM.read());
            WRITE_PC_SD.write(RETURN_ADRESS_SM.read());
            WRITE_PC_ENABLE_SD.write(1);
        }          
            
        
        // IF2DEC Gestion
        
        IF2DEC_POP_SD.write(1);
        IF2DEC_FLUSH_SD.write(0);

        // DEC2EXE Gestion

        dec2exe_push_sd.write(1);

    }
    else{
        
        // IF2DEC Gestion
        
        if (jump_sd.read() && !stall_sd) {
            IF2DEC_POP_SD.write(1);
            IF2DEC_FLUSH_SD.write(1);
        } else if (!jump_sd && !stall_sd) {
            IF2DEC_POP_SD.write(1);
            IF2DEC_FLUSH_SD.write(0);
        } else {
            IF2DEC_POP_SD.write(0);
            IF2DEC_FLUSH_SD.write(0);
        }

        // DEC2EXE Gestion
        
        if (stall_sd) {
            dec2exe_push_sd.write(0);
        } else {
            dec2exe_push_sd.write(1);
        }
    }
}

void decod::bypasses() {
    if (RADR1_SD.read() == 0) {  // ignore r0
        rdata1_sd.write(RDATA1_SR.read());
        r1_valid_sd.write(true);
    } 
    else if (RADR1_SD.read() == EXE_DEST_SD.read() && !DEC2EXE_EMPTY_SD.read()) 
    {  // dont bypass if instr is currently in exe
        r1_valid_sd.write(false);
    } 
    else if (RADR1_SD.read() == BP_DEST_RE.read() && BP_MEM_LOAD_RE.read() && !BP_EXE2MEM_EMPTY_SE) 
    {  // dont bypass if load instr is currently in mem
        r1_valid_sd.write(false);
    } 
    else if(RADR1_SD.read() == BP_DEST_RE.read() && MULT_INST_RE && !BP_EXE2MEM_EMPTY_SE.read())
    { // dont bypass if mul instruction didnt finish
        r1_valid_sd.write(false);
    } 
    else if(RADR1_SD.read() == BP_DEST_RM.read() && MULT_INST_RM && !BP_MEM2WBK_EMPTY_SM.read())
    { // dont bypass if mul instruction didnt finish
        r1_valid_sd.write(false);
    } 
    else if (RADR1_SD.read() == BP_DEST_RE.read() && !BP_EXE2MEM_EMPTY_SE) 
    {  // bypass E->D
        r1_valid_sd.write(true);
        if (CSR_WENABLE_RE.read())
            rdata1_sd.write(CSR_RDATA_RE.read());
        else
            rdata1_sd.write(BP_EXE_RES_RE.read());
    } 
    else if (RADR1_SD.read() == BP_DEST_RM.read() && !BP_MEM2WBK_EMPTY_SM.read()) 
    {  // bypass M->D
        r1_valid_sd.write(true);
        if (CSR_WENABLE_RM.read())
            rdata1_sd.write(CSR_RDATA_RM.read());
        else
            rdata1_sd.write(BP_MEM_RES_RM.read());
    } 
    else {  // no bypass
        r1_valid_sd.write(true);
        rdata1_sd.write(RDATA1_SR.read());
    }

    if (RADR2_SD.read() == 0) {  // ignore r0
        rdata2_sd.write(RDATA2_SR.read());
        r2_valid_sd.write(true);
    } else if (RADR2_SD.read() == EXE_DEST_SD.read() &&
               !DEC2EXE_EMPTY_SD.read()) {  // dont bypass if instr is currently in exe
        r2_valid_sd.write(false);
    } else if (RADR2_SD.read() == BP_DEST_RE.read() && BP_MEM_LOAD_RE.read() &&
               !BP_EXE2MEM_EMPTY_SE) {  // dont bypass if load instr is
                                        // currently in mem
        r2_valid_sd.write(false);
    } else if(RADR2_SD.read() == BP_DEST_RE.read() && MULT_INST_RE && !BP_EXE2MEM_EMPTY_SE.read()){ // dont bypass if mul instruction didnt finish
        r2_valid_sd.write(false);
    } else if(RADR2_SD.read() == BP_DEST_RM.read() && MULT_INST_RM && !BP_MEM2WBK_EMPTY_SM.read()){ // dont bypass if mul instruction didnt finish
        r2_valid_sd.write(false);
    } else if (RADR2_SD.read() == BP_DEST_RE.read() && !BP_EXE2MEM_EMPTY_SE) {  // bypass E->D
        r2_valid_sd.write(true);
        if (CSR_WENABLE_RE.read())
            rdata2_sd.write(CSR_RDATA_RE.read());
        else
            rdata2_sd.write(BP_EXE_RES_RE.read());
    } else if (RADR2_SD.read() == BP_DEST_RM.read() && !BP_MEM2WBK_EMPTY_SM.read()) {  // bypass M->D
        r2_valid_sd.write(true);
        if (CSR_WENABLE_RM.read())
            rdata2_sd.write(CSR_RDATA_RM.read());
        else
            rdata2_sd.write(BP_MEM_RES_RM.read());
    } else {  // no bypass
        r2_valid_sd.write(true);
        rdata2_sd.write(RDATA2_SR.read());
    }
    // When a load is in exe, we can block the pipeline now
    // Avoid an issue with load - load - add sequence
    block_in_dec.write((RADR1_SD.read() == EXE_DEST_SD.read() && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD.read()) ||
                       (RADR2_SD.read() == EXE_DEST_SD.read() && MEM_LOAD_RD && !DEC2EXE_EMPTY_SD.read()));
}

void decod::stall_method() {
    csr_in_progress = (CSR_WENABLE_RD && !DEC2EXE_EMPTY_SD) || (CSR_WENABLE_RE && !BP_EXE2MEM_EMPTY_SE);
    stall_sd        = (csr_in_progress || ((!r1_valid_sd || !r2_valid_sd) &&
                      (b_type_inst_sd || jalr_type_inst_sd || j_type_inst_sd || block_in_dec))
                      || IF2DEC_EMPTY_SI || dec2exe_full_sd);
}

//---------------------------------------------METHOD TO TRACE SIGNALS
//:---------------------------------------------

void decod::trace(sc_trace_file* tf) {
    dec2if.trace(tf);
    dec2exe.trace(tf);
    sc_trace(tf, RDATA1_SR, GET_NAME(RDATA1_SR));
    sc_trace(tf, RDATA2_SR, GET_NAME(RDATA2_SR));

    sc_trace(tf, READ_PC_SR, GET_NAME(READ_PC_SR));  // value of r32 which is pc coming from REG

    sc_trace(tf, RADR1_SD, GET_NAME(RADR1_SD));  // adress of rs
    sc_trace(tf, RADR2_SD, GET_NAME(RADR2_SD));  // adress of rt

    sc_trace(tf, WRITE_PC_SD, GET_NAME(WRITE_PC_SD));
    sc_trace(tf, WRITE_PC_ENABLE_SD, GET_NAME(WRITE_PC_ENABLE_SD));

    // Interface with EXE :

    sc_trace(tf, OP1_RD, GET_NAME(OP1_RD));            // value of op1
    sc_trace(tf, OP2_RD, GET_NAME(OP2_RD));            // value of op2
    sc_trace(tf, EXE_CMD_RD, GET_NAME(EXE_CMD_RD));    // value of the command sent to exe
    sc_trace(tf, NEG_OP2_RD, GET_NAME(NEG_OP2_RD));    // say if we take the opposite of the op1 to do a
                                                       // substraction for example
    sc_trace(tf, WB_RD, GET_NAME(WB_RD));              // say if we plan to wbk the value of rd or no
    sc_trace(tf, EXE_DEST_SD, GET_NAME(EXE_DEST_SD));  // the destination register
    sc_trace(tf, SELECT_TYPE_OPERATIONS_RD, GET_NAME(SELECT_TYPE_OPERATIONS_RD));  // taille fifo entrée : 110
    sc_trace(tf, SLT_RD, GET_NAME(SLT_RD));
    sc_trace(tf, SLTU_RD, GET_NAME(SLTU_RD));
    sc_trace(tf, PC_DEC2EXE_RD, GET_NAME(PC_DEC2EXE_RD));  // PC link to the current decoded instruction

    sc_trace(tf, MEM_DATA_RD, GET_NAME(MEM_DATA_RD));    // data sent to mem for storage
    sc_trace(tf, MEM_LOAD_RD, GET_NAME(MEM_LOAD_RD));    // say to mem if we do a load
    sc_trace(tf, MEM_STORE_RD, GET_NAME(MEM_STORE_RD));  // say to mem if we do a store
    sc_trace(tf, MEM_SIGN_EXTEND_RD, GET_NAME(MEM_SIGN_EXTEND_RD));
    sc_trace(tf, MEM_SIZE_RD, GET_NAME(MEM_SIZE_RD));  // tells to mem if we do an acces in word, hw or byte

    sc_trace(tf, CSR_WENABLE_RD, GET_NAME(CSR_WENABLE_RD));  // indicate if we do a csr operation,
                                                             // if so need to WBK CSR in rd
    sc_trace(tf, CSR_WADR_RD, GET_NAME(CSR_WADR_RD));        // CSR adress sent to EXE, will allow to wbk csr in MEM
    sc_trace(tf, CSR_RDATA_RD, GET_NAME(CSR_RDATA_RD));      // CSR read data to be wb in register

    // Interface with DEC2IF :

    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));  // Ifecth say to decod if it wants a pop or no
    sc_trace(tf, DEC2IF_EMPTY_SD, GET_NAME(DEC2IF_EMPTY_SD));
    sc_trace(tf, PC_RD_S1, GET_NAME(PC_RD_S1));  // this value must also be sent to REG
    sc_trace(tf, PC_RD_S2, GET_NAME(PC_RD_S2));  // this value must also be sent to REG
    sc_trace(tf, dec2if_pc_sd_s1, GET_NAME(dec2if_pc_sd_s1));  
    sc_trace(tf, dec2if_pc_sd_s2, GET_NAME(dec2if_pc_sd_s2));  
    sc_trace(tf, dec2if_in_sd, GET_NAME(dec2if_in_sd));  
    
    // Interface with IF2DEC :

    sc_trace(tf, PC_IF2DEC_RI_S1, GET_NAME(PC_IF2DEC_RI_S1));
    sc_trace(tf, INSTR_RI_S1, GET_NAME(INSTR_RI_S1));
    sc_trace(tf, IF2DEC_EMPTY_SI, GET_NAME(IF2DEC_EMPTY_SI));
    sc_trace(tf, IF2DEC_POP_SD, GET_NAME(IF2DEC_POP_SD));  // Decod says to IFETCH if it wants a pop or no
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));

    // Interface with DEC2EXE

    sc_trace(tf, DEC2EXE_POP_SE, GET_NAME(DEC2EXE_POP_SE));
    sc_trace(tf, DEC2EXE_EMPTY_SD, GET_NAME(DEC2EXE_EMPTY_SD));
    sc_trace(tf, dec2exe_out_sd, GET_NAME(dec2exe_out_sd));
    sc_trace(tf, MULT_INST_RD, GET_NAME(MULT_INST_RD));
    
    // Interface with CSR :

    sc_trace(tf, CSR_RADR_SD, GET_NAME(CSR_RADR_SD));    // CSR adress sent to CSR to get data
    sc_trace(tf, CSR_RDATA_SC, GET_NAME(CSR_RDATA_SC));  // data read from CSR

    // Bypasses

    sc_trace(tf, BP_DEST_RE, GET_NAME(BP_DEST_RE));
    sc_trace(tf, BP_EXE_RES_RE, GET_NAME(BP_EXE_RES_RE));
    sc_trace(tf, BP_MEM_LOAD_RE, GET_NAME(BP_MEM_LOAD_RE));
    sc_trace(tf, BP_EXE2MEM_EMPTY_SE, GET_NAME(BP_EXE2MEM_EMPTY_SE));
    sc_trace(tf, BP_MEM2WBK_EMPTY_SM, GET_NAME(BP_MEM2WBK_EMPTY_SM));
    sc_trace(tf, BP_DEST_RM, GET_NAME(BP_DEST_RM));
    sc_trace(tf, BP_MEM_RES_RM, GET_NAME(BP_MEM_RES_RM));
    sc_trace(tf, MULT_INST_RE, GET_NAME(MULT_INST_RE));
    sc_trace(tf, MULT_INST_RM, GET_NAME(MULT_INST_RM));
    
    sc_trace(tf, CSR_WENABLE_RE, GET_NAME(CSR_WENABLE_RE));
    sc_trace(tf, CSR_RDATA_RE, GET_NAME(CSR_RDATA_RE));
    sc_trace(tf, CSR_WENABLE_RM, GET_NAME(CSR_WENABLE_RM));
    sc_trace(tf, CSR_RDATA_RM, GET_NAME(CSR_RDATA_RM));

    sc_trace(tf, BP_R1_VALID_RD, GET_NAME(BP_R1_VALID_RD));
    sc_trace(tf, BP_R2_VALID_RD, GET_NAME(BP_R2_VALID_RD));
    sc_trace(tf, BP_RADR1_RD, GET_NAME(BP_RADR1_RD));
    sc_trace(tf, BP_RADR2_RD, GET_NAME(BP_RADR2_RD));

    // Exception :

    sc_trace(tf,
             EXCEPTION_RI,
             GET_NAME(EXCEPTION_RI));  // this signal will be at 0 considering there is no exception in IFETCH

    sc_trace(tf, ENV_CALL_U_MODE_RD, GET_NAME(ENV_CALL_U_MODE_RD));
    sc_trace(tf, ENV_CALL_WRONG_MODE_RD, GET_NAME(ENV_CALL_WRONG_MODE_RD));
    sc_trace(tf, ILLEGAL_INSTRUCTION_RD, GET_NAME(ILLEGAL_INSTRUCTION_RD));  // instruction doesnt exist
    sc_trace(tf, ADRESS_MISSALIGNED_RD, GET_NAME(ADRESS_MISSALIGNED_RD));    // branch offset is misaligned
    sc_trace(tf, ENV_CALL_S_MODE_RD, GET_NAME(ENV_CALL_S_MODE_RD));
    sc_trace(tf, ENV_CALL_M_MODE_RD, GET_NAME(ENV_CALL_M_MODE_RD));

    sc_trace(tf, EXCEPTION_RD, GET_NAME(EXCEPTION_RD));
    // General Interface :

    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, MTVEC_VALUE_RC, GET_NAME(MTVEC_VALUE_RC));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET_N, GET_NAME(RESET_N));
    sc_trace(tf, INTERRUPTION_SE, GET_NAME(INTERRUPTION_SE));
    sc_trace(tf, rdata1_sd, GET_NAME(rdata1_sd));
    sc_trace(tf, rdata2_sd, GET_NAME(rdata2_sd));
    sc_trace(tf, r1_valid_sd, GET_NAME(r1_valid_sd));
    sc_trace(tf, r2_valid_sd, GET_NAME(r2_valid_sd));
    sc_trace(tf, stall_sd, GET_NAME(stall_sd));
    sc_trace(tf, block_in_dec, GET_NAME(block_in_dec));
    sc_trace(tf, dec2if_pc_sd_s1, GET_NAME(dec2if_pc_sd_s1));
    sc_trace(tf, dec2if_push_sd, GET_NAME(dec2if_push_sd));
    sc_trace(tf, dec2if_empty_sd, GET_NAME(dec2if_empty_sd));
    sc_trace(tf, dec2if_full_sd, GET_NAME(dec2if_full_sd));
    sc_trace(tf, dec2if_out_sd, GET_NAME(dec2if_out_sd));
    sc_trace(tf, dec2exe_in_sd, GET_NAME(dec2exe_in_sd));
    sc_trace(tf, dec2exe_push_sd, GET_NAME(dec2exe_push_sd));
    sc_trace(tf, dec2exe_full_sd, GET_NAME(dec2exe_full_sd));
    sc_trace(tf, r_type_inst_sd, GET_NAME(r_type_inst_sd));        // R type format
    sc_trace(tf, i_type_inst_sd, GET_NAME(i_type_inst_sd));        // I type format
    sc_trace(tf, s_type_inst_sd, GET_NAME(s_type_inst_sd));        // S type format
    sc_trace(tf, b_type_inst_sd, GET_NAME(b_type_inst_sd));        // B type format
    sc_trace(tf, u_type_inst_sd, GET_NAME(u_type_inst_sd));        // U type format
    sc_trace(tf, j_type_inst_sd, GET_NAME(j_type_inst_sd));        // J type format
    sc_trace(tf, m_type_inst_sd, GET_NAME(m_type_inst_sd));        // M type format
    sc_trace(tf, jalr_type_inst_sd, GET_NAME(jalr_type_inst_sd));  // JALR has a specific opcode
    sc_trace(tf, system_type_inst_sd, GET_NAME(system_type_inst_sd));
    sc_trace(tf, add_i_sd, GET_NAME(add_i_sd));
    sc_trace(tf, slt_i_sd, GET_NAME(slt_i_sd));
    sc_trace(tf, sltu_i_sd, GET_NAME(sltu_i_sd));
    sc_trace(tf, and_i_sd, GET_NAME(and_i_sd));
    sc_trace(tf, or_i_sd, GET_NAME(or_i_sd));
    sc_trace(tf, xor_i_sd, GET_NAME(xor_i_sd));
    sc_trace(tf, sll_i_sd, GET_NAME(sll_i_sd));
    sc_trace(tf, srl_i_sd, GET_NAME(srl_i_sd));
    sc_trace(tf, sub_i_sd, GET_NAME(sub_i_sd));
    sc_trace(tf, sra_i_sd, GET_NAME(sra_i_sd));
    sc_trace(tf, addi_i_sd, GET_NAME(addi_i_sd));
    sc_trace(tf, slti_i_sd, GET_NAME(slti_i_sd));
    sc_trace(tf, sltiu_i_sd, GET_NAME(sltiu_i_sd));
    sc_trace(tf, andi_i_sd, GET_NAME(andi_i_sd));
    sc_trace(tf, ori_i_sd, GET_NAME(ori_i_sd));
    sc_trace(tf, xori_i_sd, GET_NAME(xori_i_sd));
    sc_trace(tf, jalr_i_sd, GET_NAME(jalr_i_sd));
    sc_trace(tf, slli_i_sd, GET_NAME(slli_i_sd));
    sc_trace(tf, srli_i_sd, GET_NAME(srli_i_sd));
    sc_trace(tf, srai_i_sd, GET_NAME(srai_i_sd));
    sc_trace(tf, lw_i_sd, GET_NAME(lw_i_sd));
    sc_trace(tf, lh_i_sd, GET_NAME(lh_i_sd));
    sc_trace(tf, lhu_i_sd, GET_NAME(lhu_i_sd));
    sc_trace(tf, lb_i_sd, GET_NAME(lb_i_sd));
    sc_trace(tf, lbu_i_sd, GET_NAME(lbu_i_sd));
    sc_trace(tf, beq_i_sd, GET_NAME(beq_i_sd));
    sc_trace(tf, bne_i_sd, GET_NAME(bne_i_sd));
    sc_trace(tf, blt_i_sd, GET_NAME(blt_i_sd));
    sc_trace(tf, bge_i_sd, GET_NAME(bge_i_sd));
    sc_trace(tf, bltu_i_sd, GET_NAME(bltu_i_sd));
    sc_trace(tf, bgeu_i_sd, GET_NAME(bgeu_i_sd));
    sc_trace(tf, lui_i_sd, GET_NAME(lui_i_sd));
    sc_trace(tf, auipc_i_sd, GET_NAME(auipc_i_sd));
    sc_trace(tf, jal_i_sd, GET_NAME(jal_i_sd));
    sc_trace(tf, sw_i_sd, GET_NAME(sw_i_sd));
    sc_trace(tf, sh_i_sd, GET_NAME(sh_i_sd));
    sc_trace(tf, sb_i_sd, GET_NAME(sb_i_sd));
    sc_trace(tf, mul_i_sd, GET_NAME(mul_i_sd));
    sc_trace(tf, mulh_i_sd, GET_NAME(mulh_i_sd));
    sc_trace(tf, mulhsu_i_sd, GET_NAME(mulhsu_i_sd));
    sc_trace(tf, mulhu_i_sd, GET_NAME(mulhu_i_sd));
    sc_trace(tf, div_i_sd, GET_NAME(div_i_sd));
    sc_trace(tf, divu_i_sd, GET_NAME(divu_i_sd));
    sc_trace(tf, rem_i_sd, GET_NAME(rem_i_sd));
    sc_trace(tf, remu_i_sd, GET_NAME(remu_i_sd));
    sc_trace(tf, csrrw_i_sd, GET_NAME(csrrw_i_sd));
    sc_trace(tf, csrrs_i_sd, GET_NAME(csrrs_i_sd));
    sc_trace(tf, csrrc_i_sd, GET_NAME(csrrc_i_sd));
    sc_trace(tf, csrrwi_i_sd, GET_NAME(csrrwi_i_sd));
    sc_trace(tf, csrrsi_i_sd, GET_NAME(csrrsi_i_sd));
    sc_trace(tf, csrrci_i_sd, GET_NAME(csrrci_i_sd));
    sc_trace(tf, csr_wenable_sd, GET_NAME(csr_wenable_sd));
    sc_trace(tf, csr_radr_sd, GET_NAME(csr_radr_sd));
    sc_trace(tf, offset_branch_sd, GET_NAME(offset_branch_sd));
    sc_trace(tf, csr_in_progress, GET_NAME(csr_in_progress));
    sc_trace(tf, mret_i_sd, GET_NAME(mret_i_sd));
    sc_trace(tf, sret_i_sd, GET_NAME(sret_i_sd));

    // PC gestion :

    sc_trace(tf, inc_pc_sd, GET_NAME(inc_pc_sd));
    sc_trace(tf, jump_sd, GET_NAME(jump_sd));

    // Internal signals :

    sc_trace(tf, adr_dest_sd, GET_NAME(adr_dest_sd));
    sc_trace(tf, exe_op1_sd, GET_NAME(exe_op1_sd));
    sc_trace(tf, exe_op2_sd, GET_NAME(exe_op2_sd));
    sc_trace(tf, mem_data_sd, GET_NAME(mem_data_sd));

    sc_trace(tf, mem_size_sd, GET_NAME(mem_size_sd));
    sc_trace(tf, mem_load_sd, GET_NAME(mem_load_sd));
    sc_trace(tf, mem_store_sd, GET_NAME(mem_store_sd));

    sc_trace(tf, exe_cmd_sd, GET_NAME(exe_cmd_sd));
    sc_trace(tf, select_type_operations_sd, GET_NAME(select_type_operations_sd));
    sc_trace(tf, exe_neg_op2_sd, GET_NAME(exe_neg_op2_sd));
    sc_trace(tf, exe_wb_sd, GET_NAME(exe_wb_sd));
    sc_trace(tf, mem_sign_extend_sd, GET_NAME(mem_sign_extend_sd));

    // Exception :

    sc_trace(tf, ecall_i_sd, GET_NAME(ecall_i_sd));
    sc_trace(tf, ebreak_i_sd, GET_NAME(ebreak_i_sd));
    sc_trace(tf, illegal_instruction_sd, GET_NAME(illegal_instruction_sd));  // instruction doesnt exist
    sc_trace(tf,
             instruction_adress_missaligned_sd,
             GET_NAME(instruction_adress_missaligned_sd));  // branch offset is misaligned
    sc_trace(tf, env_call_m_mode_sd, GET_NAME(env_call_m_mode_sd));
    sc_trace(tf, env_call_s_mode_sd, GET_NAME(env_call_s_mode_sd));
    sc_trace(tf, CURRENT_MODE_SM, GET_NAME(CURRENT_MODE_SM));
    sc_trace(tf, MRET_RD, GET_NAME(MRET_RD));
    sc_trace(tf, MRET_SM, GET_NAME(MRET_SM));
    sc_trace(tf, RETURN_ADRESS_SM, GET_NAME(RETURN_ADRESS_SM));
    sc_trace(tf, env_call_u_mode_sd, GET_NAME(env_call_u_mode_sd));
    sc_trace(tf, instruction_access_fault_sd, GET_NAME(instruction_access_fault_sd));
    sc_trace(tf, INSTRUCTION_ACCESS_FAULT_RD, GET_NAME(INSTRUCTION_ACCESS_FAULT_RD));
    sc_trace(tf, MCAUSE_WDATA_SM, GET_NAME(MCAUSE_WDATA_SM));
    sc_trace(tf, env_call_wrong_mode, GET_NAME(env_call_wrong_mode));
    sc_trace(tf, PC_BRANCH_VALUE_RD, GET_NAME(PC_BRANCH_VALUE_RD));
    sc_trace(tf, pc_branch_value_sd, GET_NAME(pc_branch_value_sd));
}