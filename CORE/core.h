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

    sc_signal< bool >           DEC2IF_POP ; 
    sc_signal< bool >           DEC2IF_EMPTY ;
    sc_signal< sc_bv<32> >      DEC2IF_PC ;

        // IF2DEC :
    
    sc_signal< sc_bv<32> >      IF_IR ;
    sc_signal< bool >           IF2DEC_EMPTY ;
    sc_signal< bool >           IF2DEC_POP ; 
    sc_signal< bool >           IF2DEC_FLUSH ; 

    //DEC-EXE interface
    
    sc_signal< sc_uint<32> >    EXE_OP1 ;
    sc_signal< sc_uint<32> >    EXE_OP2 ;
    sc_signal< sc_uint<2> >     EXE_CMD ;
    sc_signal< bool >           EXE_NEG_OP2 ;
    sc_signal< bool >           EXE_WB ;
    sc_signal< bool >           EXE_SELECT_SHIFT ;
    
    sc_signal< sc_uint<32> >    EXE_MEM_DATA ;
    sc_signal< bool>            EXE_MEM_LOAD ;
    sc_signal< bool >           EXE_MEM_STORE ;
    sc_signal< bool >           EXE_MEM_SIGN_EXTEND ; 
    sc_signal< sc_uint<2> >     EXE_MEM_SIZE ;
    sc_signal< bool >           EXE_SLT ;
    sc_signal< bool >           EXE_SLTU ;

    sc_signal< bool >           DEC2EXE_POP ;
    sc_signal< bool >           DEC2EXE_EMPTY ;   

    sc_signal  < bool >         BP_R1_VALID ;
    sc_signal  < bool >         BP_R2_VALID ;
    sc_signal  < sc_uint<6> >   BP_RADR1 ;
    sc_signal  < sc_uint<6> >   BP_RADR2 ;                 

    //DEC-REG interface
    sc_signal< sc_uint<32> >    RADR1_DATA ; 
    sc_signal< sc_uint<32> >    RADR2_DATA ;
    sc_signal< bool >           RADR1_VALID ;
    sc_signal< bool >           RADR2_VALID ;
    
    sc_signal <bool>            ADR_DEST_VALID ;
    sc_signal< sc_uint<6>>      ADR_DEST_DECOD ;


    sc_signal< sc_uint<6> >     RADR1 ;
    sc_signal< sc_uint<6> >     RADR2 ;

    sc_signal< sc_uint<6> >     EXE_DEST ;
    sc_signal< sc_uint<6> >     INVAL_DEST ;
    sc_signal< bool >           INVAL_ENABLE ;

    sc_signal< sc_uint<32> >    READ_PC ;
    sc_signal< bool >           READ_PC_VALID ;
    sc_signal< sc_uint<32> >    WRITE_PC ;
    sc_signal< bool >           WRITE_PC_ENABLE ;

    //EXE-MEM interface
    sc_signal< sc_uint<32> >    MEM_EXE_RES ;
    sc_signal< sc_uint<32> >    MEM_DATA;
    sc_signal< sc_uint<6> >     MEM_DEST;
    sc_signal< sc_uint<2> >     MEM_SIZE ;

    sc_signal< bool >           MEM_WB,  
                                MEM_SIGN_EXTEND ;
    sc_signal< bool >           MEM_LOAD, 
                                MEM_STORE ; 
    sc_signal< bool >           EXE2MEM_EMPTY, 
                                EXE2MEM_POP;

    //MEM-WBK interface
    sc_signal< sc_uint<32> >    WBK_DATA;
    sc_signal< sc_uint<6> >     WBK_DEST;
    sc_signal< sc_uint<2> >     WBK_MEM_SIZE ;
    sc_signal< bool >           WBK_WB;
    sc_signal< bool >           WBK_MEM_SIGN_EXTEND;
    sc_signal< bool >           MEM2WBK_EMPTY;
    sc_signal< bool >           MEM2WBK_POP;
    sc_signal< bool >           WBK_MEM_LOAD;

    //WBK-REG interface
    sc_signal< sc_uint<6> >     REG_DEST ;
    sc_signal< sc_uint<32> >    REG_DATA ;
    sc_signal< bool >           REG_WB ;

    //Mcache interface
    sc_out< sc_uint<32> >       MCACHE_MEM_ADR ;
    sc_out< sc_uint<32> >       MCACHE_MEM_DATA ;
    sc_out< bool>               MCACHE_MEM_ADR_VALID,
                                MCACHE_MEM_STORE,
                                MCACHE_MEM_LOAD ; 

    sc_in< sc_uint<32> >        MCACHE_MEM_RESULT ;
    sc_in< bool>                MCACHE_MEM_STALL ;

    //Icache interface
    sc_out< sc_uint<32> >       IF_ADR ; 
    sc_out< bool >              IF_ADR_VALID ; 

    sc_in< sc_bv<32> >          IC_INST ;
    sc_in< bool >               IC_STALL ;

    //Debug
    sc_in< sc_uint<32> >       DEBUG_PC_RESET;
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
        sensitive << READ_PC;
            
        ifetch_inst.DEC2IF_POP_SI(DEC2IF_POP);
        ifetch_inst.DEC2IF_EMPTY_SI(DEC2IF_EMPTY);
        ifetch_inst.PC_SI(DEC2IF_PC);
        ifetch_inst.INSTR_SI(IF_IR);
        ifetch_inst.IF2DEC_EMPTY_SI(IF2DEC_EMPTY);
        ifetch_inst.IF2DEC_POP_SI(IF2DEC_POP);
        ifetch_inst.IF2DEC_FLUSH_SI(IF2DEC_FLUSH);

        ifetch_inst.ADR_SI(IF_ADR);
        ifetch_inst.ADR_VALID_SI(IF_ADR_VALID);

        ifetch_inst.IC_INST_SI(IC_INST);
        ifetch_inst.IC_STALL_SI(IC_STALL);

        ifetch_inst.CLK(CLK);
        ifetch_inst.RESET(RESET);


        dec_inst.DEC2IF_POP_SD(DEC2IF_POP);
        dec_inst.DEC2IF_EMPTY_SD(DEC2IF_EMPTY);
        dec_inst.IF_PC_SD(DEC2IF_PC);
        dec_inst.INSTR_SD(IF_IR);
        dec_inst.IF2DEC_EMPTY_SD(IF2DEC_EMPTY);
        dec_inst.IF2DEC_POP_SD(IF2DEC_POP);
        dec_inst.IF2DEC_FLUSH_SD(IF2DEC_FLUSH);

        dec_inst.EXE_OP1_SD(EXE_OP1);
        dec_inst.EXE_OP2_SD(EXE_OP2);
        dec_inst.EXE_CMD_SD(EXE_CMD);
        dec_inst.EXE_NEG_OP2_SD(EXE_NEG_OP2);
        dec_inst.EXE_WB_SD(EXE_WB);
        dec_inst.EXE_SELECT_SHIFT_SD(EXE_SELECT_SHIFT);
        
        dec_inst.MEM_DATA_SD(EXE_MEM_DATA);
        dec_inst.MEM_LOAD_SD(EXE_MEM_LOAD);
        dec_inst.MEM_STORE_SD(EXE_MEM_STORE);
        dec_inst.MEM_SIGN_EXTEND_SD(EXE_MEM_SIGN_EXTEND);
        dec_inst.MEM_SIZE_SD(EXE_MEM_SIZE);
        dec_inst.EXE_SLT_SD(EXE_SLT);
        dec_inst.EXE_SLTU_SD(EXE_SLTU);

        dec_inst.DEC2EXE_POP_SD(DEC2EXE_POP);
        dec_inst.DEC2EXE_EMPTY_SD(DEC2EXE_EMPTY);

        dec_inst.INVAL_DEST_SD(INVAL_DEST);
        dec_inst.INVAL_ENABLE_SD(INVAL_ENABLE);

        dec_inst.IN_RDATA1_SD(RADR1_DATA);
        dec_inst.IN_RDATA2_SD(RADR2_DATA);
        dec_inst.IN_R1_VALID_SD(RADR1_VALID);
        dec_inst.IN_R2_VALID_SD(RADR2_VALID);

        dec_inst.ADR_DEST_VALID_SD(ADR_DEST_VALID) ;
        dec_inst.ADR_DEST_SD(ADR_DEST_DECOD) ;

        dec_inst.RADR1_SD(RADR1);
        dec_inst.RADR2_SD(RADR2);

        dec_inst.EXE_DEST_SD(EXE_DEST);

        dec_inst.READ_PC_SD(READ_PC);
        dec_inst.READ_PC_VALID_SD(READ_PC_VALID);
        dec_inst.WRITE_PC_SD(WRITE_PC);
        dec_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE);

        dec_inst.BP_EXE_DEST_SD(MEM_DEST);
        dec_inst.BP_EXE_RES_SD(MEM_EXE_RES);
        dec_inst.BP_MEM_DEST_SD(WBK_DEST);
        dec_inst.BP_MEM_RES_SD(WBK_DATA);

        dec_inst.BP_R1_VALID_SD(BP_R1_VALID);
        dec_inst.BP_R2_VALID_SD(BP_R2_VALID);
        dec_inst.BP_RADR1_SD(BP_RADR1);
        dec_inst.BP_RADR2_SD(BP_RADR2);

        dec_inst.CLK(CLK);
        dec_inst.RESET_N(RESET);

        exec_inst.RADR1_SE(BP_RADR1);
        exec_inst.RADR2_SE(BP_RADR2);
        exec_inst.OP1_VALID_SE(BP_R1_VALID);
        exec_inst.OP2_VALID_SE(BP_R2_VALID);
        exec_inst.MEM_DEST_SE(WBK_DEST);
        exec_inst.MEM_RES_SE(WBK_DATA);
        exec_inst.IN_OP1_SE(EXE_OP1);
        exec_inst.IN_OP2_SE(EXE_OP2);
        exec_inst.CMD_SE(EXE_CMD);
        exec_inst.IN_DEST_SE(EXE_DEST);
        exec_inst.NEG_OP2_SE(EXE_NEG_OP2);
        exec_inst.IN_WB_SE(EXE_WB);
        exec_inst.SELECT_SHIFT_SE(EXE_SELECT_SHIFT);
        
        exec_inst.IN_MEM_DATA_SE(EXE_MEM_DATA);
        exec_inst.IN_MEM_LOAD_SE(EXE_MEM_LOAD);
        exec_inst.IN_MEM_STORE_SE(EXE_MEM_STORE);
        exec_inst.IN_MEM_SIGN_EXTEND_SE(EXE_MEM_SIGN_EXTEND);
        exec_inst.IN_MEM_SIZE_SE(EXE_MEM_SIZE);
        exec_inst.SLT_SE(EXE_SLT);
        exec_inst.SLTU_SE(EXE_SLTU);

        exec_inst.DEC2EXE_POP_SE(DEC2EXE_POP);
        exec_inst.DEC2EXE_EMPTY_SE(DEC2EXE_EMPTY);

        exec_inst.EXE_RES_SE(MEM_EXE_RES);
        exec_inst.OUT_MEM_DATA_SE(MEM_DATA);
        exec_inst.OUT_DEST_SE(MEM_DEST);
        exec_inst.OUT_MEM_SIZE_SE(MEM_SIZE);

        exec_inst.OUT_WB_SE(MEM_WB);
        exec_inst.OUT_MEM_SIGN_EXTEND_SE(MEM_SIGN_EXTEND) ;
        exec_inst.OUT_MEM_LOAD_SE(MEM_LOAD);
        exec_inst.OUT_MEM_STORE_SE(MEM_STORE) ; 
        exec_inst.EXE2MEM_EMPTY_SE(EXE2MEM_EMPTY);
        exec_inst.EXE2MEM_POP_SE(EXE2MEM_POP);

        exec_inst.CLK(CLK);
        exec_inst.RESET(RESET);


        mem_inst.EXE_RES_SM(MEM_EXE_RES);
        mem_inst.MEM_DATA_SM(MEM_DATA);
        mem_inst.EXE_DEST_SM(MEM_DEST);
        mem_inst.EXE_MEM_SIZE_SM(MEM_SIZE);

        mem_inst.EXE_WB_SM(MEM_WB);
        mem_inst.SIGN_EXTEND_SM(MEM_SIGN_EXTEND) ;
        mem_inst.LOAD_SM(MEM_LOAD);
        mem_inst.STORE_SM(MEM_STORE) ; 
        mem_inst.EXE2MEM_EMPTY_SM(EXE2MEM_EMPTY);
        mem_inst.EXE2MEM_POP_SM(EXE2MEM_POP);

        mem_inst.WBK_DATA_SM(WBK_DATA);
        mem_inst.WBK_DEST_SM(WBK_DEST);
        mem_inst.WBK_MEM_SIZE_SM(WBK_MEM_SIZE);
        mem_inst.WBK_WB_SM(WBK_WB);
        mem_inst.WBK_SIGN_EXTEND_SM(WBK_MEM_SIGN_EXTEND);
        mem_inst.MEM2WBK_EMPTY_SM(MEM2WBK_EMPTY);
        mem_inst.MEM2WBK_POP_SM(MEM2WBK_POP);
        mem_inst.WBK_LOAD_SM(WBK_MEM_LOAD);

        mem_inst.MCACHE_ADR_SM(MCACHE_MEM_ADR);
        mem_inst.MCACHE_DATA_SM(MCACHE_MEM_DATA);
        mem_inst.MCACHE_ADR_VALID_SM(MCACHE_MEM_ADR_VALID);
        mem_inst.MCACHE_STORE_SM(MCACHE_MEM_STORE);
        mem_inst.MCACHE_LOAD_SM(MCACHE_MEM_LOAD); 
        mem_inst.MCACHE_RESULT_SM(MCACHE_MEM_RESULT);
        mem_inst.MCACHE_STALL_SM(MCACHE_MEM_STALL);

        mem_inst.CLK(CLK);
        mem_inst.RESET(RESET);

        reg_inst.RADR1_SD(RADR1) ;
        reg_inst.RADR2_SD(RADR2) ;
        reg_inst.R1_VALID_SD(RADR1_VALID) ;
        reg_inst.R2_VALID_SD(RADR2_VALID) ;
        reg_inst.RDEST_VALID_SD(ADR_DEST_VALID) ;
        reg_inst.RDEST_SD(ADR_DEST_DECOD) ;
        reg_inst.RDATA1_SD(RADR1_DATA) ;
        reg_inst.RDATA2_SD(RADR2_DATA) ;

        reg_inst.WADR_SD(REG_DEST) ;
        reg_inst.WENABLE_SD(REG_WB) ;
        reg_inst.WDATA_SD(REG_DATA) ;

        reg_inst.INVAL_ADR_SD(INVAL_DEST) ;
        reg_inst.INVAL_ENABLE_SD(INVAL_ENABLE) ;

        reg_inst.READ_PC_SD(READ_PC) ;
        reg_inst.READ_PC_VALID_SD(READ_PC_VALID) ;
        reg_inst.WRITE_PC_SD(WRITE_PC);
        reg_inst.WRITE_PC_ENABLE_SD(WRITE_PC_ENABLE);
        reg_inst.PC_INIT(DEBUG_PC_RESET);

        reg_inst.CLK(CLK);
        reg_inst.RESET_N(RESET);

        wbk_inst.MEM_DATA_SW(WBK_DATA);
        wbk_inst.MEM_DEST_SW(WBK_DEST);
        wbk_inst.MEM_SIZE_SW(WBK_MEM_SIZE);
        wbk_inst.MEM_WB_SW(WBK_WB);
        wbk_inst.SIGN_EXTEND_SW(WBK_MEM_SIGN_EXTEND);
        wbk_inst.MEM2WBK_EMPTY_SW(MEM2WBK_EMPTY);
        wbk_inst.MEM2WBK_POP_SW(MEM2WBK_POP);
        wbk_inst.LOAD_SW(WBK_MEM_LOAD);

        wbk_inst.REG_DEST_SW(REG_DEST);
        wbk_inst.REG_DATA_SW(REG_DATA);
        wbk_inst.REG_WB_SW(REG_WB);

        wbk_inst.CLK(CLK);
        wbk_inst.RESET(RESET);

    }
};