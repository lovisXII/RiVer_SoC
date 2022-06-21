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
    sc_trace_file* tf;
    tf = sc_create_vcd_trace_file("tf");

    cout << "Testing component \"Diviseur\"..." << endl;

    Diviseur unit_divider("Diviseur");

    // declare signals
    sc_signal<sc_uint<32>> op1, op2, quo, rem;
    sc_signal<sc_uint<4>>  op;
    sc_signal<sc_uint<2>>  cmd;

    sc_signal<bool> busy, done, start;

    sc_clock        clk("clk", 1, SC_NS);
    // link shifter with signals

    unit_divider.OP1_SE(op1);
    unit_divider.OP2_SE(op2);

    unit_divider.CMD_RD(cmd);
    unit_divider.START_SE(start);

    unit_divider.DIVIDER_RES_OUTPUT(quo);

    unit_divider.BUSY_SE(busy);
    unit_divider.DONE_SE(done);

    unit_divider.CLK(clk);

    unit_divider.trace(tf);
    // run one tick of simulation
    sc_start(1, SC_NS);

    int select_op = 8;
    int i;
    cmd = 1;
    for (i = 0; i < 10000; i++) {
        // compute random values for the inputs
        int in1 = rand()%10+1;
        int in2 = rand()%10+1;

        int sign1 = rand()%2?-1:1; 
        in1 *= sign1;
        int sign2 = rand()%2?-1:1; 
        in2 *= sign2;

        int real_quo = in1 / in2; 
        int real_rem = in1 - in2*(in1 / in2);

        cout << "testing "<<in1<<"/"<<in2<<" = "<<real_quo <<"   rem: "<<real_rem<<endl;
        // assign shifter input, and simulate

        op1 = in1;
        op2 = in2;

        start = true;

        do
        {
            sc_start(1, SC_NS);
            start = false;
        }while(!done);

        // check is result is right
        if (sc_uint<32>(quo) != real_quo) {
            cout << "Test failed !" << endl;

            cout << "op1 : ";
            print_bits(in1);
            cout << endl;

            cout << "op2 : ";
            print_bits(in2);
            cout << endl;

            cout << "quotient : ";
            print_bits((int)quo.read());
            cout << endl;

            cout << "real quo : ";
            print_bits(real_quo);
            cout << endl;

            cout << "reminder : ";
            print_bits((int)rem.read());
            cout << endl;

            cout << "real rem : ";
            print_bits(real_rem);
            cout << endl;

            exit(0);
        }
    }
    cout << i << " random tests sucessfully passed !" << endl;
    return 0;
}