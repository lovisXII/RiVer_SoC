#include <cstdlib>
#include <string>
#include "Diviseur.h"
using namespace std;

// a function to print the bits of any value
template <typename T>
void print_bits(T ptr) {
    size_t         size = sizeof(T);
    unsigned char* b    = (unsigned char*)&ptr;
    unsigned char  byte;
    int            i, j;

    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            cout << (int)byte;
        }
    }
}

int sc_main(int argc, char* argv[]) {
    cout << "Testing component \"Diviseur\"..." << endl;

    Diviseur unit_divider("Diviseur");

    cout << "??"<<endl;
    // declare signals
    sc_signal<sc_uint<32>> op1, op2, dout;
    sc_signal<sc_uint<4>>  op;
    sc_signal<sc_uint<2>>  cmd;

    sc_signal<bool> busy, done;

    sc_clock        clk("clk", 1, SC_NS);
    // link shifter with signals

    unit_divider.OP1_SE(op1);
    unit_divider.OP2_SE(op2);
    unit_divider.DIV_RES_SE(dout);

    unit_divider.SELECT_TYPE_OPERATIONS_RD(op);
    unit_divider.EXE_CMD_RD(cmd);

    unit_divider.BUSY_SE(busy);
    unit_divider.DONE_SE(done);

    unit_divider.CLK(clk);
    
    // run one tick of simulation
    sc_start(1, SC_NS);


    int select_op = 8;
    int exe_cmd = 1;
    int i;
    for (i = 0; i < 1000; i++) {
        // compute random values for the inputs
        int in1 = rand();
        int in2 = rand();

        int div_dout = in1 / in2; 
        cout << "testing: "<<in1<<" / "<<in2 << " = "<<div_dout<<endl;
        // assign shifter input, and simulate

        op1 = in1;
        op2 = in2;
        op = select_op;
        cmd = exe_cmd;


        do
        {
            sc_start(1, SC_NS);
            //cout << "busy : "<<busy.read()<<endl;
        }while(!done);

        // check is result is right

        if (sc_uint<32>(dout) != div_dout) {
            cout << "Test failed !" << endl;

            cout << "op1 : ";
            print_bits(in1);
            cout << endl;

            cout << "op2 : ";
            print_bits(in2);
            cout << endl;

            cout << "dout : ";
            print_bits((int)dout.read());
            cout << endl;

            cout << "real : ";
            print_bits(div_dout);
            cout << endl;

            exit(0);
        }
    }
    cout << i << " random tests sucessfully passed !" << endl;
    return 0;
}