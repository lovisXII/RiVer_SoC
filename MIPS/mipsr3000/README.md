1. ADVISE

All the information present on this report are a combination of suppositions and information given by Mr. Pirouz Bazargan Sabet but translated to this document well it can be bad understood well the information can be false or true, i recomend take the information with precaution and in case of doubt please go the information source so directly to Mr. Pirouz Bazargan Sabet, i'm open to receive critics and corrections.

2. MIPS STRUCTURE

This architecture is based on the mips1 and/or mips32 instruction set.
The architecture is a 5 stage pipelined, each stage are compound by 3 elements:
	1 -	The stage core wich is named like the stage, ex. decode.h
	2 - A multiplexer which is gonna handle the information flow, named 
		like the stage with the "mux" prefix, ex. mux_decode.h
	3 - A fifo (first in first out) which gonna litteraly just put the 
		input on the output, named like the stage with the "ff" prefix, ex. ff_decode.h
	4 - Exception : the alu is defined on the core but directtly connected
		to the stage execute.



3. AGREEMENT FOR INPUT, OUTPUT OR INNER SIGNALS DECLARATION

The agreement for declare a wire is compound by a name or 
an acronym, and a suffix connected by an underscore.

The suffix is compound by 2 capital letters, 
the wire type and the stage.

TYPES:

S: signal	(fils qui sort de tous elements different de la fifo)
R: registre (fils qui sort d'un registre d'une fifo)
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

	EMULATION D'INSTRUCTION

on ajout l'opcode mult qui n'est pas une instruction, quand le processeur va la lire va generait une exception(unknown instruction) donc on va verifier les handler d'exception qui pour notre cas va trouver qu'il existe un handler mult (multiplication) qui va brancher vers une "fonction" qui va
faire la multiplication que dans ce cas va ecrire sont reulta sur les registre low et high et apres il va returner a l'instruction mult du code de base.

DONE LIST: (# : done |  / : not done)
ALU         	#
ifetch      	#
mux_ifetch  	#
ff_ifetch   	#
decode      	/
mux_decode  	/
ff_decode   	/
execute     	/
mux_execute 	#
ff_execute  	#
memory      	/
mux_memory		#
ff_memory		#
writeback		#
mux_writeback	#
ff_writeback	#

Q:

# Pirouz hystories + technical explications zone <3
    1.  Mips1(premier version de mips 1981) diff de mips32
		Eret instruction il revient au debut de l'appel avant exception 
		avant (mips1) appeler rfe
	
		RFE => restore status
		Eret => restore status + saut a EPC (@ sauvegarde quand on est
		partie vers exception)
	
		- les status register est modifer a l'etage MEM
		- eret est appeler a l'etage DECOD (lors qu'on execute un eret on 	supprime l'instruction suivant), apres eret on a 3 nop*
	
	
		SI RISCV on supprim toujour les instruction apres branchment car no 	delayed slot.

	2.  Un registre de pipeline sont de 2 nature: 
		R. command(opcode) ou R. donnes(toper ou soper)
		- dans les registre donnes on peut faire 2 action:
		 	- soit conserver la donne (keep)
			- soit charger une nouvell information. (!keep)
		- sur un reg de type command on peut faire 3 action:
			- je conserve la command (hold)
			- je supprime la command (ex: Eret) (bubble donc inser nop*)
			- j'ajout une nouvelle command (!hold et !bubble)
	
	3.  Pc (a l'etage decod) est calculer de different façon 
		Pc +4, Pc+I*4, Rs, exception, etc....

	4. "effective" c'est la valeur de sortie apres bypass, entre soper et le resultat de l'instruction envoyer via bypass
#										
Q:
- mipsr3000
    sc_signal<bool> BOOTEV_XX;  			// bootstrap exc vect
	sc_signal<sc_uint<32> > EPC_XX;  		// exc pg counter
	sc_signal<sc_uint<32> > EPC_XM;  		// exc pg counter
	sc_signal<bool> WEPC_XX;  				// exc pg cntr write en
	sc_signal<bool> WEPC_XM;  				// exc pg cntr write en
	sc_signal<sc_uint<32> > EPC_RX;  		// exc pg counter reg

    sc_in<sc_uint<32> > 	CAUSE_RX;

	sc_signal<sc_uint<4> > BYTSEL_SM;  		// byte select for rw, rw?
	sc_signal<sc_uint<24> > BSEXT_SM;  		// data sign ext byte, sign ext?
	sc_signal<sc_uint<32> > DATA_SM;  		// data bus / res

- decode.h
	sc_signal<bool> 		IMDSGN_SD;		// ?
	sc_signal<sc_uint<16> >	IMDSEX_SD;		// ?
	sc_signal<sc_uint<32> >	OFFSET_SD;		// ?
	sc_signal<sc_uint<5> >	S_CMP_T_SD;		// ?
	sc_signal<bool>			S_EQ_T_SD;		// ?
	sc_signal<bool>			S_LT_Z_SD;		// ?
	sc_signal<bool>			S_LE_Z_SD;		// ?

   	sc_signal<sc_uint<32> > SEQADR_SD;		// seq @ ?


- execute.h
    sc_in<sc_uint<32> > 	NEXTSR_RX;		// next instruction sts ?
	// la valeur qui s'ecrit dans le meme registre a la fin du cycle

- mux_memory.h
	// little explication about bus data 
	sc_in<sc_uint<32> > DATA_RM;  // data bus input register
	sc_in<sc_uint<32> > DATA_SM;  // data bus input register (from bypass ?)
	sc_out<sc_uint<32> > MUX_DATA_SM;	// res data bus input register

- General 
	rom.h ?
	regfile.h ?
	ram.h ?
	misc.h ?
	ff_misc.h ?
	mips_dec.h ?