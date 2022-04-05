#include "alu.h"

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
    sc_signal<sc_uint<32> > op1, op2, res;
    sc_signal<sc_uint<2> >  cmd;
    sc_signal<bool>         cin;
    alu                     unit_alu("alu");
    unit_alu.OP1_SE(op1);
    unit_alu.OP2_SE(op2);
    unit_alu.CMD_SE(cmd);
    unit_alu.CIN_SE(cin);
    unit_alu.RES_SE(res);
    sc_start(1, SC_NS);
    for (int i = 0; i < 1000; i++) {
        int command = rand() % 3;
        int operand1, operand2;
        operand1 = rand();
        operand2 = rand();
        int result;
        int carry = rand() % 2;
        switch (command) {
            case 0: result = operand1 + operand2 + carry; break;
            case 1: result = operand1 & operand2; break;
            case 2: result = operand1 | operand2; break;
            case 3: result = operand1 ^ operand2; break;
        }
        op1 = operand1;
        op2 = operand2;
        cmd = command;
        cin = carry;
        sc_start(1, SC_NS);
        if (sc_uint<32>(result) == res) {
            std::cout << "test passed"
                      << " " << i << std::endl;
        } else {
            std::cout << "oh non gargamel" << i << std::endl;
            cout << "operand1    ";
            print_bits(operand1);
            cout << endl;
            cout << "operand2    ";
            print_bits(operand2);
            cout << endl;
            cout << "command     ";
            print_bits(command);
            cout << endl;
            cout << "result_code ";
            print_bits(result);
            cout << endl;
            cout << "result_alu  ";
            print_bits((int)res.read());
            cout << endl;
        }
    }
    printf("ok ta mere\n");
    return 0;
}