
1. AGREEMENT FOR INPUT, OUTPUT or INNER SIGNALS DECLARATION

The agreement for declare a wire is compound by a name or 
an acronym, and a prefix connected by an underscore.

The prefix is compound by 2 capital letters, 
the wire type and the stage.

TYPES:

S: signal
R: registre
X: exception signal

STAGE:

I: ifetch  
D: decod  
E: exec  
M: memory  
W: writeback

EXAMPLE:

    "RES_SE"

    name  :  resultat   
    type  :  signal
    stage :  exec


// CLZ: count leading zero

2. STAGE AND COMPONENTS DESCRIPTION
In addition of all components files, "constants.h" contains all instructions encoding and special signals encoding.

2.1 ALU

The ALU are made for make logique operations (AND, OR, NOR and XOR),
arithmetique operations (addition and substraction), shift operations and slt compare operation.



DONE LIST: (# : done |  / : not done)
ALU         #
ifetch      #
mux_ifetch  #
ff_ifetch   #
decode      /
mux_decode  /
ff_decode   /
execute     /
mux_execute #
ff_execute  #
memory      /
mux_memory
ff_memory

Q:
mipsr3000
    instruction sts?
	sc_signal<sc_uint<32> > RSTORSR_SM;  	// next ins sts (rfe)
	sc_signal<sc_uint<32> > NEXTSR_SM;  	// next ins sts (sw)
	sc_signal<sc_uint<32> > NEXTSR_XX;  	// next ins sts (hw it)
	sc_signal<sc_uint<32> > NEXTSR_RX;  	// next ins sts reg.

    sc_signal<bool> BOOTEV_XX;  			// bootstrap exc vect
	sc_signal<sc_uint<32> > EPC_XX;  		// exc pg counter
	sc_signal<sc_uint<32> > EPC_XM;  		// exc pg counter
	sc_signal<bool> WEPC_XX;  				// exc pg cntr write en
	sc_signal<bool> WEPC_XM;  				// exc pg cntr write en
	sc_signal<sc_uint<32> > EPC_RX;  		// exc pg counter reg

    sc_in<sc_uint<32> > 	CAUSE_RX;

    sc_signal<bool> WLO_SW;  				// low reg's write en
	sc_signal<sc_uint<32> > MUX_LO_SW;  	// low register
	sc_signal<sc_uint<32> > LO_RW;  		// low register
	sc_signal<bool> WHI_SW;  				// high reg's write en
	sc_signal<sc_uint<32> > MUX_HI_SW;  	// high register
	sc_signal<sc_uint<32> > HI_RW;  		// high register

    sc_signal<sc_uint<5> > RS_SD;  			// source reg nbr
	sc_signal<sc_uint<5> > RT_SD;  			// source reg nbr
	sc_signal<sc_uint<5> > MUX_RT_SD;  		// source reg nbr reg.
	sc_signal<sc_uint<5> > RT_RD;  			// source reg nbr reg.
	sc_signal<sc_uint<5> > MUX_RS_SD;  		// source reg nbr reg.
	sc_signal<sc_uint<5> > RS_RD;  			// source reg nbr reg.
    // diff between reg and reg. 

	sc_signal<sc_uint<4> > BYTSEL_SM;  		// byte select for rw, rw?
	sc_signal<sc_uint<24> > BSEXT_SM;  		// data sign ext byte, sign ext?
	sc_signal<sc_uint<32> > DATA_SM;  		// data bus / res
decode.h
	sc_signal<bool> 		IMDSGN_SD;		// ?
	sc_signal<sc_uint<16> >	IMDSEX_SD;		// ?
	sc_out<sc_uint<32> >	IOPER_SD;		
    // eff. immediate operator ? effective ?

   	sc_signal<sc_uint<32> > SEQADR_SD;		// seq @ ?

mux_decod.h
    sc_in<sc_uint<32> > NEXTPC_XX;		// next instruction @ (hw)
    hw ?

    sc_signal<sc_uint<32> > SOPER_RD;  		// eff. s oper. reg.

    oper. => operand or operator?

execute.h
    sc_in<sc_uint<32> > 	NEXTSR_RX;		// next instruction sts ?