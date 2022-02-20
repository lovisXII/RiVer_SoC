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
    sc_signal< sc_uint<32> >    DEC2IF_PC ;

        // IF2DEC :

    sc_signal< sc_uint<32> >    IF_IR ;
    sc_signal< bool >           IF2DEC_EMPTY ;
    sc_signal< bool >           IF2DEC_POP ; 

    //DEC-EXE interface
    
    sc_signal< sc_uint<32> >    EXE_OP1 ;
    sc_signal< sc_uint<32> >    EXE_OP2 ;
    sc_signal< sc_uint<2> >     EXE_CMD ;
    sc_signal< bool >           EXE_NEG_OP1 ;
    sc_signal< bool >           EXE_WB ;
    sc_signal< bool >           EXE_SELECT_SHIFT ;
    
    sc_signal< sc_uint<32> >    EXE_MEM_DATA ;
    sc_signal< bool>            EXE_MEM_LOAD ;
    sc_signal< bool >           EXE_MEM_STORE ;
    sc_signal< bool >           EXE_MEM_SIGN_EXTEND ; 
    sc_signal< sc_uint<2> >     EXE_MEM_SIZE ;

    sc_signal< bool >           DEC2EXE_POP ;
    sc_signal< bool >           DEC2EXE_EMPTY ;                    

    //DEC-REG interface
    sc_signal< sc_uint<32> >    RADR1_DATA ; 
    sc_signal< sc_uint<32> >    RADR2_DATA ;
    sc_signal< bool >           RADR1_VALID ;
    sc_signal< bool >           RADR2_VALID ;

    sc_signal< sc_uint<6> >     RADR1 ;
    sc_signal< sc_uint<6> >     RADR2 ;

    sc_signal< sc_uint<6> >     ADR_DEST ;

    sc_signal< sc_uint<32> >    READ_PC ;
    sc_signal< bool >           INC_PC ;
    sc_signal< bool >           READ_PC_VALID ;

    //EXE-MEM interface
    sc_signal< sc_uint<32> >    MEM_EXE_RES ;
    sc_signal< sc_uint<32> >    MEM_DATA;
    sc_signal< sc_uint<4> >     MEM_DEST;
    sc_signal< sc_uint<2> >     MEM_SIZE ;

    sc_signal< bool >           MEM_WB,  
                                MEM_SIGN_EXTEND ;
    sc_signal< bool >           MEM_LOAD, 
                                MEM_STORE ; 
    sc_signal< bool >           EXE2MEM_EMPTY, 
                                EXE2MEM_POP;

    //MEM-WBK interface
    sc_signal< sc_uint<32> >    WBK_DATA;
    sc_signal< sc_uint<5> >     WBK_DEST;
    sc_signal< sc_uint<2> >     WBK_MEM_SIZE ;
    sc_signal< bool >           WBK_WB;
    sc_signal< bool >           WBK_MEM_SIGN_EXTEND;
    sc_signal< bool >           MEM2WBK_EMPTY;
    sc_signal< bool >           MEM2WBK_POP;

    //WBK-REG interface
    sc_signal< sc_uint<5> >     REG_DEST ;
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

    sc_in< sc_uint<32> >        IC_INST ;
    sc_in< bool >               IC_STALL ;


    //Stage instanciation
    decod   dec_inst;
    exec    exec_inst;
    ifetch  ifetch_inst;
    mem     mem_inst;
    reg     reg_inst;
    wbk     wbk_inst;

    void trace(sc_trace_file* tf);
    SC_CTOR(core) : 
        dec_inst("decod"), 
        exec_inst("exec"), 
        ifetch_inst("ifetch"), 
        mem_inst("mem"), 
        reg_inst("reg"), 
        wbk_inst("wbk")
    {

            
        ifetch_inst.DEC2IF_POP(DEC2IF_POP);
        ifetch_inst.DEC2IF_EMPTY(DEC2IF_EMPTY);
        ifetch_inst.DEC_PC(DEC2IF_PC);
        ifetch_inst.IF_IR(IF_IR);
        ifetch_inst.IF2DEC_EMPTY(IF2DEC_EMPTY);
        ifetch_inst.IF2DEC_POP(IF2DEC_POP);

        ifetch_inst.IF_ADR(IF_ADR);
        ifetch_inst.IF_ADR_VALID(IF_ADR_VALID);

        ifetch_inst.IC_INST(IC_INST);
        ifetch_inst.IC_STALL(IC_STALL);


        dec_inst.DEC2IF_POP(DEC2IF_POP);
        dec_inst.DEC2IF_EMPTY(DEC2IF_EMPTY);
        dec_inst.DEC2IF_PC(DEC2IF_PC);
        dec_inst.IF_IR(IF_IR);
        dec_inst.IF2DEC_EMPTY(IF2DEC_EMPTY);
        dec_inst.IF2DEC_POP(IF2DEC_POP);

        dec_inst.DEC2EXE_OP1(EXE_OP1);
        dec_inst.DEC2EXE_OP2(EXE_OP2);
        dec_inst.DEC2EXE_CMD(EXE_CMD);
        dec_inst.DEC2EXE_NEG_OP1(EXE_NEG_OP1);
        dec_inst.DEC2EXE_WB(EXE_WB);
        dec_inst.SELECT_SHIFT(EXE_SELECT_SHIFT);
        
        dec_inst.MEM_DATA(EXE_MEM_DATA);
        dec_inst.MEM_LOAD(EXE_MEM_LOAD);
        dec_inst.MEM_STORE(EXE_MEM_STORE);
        dec_inst.MEM_SIGN_EXTEND(EXE_MEM_SIGN_EXTEND);
        dec_inst.MEM_SIZE(EXE_MEM_SIZE);

        dec_inst.DEC2EXE_POP(DEC2EXE_POP);
        dec_inst.DEC2EXE_EMPTY(DEC2EXE_EMPTY);

        dec_inst.RADR1_DATA(RADR1_DATA);
        dec_inst.RADR2_DATA(RADR2_DATA);
        dec_inst.RADR1_VALID(RADR1_VALID);
        dec_inst.RADR2_VALID(RADR2_VALID);

        dec_inst.RADR1(RADR1);
        dec_inst.RADR2(RADR2);

        dec_inst.ADR_DEST(ADR_DEST);

        dec_inst.READ_PC(READ_PC);
        dec_inst.INC_PC(INC_PC);
        dec_inst.READ_PC_VALID(READ_PC_VALID);

        exec_inst.OP1(EXE_OP1);
        exec_inst.OP2(EXE_OP2);
        exec_inst.CMD(EXE_CMD);
        exec_inst.NEG_OP1(EXE_NEG_OP1);
        exec_inst.WB(EXE_WB);
        exec_inst.SELECT_SHIFT(EXE_SELECT_SHIFT);
        
        exec_inst.MEM_DATA(EXE_MEM_DATA);
        exec_inst.MEM_LOAD(EXE_MEM_LOAD);
        exec_inst.MEM_STORE(EXE_MEM_STORE);
        exec_inst.MEM_SIGN_EXTEND(EXE_MEM_SIGN_EXTEND);
        exec_inst.MEM_SIZE(EXE_MEM_SIZE);

        exec_inst.DEC2EXE_POP(DEC2EXE_POP);
        exec_inst.DEC2EXE_EMPTY(DEC2EXE_EMPTY);

        exec_inst.FFOUT_EXE_RES(MEM_EXE_RES);
        exec_inst.FFOUT_MEM_DATA(MEM_DATA);
        exec_inst.FFOUT_DEST(MEM_DEST);
        exec_inst.FFOUT_MEM_SIZE(MEM_SIZE);

        exec_inst.FFOUT_WB(MEM_WB);
        exec_inst.FFOUT_MEM_SIGN_EXTEND(MEM_SIGN_EXTEND) ;
        exec_inst.FFOUT_MEM_LOAD(MEM_LOAD);
        exec_inst.FFOUT_MEM_STORE(MEM_STORE) ; 
        exec_inst.EXE2MEM_EMPTY(EXE2MEM_EMPTY);
        exec_inst.EXE2MEM_POP(EXE2MEM_POP);


        mem_inst.EXE_RES(MEM_EXE_RES);
        mem_inst.MEM_DATA(MEM_DATA);
        mem_inst.EXE_DEST(MEM_DEST);
        mem_inst.EXE_MEM_SIZE(MEM_SIZE);

        mem_inst.EXE_WB(MEM_WB);
        mem_inst.EXE_MEM_SIGN_EXTEND(MEM_SIGN_EXTEND) ;
        mem_inst.EXE_MEM_LOAD(MEM_LOAD);
        mem_inst.EXE_MEM_STORE(MEM_STORE) ; 
        mem_inst.EXE2MEM_EMPTY(EXE2MEM_EMPTY);
        mem_inst.EXE2MEM_POP(EXE2MEM_POP);

        mem_inst.WBK_DATA(WBK_DATA);
        mem_inst.WBK_DEST(WBK_DEST);
        mem_inst.WBK_MEM_SIZE(WBK_MEM_SIZE);
        mem_inst.WBK_WB(WBK_WB);
        mem_inst.WBK_MEM_SIGN_EXTEND(WBK_MEM_SIGN_EXTEND);
        mem_inst.MEM2WBK_EMPTY(MEM2WBK_EMPTY);
        mem_inst.MEM2WBK_POP(MEM2WBK_POP);

        mem_inst.MEM_ADR(MCACHE_MEM_ADR);
        mem_inst.MEM_DATA(MCACHE_MEM_DATA);
        mem_inst.MEM_ADR_VALID(MCACHE_MEM_ADR_VALID);
        mem_inst.MEM_STORE(MCACHE_MEM_STORE);
        mem_inst.MEM_LOAD(MCACHE_MEM_LOAD); 
        mem_inst.MEM_RESULT(MCACHE_MEM_RESULT);
        mem_inst.MEM_STALL(MCACHE_MEM_STALL);

        reg_inst.RADR1(RADR1) ;
        reg_inst.RADR2(RADR2) ;
        reg_inst.RADR1_VALID(RADR1_VALID) ;
        reg_inst.RADR2_VALID(RADR2_VALID) ;
        reg_inst.RADR1_DATA(RADR1_DATA) ;
        reg_inst.RADR2_DATA(RADR2_DATA) ;

        reg_inst.WADR1(REG_DEST) ;
        reg_inst.WADR1_VALID(REG_WB) ;
        reg_inst.WADR1_DATA(REG_DATA) ;

        reg_inst.INVAL_ADR(ADR_DEST) ;
        reg_inst.INVAL_ENABLE() ;

        reg_inst.READ_PC(READ_PC) ;
        reg_inst.READ_PC_VALID(READ_PC_VALID) ;








        wbk_inst.MEM_DATA(WBK_DATA);
        wbk_inst.MEM_DEST(WBK_DEST);
        wbk_inst.MEM_SIZE(WBK_MEM_SIZE);
        wbk_inst.MEM_WB(WBK_WB);
        wbk_inst.MEM_SIGN_EXTEND(WBK_MEM_SIGN_EXTEND);
        wbk_inst.MEM2WBK_EMPTY(MEM2WBK_EMPTY);
        wbk_inst.MEM2WBK_POP(MEM2WBK_POP);

        wbk_inst.REG_DEST(REG_DEST);
        wbk_inst.REG_DATA(REG_DATA);
        wbk_inst.REG_WB(REG_WB);

    }
};