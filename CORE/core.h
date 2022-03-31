#include "systemc.h"
#include "DEC/dec.h"
#include "EXE/exec.h"
#include "IFETCH/ifetch.h"
#include "MEM/mem.h"
#include "REG/reg.h"
#include "WBK/wbk.h"


SC_MODULE(core)
{
    //Global Interface :

    sc_in_clk                   CLK;
    sc_in<bool>                 RESET;

    //IFETCH-DEC interface
        // DEC2IF : 

    sc_signal< bool >           DEC2IF_POP_SI ; 
    sc_signal< bool >           DEC2IF_EMPTY_SI ;
    sc_signal< sc_bv<32> >      PC_RD ;
    
        // IF2DEC :
    
    sc_signal< sc_bv<32> >      INSTR_RI ;
    sc_signal< bool >           IF2DEC_EMPTY_SI ;
    sc_signal< bool >           IF2DEC_POP_SD ; 
    sc_signal< bool >           IF2DEC_FLUSH_SD ; 
    sc_signal<sc_bv<32>>           PC_OUT_RI ;
    //DEC-EXE interface
    
    sc_signal< sc_bv<32> >    PC_OUT_SD ;
    sc_signal< sc_uint<32> >    OP1_RD ;
    sc_signal< sc_uint<32> >    OP2_RD ;
    sc_signal< sc_uint<2> >     EXE_CMD_RD ;
    sc_signal< bool >           NEG_OP2_RD ;
    sc_signal< bool >           WB_RD ;
    sc_signal< bool >           SELECT_SHIFT_RD ;
    
    sc_signal< sc_uint<32> >    MEM_DATA_RD ;
    sc_signal< bool>            MEM_LOAD_RD ;
    sc_signal< bool >           MEM_STORE_RD ;
    sc_signal< bool >           MEM_SIGN_EXTEND_RD ; 
    sc_signal< sc_uint<2> >     MEM_SIZE_RD ;
    sc_signal< bool >           SLT_RD ;
    sc_signal< bool >           SLTU_RD ;

    sc_signal< bool >           DEC2EXE_POP_SE ;
    sc_signal< bool >           DEC2EXE_EMPTY_SD ;   

    sc_signal  < bool >         BP_R1_VALID_RD ;
    sc_signal  < bool >         BP_R2_VALID_RD ;
    sc_signal  < sc_uint<6> >   BP_RADR1_RD ;
    sc_signal  < sc_uint<6> >   BP_RADR2_RD ;                 

    //DEC-REG interface
    sc_signal< sc_uint<32> >    RDATA1_SR ; 
    sc_signal< sc_uint<32> >    RDATA2_SR ;

    sc_signal< sc_uint<6> >     RADR1_SD ;
    sc_signal< sc_uint<6> >     RADR2_SD ;

    sc_signal< sc_uint<6> >     EXE_DEST_SD ;

    sc_signal< sc_uint<32> >    READ_PC_SR ;
    sc_signal< sc_uint<32> >    WRITE_PC_SD ;
    sc_signal< bool >           WRITE_PC_ENABLE_SD ;

    //EXE-MEM interface
    sc_signal< sc_uint<32> >    EXE_RES_RE ;
    sc_signal< sc_uint<32> >    MEM_DATA_RE;
    sc_signal< sc_uint<6> >     DEST_RE;
    sc_signal< sc_uint<2> >     MEM_SIZE_RE ;

    sc_signal< bool >           MEM_WB,  
                                MEM_SIGN_EXTEND_RE ;
    sc_signal< bool >           MEM_LOAD_RE, 
                                MEM_STORE_RE ; 
    sc_signal< bool >           EXE2MEM_EMPTY_SE, 
                                EXE2MEM_POP_SM;

    //MEM-WBK interface
    sc_signal< sc_uint<32> >    MEM_RES_RM;
    sc_signal< sc_uint<6> >     DEST_RM;
    sc_signal< bool >           WB_RM;
    sc_signal< bool >           WBK_MEM_SIGN_EXTEND;
    sc_signal< bool >           MEM2WBK_EMPTY_SM;
    sc_signal< bool >           MEM2WBK_POP_SW;
    sc_signal< bool >           WBK_MEM_LOAD;

    //WBK-REG interface
    sc_signal< sc_uint<6> >     WADR_SW ;
    sc_signal< sc_uint<32> >    WDATA_SW ;
    sc_signal< bool >           WENABLE_SW ;

    //Mcache interface
    sc_out< sc_uint<32> >       MCACHE_ADR_SM ;
    sc_out< sc_uint<32> >       MCACHE_DATA_SM ;
    sc_out< bool>               MCACHE_ADR_VALID_SM,
                                MCACHE_STORE_SM,
                                MCACHE_LOAD_SM ; 

    sc_in< sc_uint<32> >        MCACHE_RESULT_SM ;
    sc_in< bool>                MCACHE_STALL_SM ;

    //Icache interface
    sc_out< sc_uint<32> >       ADR_SI ; 
    sc_out< bool >              ADR_VALID_SI ; 

    sc_in< sc_bv<32> >          IC_INST_SI ;
    sc_in< bool >               IC_STALL_SI ;

    //Debug
    sc_in< sc_uint<32> >       PC_INIT;
    sc_out< sc_uint<32> >      DEBUG_PC_READ;


    //Stage instanciation
    decod   dec_inst;
    exec    exec_inst;
    ifetch  ifetch_inst;
    mem     mem_inst;
    reg     reg_inst;
    wbk     wbk_inst;

    void core_method();

    void trace(sc_trace_file* tf);
    SC_CTOR(core) : 
        dec_inst("decod"), 
        exec_inst("exec"), 
        ifetch_inst("ifetch"), 
        mem_inst("mem"), 
        reg_inst("reg"), 
        wbk_inst("wbk")
    {
        SC_METHOD(core_method);
        sensitive << READ_PC_SR;
            
        ifetch_inst.DEC2IF_POP_SI(DEC2IF_POP_SI);
        ifetch_inst.DEC2IF_EMPTY_SI(DEC2IF_EMPTY_SI);
        ifetch_inst.PC_RD(PC_RD);
        ifetch_inst.INSTR_RI(INSTR_RI);
        ifetch_inst.IF2DEC_EMPTY_SI(IF2DEC_EMPTY_SI);
        ifetch_inst.IF2DEC_POP_SD(IF2DEC_POP_SD);
        ifetch_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH_SD);

        ifetch_inst.ADR_SI(ADR_SI);
        ifetch_inst.ADR_VALID_SI(ADR_VALID_SI);

        ifetch_inst.IC_INST_SI(IC_INST_SI);
        ifetch_inst.IC_STALL_SI(IC_STALL_SI);


        ifetch_inst.PC_OUT_RI(PC_OUT_RI);

        ifetch_inst.CLK(CLK);
        ifetch_inst.RESET(RESET);


        dec_inst.DEC2IF_POP_SI(DEC2IF_POP_SI);
        dec_inst.DEC2IF_EMPTY_SD(DEC2IF_EMPTY_SI);
        dec_inst.PC_RD(PC_RD);
        dec_inst.PC_OUT_SD(PC_OUT_SD);
        dec_inst.PC_CURRENT_INST_RI(PC_OUT_RI);
        dec_inst.INSTR_RI(INSTR_RI);
        dec_inst.IF2DEC_EMPTY_SI(IF2DEC_EMPTY_SI);
        dec_inst.IF2DEC_POP_SD(IF2DEC_POP_SD);
        dec_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH_SD);

        dec_inst.OP1_RD(OP1_RD);
        dec_inst.OP2_RD(OP2_RD);
        dec_inst.EXE_CMD_RD(EXE_CMD_RD);
        dec_inst.NEG_OP2_RD(NEG_OP2_RD);
        dec_inst.WB_RD(WB_RD);
        dec_inst.SELECT_SHIFT_RD(SELECT_SHIFT_RD);
        dec_inst.SLT_RD(SLT_RD);
        dec_inst.SLTU_RD(SLTU_RD);
        
        dec_inst.MEM_DATA_RD(MEM_DATA_RD);
        dec_inst.MEM_LOAD_RD(MEM_LOAD_RD);
        dec_inst.MEM_STORE_RD(MEM_STORE_RD);
        dec_inst.MEM_SIGN_EXTEND_RD(MEM_SIGN_EXTEND_RD);
        dec_inst.MEM_SIZE_RD(MEM_SIZE_RD);

        dec_inst.DEC2EXE_POP_SE(DEC2EXE_POP_SE);
        dec_inst.DEC2EXE_EMPTY_SD(DEC2EXE_EMPTY_SD);


        dec_inst.RDATA1_SR(RDATA1_SR);
        dec_inst.RDATA2_SR(RDATA2_SR);


        dec_inst.RADR1_SD(RADR1_SD);
        dec_inst.RADR2_SD(RADR2_SD);

        dec_inst.EXE_DEST_SD(EXE_DEST_SD);

        dec_inst.READ_PC_SR(READ_PC_SR);
        dec_inst.WRITE_PC_SD(WRITE_PC_SD);
        dec_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE_SD);

        dec_inst.BP_DEST_RE(DEST_RE);
        dec_inst.BP_EXE_RES_RE(EXE_RES_RE);
        dec_inst.BP_DEST_RM(DEST_RM);
        dec_inst.BP_MEM_RES_RM(MEM_RES_RM);
        dec_inst.BP_EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);

        dec_inst.BP_R1_VALID_RD(BP_R1_VALID_RD);
        dec_inst.BP_R2_VALID_RD(BP_R2_VALID_RD);
        dec_inst.BP_RADR1_RD(BP_RADR1_RD);
        dec_inst.BP_RADR2_RD(BP_RADR2_RD);
        dec_inst.BP_MEM_LOAD_RE(MEM_LOAD_RE);

        dec_inst.CLK(CLK);
        dec_inst.RESET_N(RESET);

        exec_inst.RADR1_RD(BP_RADR1_RD);
        exec_inst.RADR2_RD(BP_RADR2_RD);
        exec_inst.OP1_VALID_RD(BP_R1_VALID_RD);
        exec_inst.OP2_VALID_RD(BP_R2_VALID_RD);
        exec_inst.MEM_DEST_RM(DEST_RM);
        exec_inst.MEM_RES_RM(MEM_RES_RM);
        exec_inst.OP1_RD(OP1_RD);
        exec_inst.OP2_RD(OP2_RD);
        exec_inst.CMD_RD(EXE_CMD_RD);
        exec_inst.DEST_RD(EXE_DEST_SD);
        exec_inst.NEG_OP2_RD(NEG_OP2_RD);
        exec_inst.WB_RD(WB_RD);
        exec_inst.SELECT_SHIFT_RD(SELECT_SHIFT_RD);
        
        exec_inst.MEM_DATA_RD(MEM_DATA_RD);
        exec_inst.MEM_LOAD_RD(MEM_LOAD_RD);
        exec_inst.MEM_STORE_RD(MEM_STORE_RD);
        exec_inst.MEM_SIGN_EXTEND_RD(MEM_SIGN_EXTEND_RD);
        exec_inst.MEM_SIZE_RD(MEM_SIZE_RD);
        exec_inst.SLT_RD(SLT_RD);
        exec_inst.SLTU_RD(SLTU_RD);

        exec_inst.DEC2EXE_POP_SE(DEC2EXE_POP_SE);
        exec_inst.DEC2EXE_EMPTY_SD(DEC2EXE_EMPTY_SD);

        exec_inst.EXE_RES_RE(EXE_RES_RE);
        exec_inst.MEM_DATA_RE(MEM_DATA_RE);
        exec_inst.DEST_RE(DEST_RE);
        exec_inst.MEM_SIZE_RE(MEM_SIZE_RE);

        exec_inst.WB_RE(MEM_WB);
        exec_inst.MEM_SIGN_EXTEND_RE(MEM_SIGN_EXTEND_RE) ;
        exec_inst.MEM_LOAD_RE(MEM_LOAD_RE);
        exec_inst.MEM_STORE_RE(MEM_STORE_RE) ; 
        exec_inst.EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);
        exec_inst.EXE2MEM_POP_SM(EXE2MEM_POP_SM);

        exec_inst.CLK(CLK);
        exec_inst.RESET(RESET);


        mem_inst.EXE_RES_RE(EXE_RES_RE);
        mem_inst.MEM_DATA_RE(MEM_DATA_RE);
        mem_inst.DEST_RE(DEST_RE);
        mem_inst.MEM_SIZE_RE(MEM_SIZE_RE);

        mem_inst.WB_RE(MEM_WB);
        mem_inst.SIGN_EXTEND_RE(MEM_SIGN_EXTEND_RE) ;
        mem_inst.LOAD_RE(MEM_LOAD_RE);
        mem_inst.STORE_RE(MEM_STORE_RE) ; 
        mem_inst.EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY_SE);
        mem_inst.EXE2MEM_POP_SM(EXE2MEM_POP_SM);

        mem_inst.MEM_RES_RM(MEM_RES_RM);
        mem_inst.DEST_RM(DEST_RM);
        mem_inst.WB_RM(WB_RM);
        mem_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
        mem_inst.MEM2WBK_POP_SW(MEM2WBK_POP_SW);

        mem_inst.MCACHE_ADR_SM(MCACHE_ADR_SM);
        mem_inst.MCACHE_DATA_SM(MCACHE_DATA_SM);
        mem_inst.MCACHE_ADR_VALID_SM(MCACHE_ADR_VALID_SM);
        mem_inst.MCACHE_STORE_SM(MCACHE_STORE_SM);
        mem_inst.MCACHE_LOAD_SM(MCACHE_LOAD_SM); 
        mem_inst.MCACHE_RESULT_SM(MCACHE_RESULT_SM);
        mem_inst.MCACHE_STALL_SM(MCACHE_STALL_SM);

        mem_inst.CLK(CLK);
        mem_inst.RESET(RESET);

        reg_inst.RADR1_SD(RADR1_SD) ;
        reg_inst.RADR2_SD(RADR2_SD) ;
        reg_inst.RDATA1_SR(RDATA1_SR) ;
        reg_inst.RDATA2_SR(RDATA2_SR) ;

        reg_inst.WADR_SW(WADR_SW) ;
        reg_inst.WENABLE_SW(WENABLE_SW) ;
        reg_inst.WDATA_SW(WDATA_SW) ;


        reg_inst.READ_PC_SR(READ_PC_SR) ;
        reg_inst.WRITE_PC_SD(WRITE_PC_SD);
        reg_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE_SD);
        reg_inst.PC_INIT(PC_INIT);

        reg_inst.CLK(CLK);
        reg_inst.RESET_N(RESET);

        wbk_inst.MEM_RES_RM(MEM_RES_RM);
        wbk_inst.DEST_RM(DEST_RM);
        wbk_inst.WB_RM(WB_RM);
        wbk_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY_SM);
        wbk_inst.MEM2WBK_POP_SW(MEM2WBK_POP_SW);

        wbk_inst.WADR_SW(WADR_SW);
        wbk_inst.WDATA_SW(WDATA_SW);
        wbk_inst.WENABLE_SW(WENABLE_SW);

        wbk_inst.CLK(CLK);
        wbk_inst.RESET(RESET);

    }
};