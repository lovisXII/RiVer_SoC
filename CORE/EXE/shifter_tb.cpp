#include "shifter.h"
#include <cstdlib>
using namespace std;
int sc_main(int argc, char* argv[])
{
    printf("Testing component \"Shifter\"...\n");
    sc_signal< sc_uint<32> >  shifter_din, shifter_dout ;
    sc_signal< sc_uint<5> >  shifter_shift_val ;
    sc_signal< bool > shifter_sll, shifter_srl, shifter_sra ;
    shifter unit_shifter("shifter") ;
    unit_shifter.DIN(shifter_din);
    unit_shifter.DOUT(shifter_dout);
    unit_shifter.SHIFT_VAL(shifter_shift_val);
    unit_shifter.SLL(shifter_sll);
    unit_shifter.SRL(shifter_srl);
    unit_shifter.SRA(shifter_sra);
    sc_start(1, SC_NS);
    int i;
    for (i = 0; i < 1000; i++) {
        int sv = rand() % 16;
        int din = rand();
        int dout;
        bool sll = false;
        bool srl = false;
        bool sra = false;
        switch (rand() % 3) {
            case 0: 
                sll = true; 
                dout = din << sv;
                break; 
            case 1: 
                srl = true; 
                dout = ((unsigned int) din) >> sv;
                break; 
            case 2: 
                sra = true; 
                dout = din >> sv;
                break; 
        }
        shifter_din = din; 
        shifter_shift_val = sv;
        shifter_sll = sll;
        shifter_srl = srl;
        shifter_sra = sra;
        sc_start(1, SC_NS);
        if (sc_uint<32>(dout) == shifter_dout) {
        }
    }
    printf("%i random tests sucessfully passed !\n", i);
    return 0 ;
}