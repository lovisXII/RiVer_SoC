#include "shifter.h"
#include <cstdlib>
#include <string>
using namespace std;

//a function to print the bits of any value
template<typename T>
void print_bits(T ptr) {
    size_t size = sizeof(T);
    unsigned char *b = (unsigned char*) &ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            cout << (int) byte;
        }
    }
}

int sc_main(int argc, char* argv[])
{
    cout << "Testing component \"Shifter\"..." << endl;

    shifter unit_shifter("shifter") ;

    //declare signals 
    sc_signal< sc_uint<32> >  shifter_din, shifter_dout ;
    sc_signal< sc_uint<5> >  shifter_shift_val ;
    sc_signal< sc_uint<2> > shifter_shift_cmd ;
    
    //link shifter with signals

    unit_shifter.DIN(shifter_din);
    unit_shifter.DOUT(shifter_dout);
    unit_shifter.CMD(shifter_shift_cmd) ;
    unit_shifter.SHIFT_VAL(shifter_shift_val);

    //run one tick of simulation
    sc_start(1, SC_NS);

    int i;
    for (i = 0; i < 1000; i++) {
        //compute random values for the inputs
        int sv = rand() % 16;
        int din = rand();
        int dout;
        int cmd = rand() % 2 ;
        
        switch (cmd) {
            case 0: 
                //compute the result in C
                dout = din << sv;
                break; 
            case 1: 
                //compute the result in C
                dout = ((unsigned int) din) >> sv;
                break; 
            case 2: 
                //compute the result in C
                dout = din >> sv;
                break; 
        }
        
        //assign shifter input, and simulate

        shifter_din = din; 
        shifter_shift_val = sv;
        shifter_shift_cmd = cmd ;

        sc_start(1, SC_NS);

        //check is result is right

        if (sc_uint<32>(dout) != shifter_dout) {
            
            cout << "Test failed !" << endl;
            
            cout << "din : ";
			print_bits(din);
			cout << endl;
            
            cout << "shift_val : ";
			print_bits(sv);
	        cout << endl;
            
            cout << "shifter command :" << cmd << endl ;
            
            cout << "shifter : ";
			print_bits((int) shifter_dout.read());
			cout << endl;
            
            cout << "real : ";
			print_bits(dout);
			cout << endl;
            
            exit(0);
        }
    }
    cout << i << " random tests sucessfully passed !" << endl;
    return 0 ;
}