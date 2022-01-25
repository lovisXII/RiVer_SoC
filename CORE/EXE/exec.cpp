#include "exec.h"
#include "alu.h"
#include "shifter.h"


void exec::comp_op2() {
    sc_uint<32> op2 = OP2.read();
    if (NEG_OP2.read()) {
        ALU_IN_OP2.write(~op2);
    }
    else {
        ALU_IN_OP2.write(op2);
    }
}

void exec::select_exec_res() {
    sc_uint<32> alu_out = ALU_OUT.read();
    sc_uint<32> shifter_out = SHIFTER_OUT.read();
    if (SELECT_SHIFT.read()) {
        FF_EXE_RES.write(SHIFTER_OUT);
    }
    else {
        FF_EXE_RES.write(ALU_OUT);
    }

}

void exec::entry() {
    alu alu_inst("alu");
    shifter shift_inst("shifter");


    wait(3);

    while(1) {

    }
}