1. ADVISE
	All the information present on this report are a combination of 	suppositions and information given by Mr. Pirouz Bazargan Sabet and 	translated to this document but it can be bad understood then the 	information can be wrong or true, i recommend take the information with precaution and in case of doubt please go to the information source so 	directly to Mr. Pirouz Bazargan Sabet.

2. MIPS STRUCTURE
	This architecture is based on the mips1 and/or mips32 instruction set.
	The architecture is a 5 stage pipelined, each stage are compound by 3 	elements:
		1 -	The stage core witch is named like the stage, ex. decode.h
		2 - A multiplexer which is gonna handle the 	information flow in the pipeline, named
			like the stage with the "mux" prefix, ex. mux_decode.h.
		3 - A fifo (first in first out) which gonna literally just put the 
			input on the output, named like the stage with the "ff" prefix, ex.	ff_decode.h
		4 - Exception : the alu is defined on the core but directly 
			connected
			to the stage execute.
	2.1 PIPELINE
		The pipeline is composed with the manager (multipleux) and the fifo.
		2.1.1 PIPELINE REGITERS
			A pipeline have 2 types of register:
				1. command register (ex. opcode)
				2. data register    (ex. toper or soper)
			Each register have different behavior:
				1. The command register can do 2 actions:
					- Either keep the data (keep)
					- Either charge the new data (!keep) 
				2. The data register can do 3 actions:
					- Either conserve the command (hold)
					- Either delete the command (bubble)
						ex. Eret - so insert nop*
					- Either add a new command (!hold and !bubble)
				
3. AGREEMENT FOR INPUT, OUTPUT OR INNER SIGNALS DECLARATION
	The agreement for declare a wire is compound by a name or 
	an acronym, and a suffix connected by an underscore.
	The suffix is compound by 2 capital letters, 
	the wire type and the stage.
	
	TYPES:
		S: signal	(wire which is not connected to a register 	  from a fifo)
		R: register (wire which is connected to a registe 
		   from a fifo)
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

4. STAGE AND COMPONENTS DESCRIPTION
	In addition of all components files, 
	"constants.h" contains all instructions encoding and special signals encoding.

	4.1 Ifetch
		4.1.1 Ifetch component
			ifetch only manage 2 thinks, a bool witch tell if then incoming instruction is a delayed slot and a signal of 32 bits witch define the status register for the incoming instruction
		4.1.2 Ifetch mux
			The mux is gonna manage the instruction register, the new instruction fetched, a bool witch tell if is or not a delayed slot, the pc register and the status register with the 3 base mux signal(BUBBLE, HOLD and KEEP).
		4.1.3 Ifetch fifo
			The input of the fifo is the output of the mux.
	4.2 Decod
		4.2.1 Decod component
			Decod can handle 3 instruction format J, I and R.
			Decod use the coprocessor 0 and handle exceptions.
			HIGH and LOW register are used by the exception handler(for example for instruction emulation see index 5).
			Multiple interesting signals:
				- I_DUSE_SD: uses operands signal
				- I_READS_SD: instruction uses S operands (soper)
				- I_READT_SD: instruction uses T operands (toper)
				- I_OSGND_SD: signed operation
				- I_WRT31_SD: write into r31
				- I_WRITE_SD: write into reg
			
		4.2.2 Decod mux
		4.2.3 Decod fifo
	4.3 Exec
		4.3.1 Exec component
		4.3.2 ALU
			The ALU are made for make logique operations (AND, OR, 	NOR and XOR), arithmetique operations (addition and substraction), 	shift operations 	and slt compare operation.
		4.3.2 Exec mux
		4.3.3 Exec fifo
	4.4 Memory
		4.4.1 Memory component
		4.4.2 Memory mux
		4.4.3 Memory fifo
	4.5 Write back
		4.5.1 Write back component
		4.5.2 Write back mux
		4.5.3 Write back fifo

5 EMULATION D'INSTRUCTION

on ajout l'opcode mult qui n'est pas une instruction, quand leprocesseur 	va la lire va generait une exception(unknown instruction)donc on va 	verifier les handler d'exception qui pour notre cas vatrouver qu'il existe 	un handler mult (multiplication) qui vabrancher vers une "fonction" qui va
faire la multiplication que dans ce cas va ecrire sont reulta sur les registre low et high et apres il va returner a l'instruction mult ducode 	de base.

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

// CLZ: count leading zero
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

	2.  -
	
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