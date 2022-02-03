#include <systemc.h>

#include "../alu.h"

/* Clock generation function */
void next_cycle (sc_signal<bool> &signal_clk)
{
  signal_clk = false;
  sc_start(1, SC_NS);   //sc_cycle(1)
  signal_clk = true;
  sc_start(1, SC_NS);   //sc_cycle(1)
}

int sc_main(int ac, char** av)
{
    sc_signal<bool> CK;         // external clock
    sc_signal<bool> RESET_N;    // external reset

    sc_signal<sc_uint<32>>  opperandX;    //operand X
    sc_signal<sc_uint<32>> 	opperandY;    //operand Y

    sc_signal<bool>		      sub_sig;    //si 1 donc X - Y sinon X + Y
    sc_signal<sc_uint<3>>	  inst_type;   //type d'instruction (ti)
    sc_signal<bool>         rsgnd;  // ? 
    sc_signal<bool>         right;  // ?
    sc_signal<sc_uint<2>>	  command_alu;  //command de ALU

    sc_signal<sc_uint<32>>	ioper;    //result si ti est "nop"
    sc_signal<sc_uint<32>>	toper;        //result si ti est "toper"
    sc_signal<sc_uint<32>>	soper;        //result si ti est "soper"
    sc_signal<sc_uint<8>>	  opcod; 

    alu* alu_ = new alu("alu1");
    alu_->XOPER_SE(opperandX);
    alu_->YOPER_SE(opperandY);
    alu_->I_SUB_SE(sub_sig);
    alu_->I_OPER_SE(inst_type);
    alu_->I_RSGND_SE(rsgnd);
    alu_->I_RIGHT_SE(right);
    alu_->I_LOGIC_SE(command_alu);
    alu_->IOPER_RD(ioper);
    alu_->Y_SE(toper);
    alu_->X_SE(soper);
    alu_->OPCOD_RD(opcod);

    sc_trace_file *tf;
    tf = sc_create_vcd_trace_file("tf");
    sc_trace(tf, CK, "CK");
    sc_trace(tf, RESET_N, "reset");
    sc_trace(tf, opperandX, "XOPER");
    sc_trace(tf, opperandY, "YOPER");
    sc_trace(tf, sub_sig, "I_SUB_SE");
    sc_trace(tf, inst_type, "I_OPER_SE");
    sc_trace(tf, rsgnd, "I_RSGND");
    sc_trace(tf, right, "I_RIGHT_SE");
    sc_trace(tf, command_alu, "I_LOGIC_SE");
    sc_trace(tf, ioper, "IOPER_RD");
    sc_trace(tf, toper, "Y_SE");
    sc_trace(tf, soper, "X_SE");
    sc_trace(tf, opcod, "OPCOD_RD");


    sc_start(SC_ZERO_TIME); 
    RESET_N = false;

    next_cycle (CK); 
    next_cycle (CK);
    next_cycle (CK);
    RESET_N = true;

    //
    int opX;
    int opY;
    bool sub_s;
    int inst_type;
    bool rsgnd_;
    bool right_;
    int com_alu;
    int	ioper_;
    int	toper_;
    int	soper_;
    int opcod_;

    opperandX = opX;    
    opperandY = opY;    
    sub_sig = sub_s;  
    inst_type = inst_type;
    rsgnd = rsgnd_;  
    right = right_;  
    command_alu = com_alu;
    ioper = ioper_;
    toper = toper_;
    soper = soper_;
    opcod = opcod_; 

    sc_start(1, SC_NS);

    next_cycle(CK);
    //

    sc_close_vcd_trace_file(tf);
    return 0;
}

