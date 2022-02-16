
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
#1
    instruction sts?
	mips1(premier version de mips 1981) diff de mips32
	Eret instruction il revient au debut de l'appel avant exception 
	avant (mips1) appeler rfe

	RFE => restore status
	Eret => restore status + saut a EPC (@ sauvegarde quand on est
	partie vers exception)

	- les status register est modifer a l'etage MEM
	- eret est appeler a l'etage DECOD (lors qu'on execute un eret on supprime l'instruction suivant), apres eret on a 3 nop*


	SI RISCV on supprim toujour les instruction apres branchment car no delayed slot.

	sc_signal<sc_uint<32> > RSTORSR_SM;  	// next ins sts (rfe)
	sc_signal<sc_uint<32> > NEXTSR_SM;  	// next ins sts (sw)
	sc_signal<sc_uint<32> > NEXTSR_XX;  	// next ins sts (hw it)
	sc_signal<sc_uint<32> > NEXTSR_RX;  	// next ins sts reg.
#1											


    sc_signal<bool> BOOTEV_XX;  			// bootstrap exc vect
	sc_signal<sc_uint<32> > EPC_XX;  		// exc pg counter
	sc_signal<sc_uint<32> > EPC_XM;  		// exc pg counter
	sc_signal<bool> WEPC_XX;  				// exc pg cntr write en
	sc_signal<bool> WEPC_XM;  				// exc pg cntr write en
	sc_signal<sc_uint<32> > EPC_RX;  		// exc pg counter reg

    sc_in<sc_uint<32> > 	CAUSE_RX;

#2
	// ecriture dans les reg low & high 2 reg 32 bits pour la mult et div
	// dans cette version il sont que utiliser pour mult et div logiciel mais
	// on peut faire un hardware avec
	emulation d'instruction
	on ajout l'opcode mult qui n'est pas une instruction, quand le processeur la
	lit va generait une exception(unknown instruction) donc on va verifier les handler d'exception qui pour notre cas va trouver qu'il existe un handler 
	multiplication brancher vers une "fonction" qui va ecrire sont reulta sur low & high et apres il va returner a l'instruction mult du code de base

    sc_signal<bool> WLO_SW;  				// low reg's write en
	sc_signal<sc_uint<32> > MUX_LO_SW;  	// low register
	sc_signal<sc_uint<32> > LO_RW;  		// low register
	sc_signal<bool> WHI_SW;  				// high reg's write en
	sc_signal<sc_uint<32> > MUX_HI_SW;  	// high register
	sc_signal<sc_uint<32> > HI_RW;  		// high register
#2
#3
	source reg nbr => le numero du registre source mis dans un registre
	le numero de r4 c'est 4
    sc_signal<sc_uint<5> > RS_SD;  			// source reg nbr
	sc_signal<sc_uint<5> > RT_SD;  			// source reg nbr
	sc_signal<sc_uint<5> > MUX_RT_SD;  		// source reg nbr reg.
	sc_signal<sc_uint<5> > RT_RD;  			// source reg nbr reg.
	sc_signal<sc_uint<5> > MUX_RS_SD;  		// source reg nbr reg.
	sc_signal<sc_uint<5> > RS_RD;  			// source reg nbr reg.
    // diff between reg and reg. 
#3
	sc_signal<sc_uint<4> > BYTSEL_SM;  		// byte select for rw, rw?
	sc_signal<sc_uint<24> > BSEXT_SM;  		// data sign ext byte, sign ext?
	sc_signal<sc_uint<32> > DATA_SM;  		// data bus / res
decode.h
	sc_signal<bool> 		IMDSGN_SD;		// ?
	sc_signal<sc_uint<16> >	IMDSEX_SD;		// ?
	sc_out<sc_uint<32> >	IOPER_SD;		
    "effective" c'est la valeur de sortie apres bypass, entre soper et le resultat de l'instruction envoyer via bypass

   	sc_signal<sc_uint<32> > SEQADR_SD;		// seq @ ?

mux_decod.h
	dans decod pc est calculer de diff façon pc +4, +I*4, Rs, exception, etc....

    sc_in<sc_uint<32> > NEXTPC_XX;		// next instruction @ (hw)
    hw ? hardware

    sc_signal<sc_uint<32> > SOPER_RD;  		// eff. s oper. reg.

    oper. => operand

execute.h
    sc_in<sc_uint<32> > 	NEXTSR_RX;		// next instruction sts ?
	// la valeur qui s'ecrit dans le meme registre a la fin du cycle



	un registre de pipeline sont de 2 nature: 
	R. command(opcode) ou R. donnes(toper ou soper)
	- dans les registre donnes on peut faire 2 action:
	 	- soit conserver la donne (keep)
		- soit charger une nouvell information. (!keep)
	- sur un reg de type command on peut faire 3 action:
		- je conserve la command (hold)
		- je supprime la command (ex: Eret) (bubble donc inser nop*)
		- j'ajout une nouvelle command (!hold et !bubble)