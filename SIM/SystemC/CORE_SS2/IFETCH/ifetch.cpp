#include "ifetch.h"

void ifetch::fetch_method() {
    sc_bv<if2dec_size> if2dec_in_var_s1;
    sc_bv<if2dec_size> if2dec_in_var_s2;
    sc_bv<if2dec_size> instr_ri_var_s1 = instr_si_s1.read();
    sc_bv<if2dec_size> instr_ri_var_s2 = instr_si_s2.read();
    if (EXCEPTION_SM.read() == 0) {

        ADR_SI_S1 = PC_DEC2IF_RD_S1.read();
        ADR_SI_S2 = PC_DEC2IF_RD_S2.read();

        // data sent in if2dec
        if2dec_in_var_s2.range(63, 32) = (sc_bv_base)IC_INST_SI_S2;
        if2dec_in_var_s2.range(31, 0)  = (sc_bv_base)PC_DEC2IF_RD_S2     ;

        if2dec_in_var_s1.range(63, 32) = (sc_bv_base)IC_INST_SI_S1 ;
        if2dec_in_var_s1.range(31, 0)  = (sc_bv_base)PC_DEC2IF_RD_S1      ;
        
        if2dec_in_si_s2    = if2dec_in_var_s2;
        if2dec_in_si_s1    = if2dec_in_var_s1;      


        // data coming out from if2dec :
        // We must check which pipeline is prioritary 
        // If there is an inversion, meaning S1 prio on S2
        // then we load if2dec_s1 inside INST_S2 and 
        // if2dec_s2 in INST_S1
        if(!PRIORITARY_PIPELINE_RD.read()){
            INSTR_RI_S2     = (sc_bv_base)instr_ri_var_s2.range(63, 32);
            PC_IF2DEC_RI_S2 = (sc_bv_base)instr_ri_var_s2.range(31, 0) ;
            
            INSTR_RI_S1     = (sc_bv_base)instr_ri_var_s1.range(63, 32) ;
            PC_IF2DEC_RI_S1 = (sc_bv_base)instr_ri_var_s1.range(31, 0)  ;
        }        
        else{            
            INSTR_RI_S1     = (sc_bv_base)instr_ri_var_s2.range(63, 32);
            PC_IF2DEC_RI_S1 = (sc_bv_base)instr_ri_var_s2.range(31, 0) ;
            
            INSTR_RI_S2     = (sc_bv_base)instr_ri_var_s1.range(63, 32) ;
            PC_IF2DEC_RI_S2 = (sc_bv_base)instr_ri_var_s1.range(31, 0)  ;
        }


        if (IF2DEC_FLUSH_SD.read()) {
            
            if2dec_push_si_s1.write(false);
            if2dec_push_si_s2.write(false);

            DEC2IF_POP_SI.write(true);
            
            ADR_VALID_SI_S1.write(false);
            ADR_VALID_SI_S2.write(false);
        } else {
            // stall if the memory stalls, if we can't push to dec, or have no value
            // of pc to pop from dec
            bool stall = IC_STALL_SI.read() || if2dec_full_si_s1.read() || if2dec_full_si_s2 || DEC2IF_EMPTY_SI.read() ;
            if2dec_push_si_s1.write(!stall);
            if2dec_push_si_s2.write(!stall);
            DEC2IF_POP_SI.write(!stall);
            ADR_VALID_SI_S1.write(!DEC2IF_EMPTY_SI.read());
            ADR_VALID_SI_S2.write(!DEC2IF_EMPTY_SI.read());
        }

    } else {
        // data sent in if2dec
        // If an exception is detected
        // Pipeline pass in M-mode
        // Fifo send nop instruction 

        if2dec_in_var_s2.range(63, 32) = nop_encoding;
        if2dec_in_var_s2.range(31, 0)  = (sc_bv_base)PC_DEC2IF_RD_S2.read();

        if2dec_in_var_s1.range(63, 32) = nop_encoding;
        if2dec_in_var_s1.range(31, 0)  = (sc_bv_base)PC_DEC2IF_RD_S1.read();
        
        ADR_SI_S1 = PC_DEC2IF_RD_S1.read();
        ADR_SI_S2 = PC_DEC2IF_RD_S2.read();


        if2dec_in_si_s1.write(if2dec_in_var_s1);  
        if2dec_in_si_s2.write(if2dec_in_var_s2);  
        
        // data coming out from if2dec :

        INSTR_RI_S1.write((sc_bv_base)instr_ri_var_s1.range(63, 32));
        PC_IF2DEC_RI_S1.write((sc_bv_base)instr_ri_var_s1.range(31, 0));


        INSTR_RI_S2.write((sc_bv_base)instr_ri_var_s2.range(63, 32));
        PC_IF2DEC_RI_S2.write((sc_bv_base)instr_ri_var_s2.range(31, 0));

        if2dec_push_si_s1.write(true);
        if2dec_push_si_s2.write(true);

        DEC2IF_POP_SI.write(true);
        
        ADR_VALID_SI_S1.write(false);
        ADR_VALID_SI_S2.write(false);
    }

}


void ifetch::exception()  
// No exception in IFECTH
// Gestion of current mode, start in Machine mode
// Then keep the same mode as the pipeline 
{
    EXCEPTION_RI.write(0);
}


void ifetch::trace(sc_trace_file* tf) {
    sc_trace(tf, ADR_SI_S1, GET_NAME(ADR_SI_S1));
    sc_trace(tf, ADR_VALID_SI_S1, GET_NAME(ADR_VALID_SI_S1));
    sc_trace(tf, IC_INST_SI_S1, GET_NAME(IC_INST_SI_S1));
    sc_trace(tf, ADR_SI_S2, GET_NAME(ADR_SI_S2));
    sc_trace(tf, ADR_VALID_SI_S2, GET_NAME(ADR_VALID_SI_S2));
    sc_trace(tf, IC_INST_SI_S2, GET_NAME(IC_INST_SI_S2));
    sc_trace(tf, IC_STALL_SI, GET_NAME(IC_STALL_SI));
    sc_trace(tf, DEC2IF_EMPTY_SI, GET_NAME(DEC2IF_EMPTY_SI));
    sc_trace(tf, DEC2IF_POP_SI, GET_NAME(DEC2IF_POP_SI));
    sc_trace(tf, IF2DEC_FLUSH_SD, GET_NAME(IF2DEC_FLUSH_SD));
    sc_trace(tf, IF2DEC_POP_SD_S1, GET_NAME(IF2DEC_POP_SD_S1));
    sc_trace(tf, if2dec_push_si_s1, GET_NAME(if2dec_push_si_s1));
    sc_trace(tf, if2dec_push_si_s2, GET_NAME(if2dec_push_si_s2));
    sc_trace(tf, if2dec_full_si_s1, GET_NAME(if2dec_full_si_s1));
    sc_trace(tf, if2dec_full_si_s2, GET_NAME(if2dec_full_si_s2));
    sc_trace(tf,PRIORITARY_PIPELINE_RD, GET_NAME(PRIORITARY_PIPELINE_RD));
    sc_trace(tf, IF2DEC_EMPTY_SI_S1, GET_NAME(IF2DEC_EMPTY_SI_S1));
    sc_trace(tf, IF2DEC_EMPTY_SI_S2, GET_NAME(IF2DEC_EMPTY_SI_S2));
    sc_trace(tf, PC_DEC2IF_RD_S1, GET_NAME(PC_DEC2IF_RD_S1));
    sc_trace(tf, PC_DEC2IF_RD_S2, GET_NAME(PC_DEC2IF_RD_S2));
    sc_trace(tf, INSTR_RI_S1, GET_NAME(INSTR_RI_S1));
    sc_trace(tf, INSTR_RI_S2, GET_NAME(INSTR_RI_S2));
    sc_trace(tf, PC_IF2DEC_RI_S1, GET_NAME(PC_IF2DEC_RI_S1));
    sc_trace(tf, CLK, GET_NAME(CLK));
    sc_trace(tf, RESET, GET_NAME(RESET));
    sc_trace(tf, if2dec_in_si_s1, GET_NAME(if2dec_in_si_s1));
    sc_trace(tf, if2dec_in_si_s2, GET_NAME(if2dec_in_si_s2));
    sc_trace(tf, instr_si_s1, GET_NAME(instr_si_s1));
    sc_trace(tf, instr_si_s2, GET_NAME(instr_si_s2));
    sc_trace(tf, EXCEPTION_RI, GET_NAME(EXCEPTION_RI));
    sc_trace(tf, EXCEPTION_SM, GET_NAME(EXCEPTION_SM));
    sc_trace(tf, INTERRUPTION_SE_S1, GET_NAME(INTERRUPTION_SE_S1));
    sc_trace(tf, MRET_SM, GET_NAME(MRET_SM));
    buffer_inst_1.trace(tf);
    buffer_inst_2.trace(tf);
}