library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity core is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Mcache interface
        MCACHE_RESULT_SM : in std_logic_vector(31 downto 0);
        MCACHE_STALL_SM : in std_logic;

        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM : out std_logic;
        MCACHE_DATA_SM : out std_logic_vector(31 downto 0);
        MCACHE_ADR_SM : out std_logic_vector(31 downto 0);
        byt_sel : out std_logic_vector(3 downto 0);

        -- Icache interface
        IC_INST_SI : in std_logic_vector(31 downto 0);
        IC_STALL_SI : in std_logic; 

        ADR_SI : out std_logic_vector(31 downto 0);
        ADR_VALID_SI : out std_logic; 

        -- Debug 
        PC_INIT : in std_logic_vector(31 downto 0);
        DEBUG_PC_READ : out std_logic_vector(31 downto 0)
    );
end core;

architecture archi of core is 

-- dec2if  
signal DEC2IF_EMPTY_SD, DEC2IF_POP_SI : std_logic; 

-- if2dec  
signal IF2DEC_FLUSH_SD, IF2DEC_POP_SD, IF2DEC_EMPTY_SI : std_logic;
signal PC_RD, INSTR_RI, PC_IF2DEC_RI : std_logic_vector(31 downto 0);

-- dec2exe 
signal DEC2EXE_POP_SE, DEC2EXE_EMPTY_SD : std_logic;
signal OP1_RD, OP2_RD : std_logic_vector(31 downto 0);
signal RADR1_RD, RADR2_RD : std_logic_vector(5 downto 0);
signal MEM_DATA_RD : std_logic_vector(31 downto 0);
signal DEST_RD : std_logic_vector(5 downto 0);
signal CMD_RD : std_logic_vector(1 downto 0);
signal MEM_SIZE_RD : std_logic_vector(1 downto 0);
signal NEG_OP2_RD, WB_RD, SELECT_SHIFT_RD : std_logic;
signal MEM_LOAD_RD, MEM_STORE_RD, MEM_SIGN_EXTEND_RD : std_logic; 
signal SLT_RD, SLTU_RD : std_logic;

-- Decod Reg interface
signal RDATA1_SR, RDATA2_SR : std_logic_vector(31 downto 0);
signal RADR1_SD, RADR2_SD : std_logic_vector(5 downto 0);
signal WRITE_PC_SD : std_logic_vector(31 downto 0);
signal WRITE_PC_ENABLE_SD : std_logic;
signal READ_PC_SR : std_logic_vector(31 downto 0); 

-- exe2mem
signal EXE2MEM_POP_SM, EXE2MEM_EMPTY_SE : std_logic;
signal RES_RE : std_logic_vector(31 downto 0);
signal MEM_DATA_RE : std_logic_vector(31 downto 0);
signal DEST_RE : std_logic_vector(5 downto 0);
signal MEM_SIZE_RE : std_logic_vector(1 downto 0);
signal WB_RE, MEM_SIGN_EXTEND_RE, MEM_LOAD_RE, MEM_STORE_RE : std_logic;

-- mem2wbk 
signal MEM_RES_RM : std_logic_vector(31 downto 0);
signal MEM_DEST_RM : std_logic_vector(5 downto 0);
signal MEM_SIZE_RM : std_logic_vector(1 downto 0);
signal WB_RM, SIGN_EXTEND_RM, LOAD_RM : std_logic; 
signal MEM2WBK_EMPTY_SM, MEM2WBK_POP_SW : std_logic;

-- Reg interface
signal REG_DATA_SW : std_logic_vector(31 downto 0);
signal REG_DEST_SW : std_logic_vector(5 downto 0);
signal REG_WB_SW : std_logic;

-- Bypasses
signal BP_DEST_RE : std_logic_vector(5 downto 0);
signal BP_EXE_RES_RE : std_logic_vector(31 downto 0);
signal BP_MEM_LOAD_RE, BP_EXE2MEM_EMPTY_SE : std_logic;
signal BP_DEST_RM : std_logic_vector(5 downto 0);
signal BP_MEM_RES_RM : std_logic_vector(31 downto 0);
signal BP_R1_VALID_RD, BP_R2_VALID_RD : std_logic;
signal BP_RADR1_RD, BP_RADR2_RD : std_logic_vector(5 downto 0);

signal BLOCK_BP_RD : std_logic;

-- CSR
signal CSR_WADR_SM         : std_logic_vector(11 downto 0);
signal CSR_WDATA_SM        : std_logic_vector(31 downto 0);
signal CSR_ENABLE_SM, CSR_WENABLE_RD : std_logic;
signal EXCEPTION_SM        : std_logic := '0';
signal MSTATUS_WDATA_RM    : std_logic_vector(31 downto 0);
signal MIP_WDATA_RM        : std_logic_vector(31 downto 0);
signal MEPC_WDATA_RM       : std_logic_vector(31 downto 0);
signal MCAUSE_WDATA_SM     : std_logic_vector(31 downto 0);
signal MTVAL_WDATA_SM      : std_logic_vector(31 downto 0);
signal MEPC_SC             : std_logic_vector(31 downto 0);
signal MSTATUS_RC          : std_logic_vector(31 downto 0);
signal MTVEC_VALUE_RC      : std_logic_vector(31 downto 0);
signal MIP_VALUE_RC        : std_logic_vector(31 downto 0);
signal MIE_VALUE_RC        : std_logic_vector(31 downto 0);
signal MCAUSE_SC           : std_logic_vector(31 downto 0);
signal CSR_RADR_SD, CSR_WADR_RD : std_logic_vector(11 downto 0);
signal CSR_RDATA_SC, CSR_RDATA_RD : std_logic_vector(31 downto 0);
signal CSR_ENABLE_RD : std_logic;
signal MIP_WDATA_SM : std_logic_vector(31 downto 0);
-- Exception 
signal ILLEGAL_INSTRUCTION_RD, ADRESS_MISALIGNED_RD, INSTRUCTION_ACCESS_FAULT_RD, EBREAK_RD : std_logic;
signal ILLEGAL_INSTRUCTION_RE, ADRESS_MISALIGNED_RE, INSTRUCTION_ACCESS_FAULT_RE, EBREAK_RE : std_logic;
signal EXCEPTION_RI, EXCEPTION_RD, EXCEPTION_RE : std_logic;
signal INSTRUCTION_ADRESS_MISALIGNED_RE : std_logic;
signal STORE_ADRESS_MISALIGNED_RE, STORE_ACCESS_FAULT_RE : std_logic;
signal LOAD_ADRESS_MISALIGNED_RE, LOAD_ACCESS_FAULT_RE : std_logic;
signal ENV_CALL_U_MODE_RD, ENV_CALL_S_MODE_RD, ENV_CALL_M_MODE_RD : std_logic;
signal ENV_CALL_U_MODE_RE, ENV_CALL_S_MODE_RE, ENV_CALL_M_MODE_RE : std_logic;
signal ENV_CALL_WRONG_MODE_RE, ENV_CALL_WRONG_MODE_RD : std_logic;
signal MRET_RD, MRET_RE : std_logic;

signal CSR_WENABLE_RE, CSR_WENABLE_RM : std_logic;
signal CSR_RDATA_RE, CSR_RDATA_RM : std_logic_vector(31 downto 0);
signal CSR_WADR_RE : std_logic_vector(11 downto 0);

signal PC_BRANCH_VALUE_RE : std_logic_vector(31 downto 0);

signal BUS_ERROR_SX : std_logic;

signal CURRENT_MODE_SM : std_logic_vector(1 downto 0);
signal RETURN_ADRESS_SM : std_logic_vector(31 downto 0);
signal MRET_SM : std_logic;

signal PC_EXE2MEM_RE, PC_DEC2EXE_RD, PC_BRANCH_VALUE_RD, PC_MEM2WBK_RM : std_logic_vector(31 downto 0);

component ifetch 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Icache interface
        IC_INST_SI : in std_logic_vector(31 downto 0);
        IC_STALL_SI : in std_logic;
        ADR_SI : out std_logic_vector(31 downto 0);
        ADR_VALID_SI : out std_logic;

        -- dec2if interface 
        DEC2IF_EMPTY_SD : in std_logic;
        DEC2IF_POP_SI : out std_logic;

        -- if2dec interface 
        IF2DEC_FLUSH_SD : in std_logic;
        IF2DEC_POP_SD : in std_logic; 
        IF2DEC_EMPTY_SI : out std_logic; 

        PC_RD : in std_logic_vector(31 downto 0);
        INSTR_RI : out std_logic_vector(31 downto 0);
        PC_IF2DEC_RI : out std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_SM : in std_logic; 
        EXCEPTION_RI : out std_logic
    );
end component; 

component dec 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Reg interface
        RDATA1_SR, RDATA2_SR : in std_logic_vector(31 downto 0);
        REG_ADR1_SD, REG_ADR2_SD : out std_logic_vector(5 downto 0);
        WRITE_PC_SD : out std_logic_vector(31 downto 0);
        WRITE_PC_ENABLE_SD : out std_logic;
        READ_PC_SR : in std_logic_vector(31 downto 0);

        -- Exe interface 
        OP1_RD, OP2_RD : out std_logic_vector(31 downto 0);
        CMD_RD : out std_logic_vector(1 downto 0);
        NEG_OP2_RD : out std_logic; 
        WB_RD : out std_logic;
        DEST_RD : out std_logic_vector(5 downto 0);
        SELECT_SHIFT_RD : out std_logic;
        SLT_RD, SLTU_RD : out std_logic;    
        MEM_DATA_RD : out std_logic_vector(31 downto 0);
        MEM_LOAD_RD , MEM_STORE_RD, MEM_SIGN_EXTEND_RD : out std_logic;
        MEM_SIZE_RD : out std_logic_vector(1 downto 0);

        PC_DEC2EXE_RD : out std_logic_vector(31 downto 0);
        PC_BRANCH_VALUE_RD : out std_logic_vector(31 downto 0);

        CSR_WENABLE_RD  : out std_logic; 
        CSR_WADR_RD     : out std_logic_vector(11 downto 0);
        CSR_RDATA_RD    : out std_logic_vector(31 downto 0);

        -- dec2if interface
        DEC2IF_POP_SI : in std_logic; 
        DEC2IF_EMPTY_SD : out std_logic;
        PC_RD : out std_logic_vector(31 downto 0);

        -- if2dec interface
        INSTR_RI, PC_IF2DEC_RI : in std_logic_vector(31 downto 0);
        IF2DEC_EMPTY_SI : in std_logic;
        IF2DEC_POP_SD : out std_logic;
        IF2DEC_FLUSH_SD : out std_logic;

        -- dec2exe interface
        DEC2EXE_POP_SE : in std_logic;
        DEC2EXE_EMPTY_SD : out std_logic;

        -- Bypasses
        BP_DEST_RE : in std_logic_vector(5 downto 0);
        BP_EXE_RES_RE : in std_logic_vector(31 downto 0);
        BP_MEM_LOAD_RE : in std_logic;
        BP_EXE2MEM_EMPTY_SE, BP_MEM2WBK_EMPTY_SM : in std_logic;
        BP_DEST_RM : in std_logic_vector(5 downto 0);
        BP_MEM_RES_RM : in std_logic_vector(31 downto 0);
        BP_R1_VALID_RD, BP_R2_VALID_RD : out std_logic;
        BP_RADR1_RD, BP_RADR2_RD : out std_logic_vector(5 downto 0);
        BLOCK_BP_RD : out std_logic;

        CSR_WENABLE_RE, CSR_WENABLE_RM : in std_logic;
        CSR_RDATA_RE, CSR_RDATA_RM : in std_logic_vector(31 downto 0);

        CSR_RADR_SD : out std_logic_vector(11 downto 0);
        CSR_RDATA_SC : in std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_RI : in std_logic;
        ILLEGAL_INSTRUCTION_RD : out std_logic;
        ADRESS_MISALIGNED_RD : out std_logic; 
        ENV_CALL_U_MODE_RD, ENV_CALL_M_MODE_RD, ENV_CALL_S_MODE_RD : out std_logic; 
        ENV_CALL_WRONG_MODE_RD : out std_logic;
        INSTRUCTION_ACCESS_FAULT_RD : out std_logic;
        MRET_RD : out std_logic;
        EXCEPTION_RD : out std_logic;
        EBREAK_RD : out std_logic; 
        
        CURRENT_MODE_SM : in std_logic_vector(1 downto 0);
        EXCEPTION_SM    : in std_logic;
        MTVEC_VALUE_RC  : in std_logic_vector(31 downto 0);
        MCAUSE_WDATA_SM : in std_logic_vector(31 downto 0);
        MRET_SM         : in std_logic;
        RETURN_ADRESS_SM : in std_logic_vector(31 downto 0)
        
    );
end component; 

component exec 
    port(
        -- global interface 
        clk, reset_n : in std_logic;

        OP1_RD, OP2_RD : in std_logic_vector(31 downto 0);
        RADR1_RD, RADR2_RD : in std_logic_vector(5 downto 0);
        MEM_DATA_RD : in std_logic_vector(31 downto 0);
        DEST_RD : in std_logic_vector(5 downto 0);
        CMD_RD : in std_logic_vector(1 downto 0);
        MEM_SIZE_RD : in std_logic_vector(1 downto 0);
        NEG_OP2_RD : in std_logic;
        WB_RD : in std_logic;
        MEM_SIGN_EXTEND_RD : in std_logic;
        SELECT_SHIFT_RD : in std_logic;
        MEM_LOAD_RD, MEM_STORE_RD : in std_logic;
        EXE2MEM_POP_SM : in std_logic;
        DEC2EXE_EMPTY_SD : in std_logic;
        SLT_RD, SLTU_RD : in std_logic;

        RES_RE : out std_logic_vector(31 downto 0);    
        MEM_DATA_RE : out std_logic_vector(31 downto 0);
        DEST_RE : out std_logic_vector(5 downto 0);
        MEM_SIZE_RE : out std_logic_vector(1 downto 0);
        WB_RE : out std_logic;
        MEM_SIGN_EXTEND_RE : out std_logic;
        MEM_LOAD_RE, MEM_STORE_RE : out std_logic;
        EXE2MEM_EMPTY_SE : out std_logic;
        DEC2EXE_POP_SE : out std_logic;

        PC_EXE2MEM_RE : out std_logic_vector(31 downto 0);

        -- bypasses 
        BLOCK_BP_RD : in std_logic;
        MEM_DEST_RM : in std_logic_vector(5 downto 0); 
        MEM_RES_RM : in std_logic_vector(31 downto 0);
        CSR_WENABLE_RM : in std_logic;
        CSR_RDATA_RM : in std_logic_vector(31 downto 0);

        -- CSR 
        CSR_ENABLE_RD : in std_logic;
        CSR_WADR_RD   : in std_logic_vector(11 downto 0);
        CSR_RDATA_RD  : in std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_SM    : in std_logic;
        EXCEPTION_RD    : in std_logic;
        CURRENT_MODE_SM : in std_logic_vector(1 downto 0);
        PC_BRANCH_VALUE_RD : in std_logic_vector(31 downto 0);
        PC_BRANCH_VALUE_RE : out std_logic_vector(31 downto 0);

        ILLEGAL_INSTRUCTION_RD, ADRESS_MISALIGNED_RD, INSTRUCTION_ACCESS_FAULT_RD : in std_logic; 
        ENV_CALL_U_MODE_RD, ENV_CALL_S_MODE_RD, ENV_CALL_M_MODE_RD : in std_logic;
        ENV_CALL_WRONG_MODE_RD : in std_logic;
        MRET_RD : in std_logic;
        EBREAK_RD : in std_logic;

        EXCEPTION_RE    : out std_logic;
        ILLEGAL_INSTRUCTION_RE, ADRESS_MISALIGNED_RE, INSTRUCTION_ACCESS_FAULT_RE : out std_logic; 
        ENV_CALL_U_MODE_RE, ENV_CALL_S_MODE_RE, ENV_CALL_M_MODE_RE : out std_logic;
        ENV_CALL_WRONG_MODE_RE : out std_logic;
        LOAD_ADRESS_MISALIGNED_RE, LOAD_ACCESS_FAULT_RE : out std_logic;
        STORE_ADRESS_MISALIGNED_RE, STORE_ACCESS_FAULT_RE : out std_logic;
        MRET_RE : out std_logic;
        EBREAK_RE : out std_logic;

        CSR_WENABLE_RE  : out std_logic;
        CSR_WADR_RE     : out std_logic_vector(11 downto 0);
        CSR_RDATA_RE    : out std_logic_vector(31 downto 0)
    
    );
end component; 

component mem 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Mcache interface
        MCACHE_RESULT_SM : in std_logic_vector(31 downto 0);
        MCACHE_STALL_SM : in std_logic;
        MCACHE_ADR_SM, MCACHE_DATA_SM : out std_logic_vector(31 downto 0);
        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM : out std_logic;
        byt_sel : out std_logic_vector(3 downto 0);

        -- Exe interface
        RES_RE, MEM_DATA_RE : in std_logic_vector(31 downto 0);
        DEST_RE : in std_logic_vector(5 downto 0);
        MEM_SIZE_RE : in std_logic_vector(1 downto 0);
        WB_RE, SIGN_EXTEND_RE, LOAD_RE, STORE_RE : in std_logic;
        
        PC_EXE2MEM_RE : std_logic_vector(31 downto 0);

        -- exe2mem interface
        EXE2MEM_EMPTY_SE : in std_logic;
        EXE2MEM_POP_SM : out std_logic;

        -- mem2wbk interface
        MEM2WBK_POP_SW : in std_logic;
        MEM2WBK_EMPTY_SM : out std_logic;
        
        -- Wbk interface
        MEM_RES_RM : out std_logic_vector(31 downto 0);
        MEM_DEST_RM : out std_logic_vector(5 downto 0);
        MEM_SIZE_RM : out std_logic_vector(1 downto 0);
        WB_RM, SIGN_EXTEND_RM, LOAD_RM : out std_logic;

        -- CSR 
        CSR_WENABLE_RE  : in std_logic;
        CSR_WADR_RE     : in std_logic_vector(11 downto 0);
        CSR_RDATA_RE    : in std_logic_vector(31 downto 0);

        CSR_WADR_SM     : out std_logic_vector(11 downto 0);
        CSR_WDATA_SM    : out std_logic_vector(31 downto 0);
        CSR_ENABLE_SM   : out std_logic; 

        MSTATUS_WDATA_RM    : out std_logic_vector(31 downto 0);
        MIP_WDATA_RM        : out std_logic_vector(31 downto 0);
        MEPC_WDATA_RM       : out std_logic_vector(31 downto 0);
        MCAUSE_WDATA_SM     : out std_logic_vector(31 downto 0);
        MTVAL_WDATA_SM      : out std_logic_vector(31 downto 0);

        MEPC_SC             : in std_logic_vector(31 downto 0);
        MSTATUS_RC          : in std_logic_vector(31 downto 0);
        MTVEC_VALUE_RC      : in std_logic_vector(31 downto 0);
        MIP_VALUE_RC        : in std_logic_vector(31 downto 0);
        MIE_VALUE_RC        : in std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_RE    : in std_logic;
        LOAD_ADRESS_MISALIGNED_RE, LOAD_ACCESS_FAULT_RE, ILLEGAL_INSTRUCTION_RE : in std_logic;
        INSTRUCTION_ADRESS_MISALIGNED_RE, INSTRUCTION_ACCESS_FAULT_RE : in std_logic;
        STORE_ADRESS_MISALIGNED_RE, STORE_ACCESS_FAULT_RE : in std_logic;
        ENV_CALL_U_MODE_RE, ENV_CALL_S_MODE_RE, ENV_CALL_M_MODE_RE : in std_logic;
        ENV_CALL_WRONG_MODE_RE : in std_logic;
        MRET_RE : in std_logic;

        PC_BRANCH_VALUE_RE : in std_logic_vector(31 downto 0);

        BUS_ERROR_SX : in std_logic;

        EXCEPTION_SM : out std_logic;
        CURRENT_MODE_SM : out std_logic_vector(1 downto 0);
        RETURN_ADRESS_SM : out std_logic_vector(31 downto 0);
        MRET_SM : out std_logic
    ); 
end component; 

component wbk 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Mem 
        MEM_RES_RM : in std_logic_vector(31 downto 0);
        MEM_DEST_RM : in std_logic_vector(5 downto 0);
        MEM_SIZE_RM : in std_logic_vector(1 downto 0);
        WB_RM : in std_logic;
        SIGN_EXTEND_RM : in std_logic;
        LOAD_RM : in std_logic;

        PC_MEM2WBK_RM   : in std_logic_vector(31 downto 0);
        CSR_RDATA_RM    : in std_logic_vector(31 downto 0);
        CSR_WENABLE_RM  : in std_logic;
        CURRENT_MODE_SM : in std_logic_vector(1 downto 0);

        -- mem2wbk interface
        MEM2WBK_EMPTY_SM : in std_logic;
        MEM2WBK_POP_SW : out std_logic;

        -- Reg interface
        DATA_SW : out std_logic_vector(31 downto 0);
        DEST_SW : out std_logic_vector(5 downto 0);
        WB_SW : out std_logic 

    );
end component; 

component reg 
    port(
        -- global interface
        clk, reset_n : in std_logic;
        PC_INIT : in std_logic_vector(31 downto 0);

        -- read ports
        RDATA1_SR, RDATA2_SR : out std_logic_vector(31 downto 0);
        RADR1_SD, RADR2_SD : in std_logic_vector(5 downto 0);
        
        -- write ports
        WDATA_SW : in std_logic_vector(31 downto 0);
        WADR_SW : in std_logic_vector(5 downto 0);
        WENABLE_SW : in std_logic; 

        WRITE_PC_SD : in std_logic_vector(31 downto 0);
        WRITE_PC_ENABLE_SD : in std_logic;

        -- PC
        READ_PC_SR : out std_logic_vector(31 downto 0)
    );
end component; 

component csr
    port(
        -- global interface
        clk, reset_n : in std_logic;


        CSR_WADR_SM         : in std_logic_vector(11 downto 0);
        CSR_WDATA_SM        : in std_logic_vector(31 downto 0);
        CSR_ENABLE_SM       : in std_logic;

        EXCEPTION_SM        : in std_logic;
        MSTATUS_WDATA_RM    : in std_logic_vector(31 downto 0);
        MIP_WDATA_RM        : in std_logic_vector(31 downto 0);
        MEPC_WDATA_RM       : in std_logic_vector(31 downto 0);
        MCAUSE_WDATA_SM     : in std_logic_vector(31 downto 0);
        MTVAL_WDATA_SM      : in std_logic_vector(31 downto 0);

        MEPC_SC             : out std_logic_vector(31 downto 0);
        MSTATUS_RC          : out std_logic_vector(31 downto 0);
        MTVEC_VALUE_RC      : out std_logic_vector(31 downto 0);
        MIP_VALUE_RC        : out std_logic_vector(31 downto 0);
        MIE_VALUE_RC        : out std_logic_vector(31 downto 0);
        MCAUSE_SC           : out std_logic_vector(31 downto 0);
    
        CSR_RADR_SD         : in std_logic_vector(11 downto 0);
        CSR_RDATA_SC        : out std_logic_vector(31 downto 0)
    );
end component; 

begin 

-------------------------
-- Instanciation 
-------------------------
ifetch_i : ifetch 
    port map(
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        -- Icache interface
        IC_INST_SI          => IC_INST_SI,
        IC_STALL_SI         => IC_STALL_SI, 
        ADR_SI              => ADR_SI,
        ADR_VALID_SI        => ADR_VALID_SI, 

        -- dec2if interface 
        DEC2IF_EMPTY_SD     => DEC2IF_EMPTY_SD, 
        DEC2IF_POP_SI       => DEC2IF_POP_SI, 

        -- if2dec interface 
        IF2DEC_FLUSH_SD     => IF2DEC_FLUSH_SD,
        IF2DEC_POP_SD       => IF2DEC_POP_SD, 
        IF2DEC_EMPTY_SI     => IF2DEC_EMPTY_SI, 

        PC_RD               => PC_RD, 
        INSTR_RI            => INSTR_RI,
        PC_IF2DEC_RI        => PC_IF2DEC_RI,


        -- Exception 
        EXCEPTION_SM        => EXCEPTION_SM,
        EXCEPTION_RI        => EXCEPTION_RI       
    );

dec_i : dec
    port map(
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        -- Reg interface
        RDATA1_SR           => RDATA1_SR,
        RDATA2_SR           => RDATA2_SR, 
        REG_ADR1_SD            => RADR1_SD, 
        REG_ADR2_SD            => RADR2_SD,
        WRITE_PC_SD         => WRITE_PC_SD,
        WRITE_PC_ENABLE_SD  => WRITE_PC_ENABLE_SD, 
        READ_PC_SR          => READ_PC_SR,           

        -- Exe interface 
        OP1_RD              => OP1_RD, 
        OP2_RD              => OP2_RD,
        CMD_RD              => CMD_RD,
        NEG_OP2_RD          => NEG_OP2_RD, 
        WB_RD               => WB_RD,
        DEST_RD             => DEST_RD,
        SELECT_SHIFT_RD     => SELECT_SHIFT_RD ,
        SLT_RD              => SLT_RD, 
        SLTU_RD             => SLTU_RD,    
        MEM_DATA_RD         => MEM_DATA_RD,
        MEM_LOAD_RD         => MEM_LOAD_RD, 
        MEM_STORE_RD        => MEM_STORE_RD, 
        MEM_SIGN_EXTEND_RD  => MEM_SIGN_EXTEND_RD,
        MEM_SIZE_RD         => MEM_SIZE_RD,


        CSR_WENABLE_RD      => CSR_WENABLE_RD, 
        CSR_WADR_RD         => CSR_WADR_RD,
        CSR_RDATA_RD        => CSR_RDATA_RD,

        PC_DEC2EXE_RD       => PC_DEC2EXE_RD,
        PC_BRANCH_VALUE_RD  => PC_BRANCH_VALUE_RD,


        -- dec2if interface
        DEC2IF_POP_SI       => DEC2IF_POP_SI, 
        DEC2IF_EMPTY_SD     => DEC2IF_EMPTY_SD,
        PC_RD               => PC_RD,

        -- if2dec interface
        INSTR_RI            => INSTR_RI, 
        PC_IF2DEC_RI        => PC_IF2DEC_RI,
        IF2DEC_EMPTY_SI     => IF2DEC_EMPTY_SI,
        IF2DEC_POP_SD       => IF2DEC_POP_SD,
        IF2DEC_FLUSH_SD     => IF2DEC_FLUSH_SD,

        -- dec2exe interface
        DEC2EXE_POP_SE      => DEC2EXE_POP_SE,
        DEC2EXE_EMPTY_SD    => DEC2EXE_EMPTY_SD,

        -- Bypasses
        BP_DEST_RE             => DEST_RE,
        BP_EXE_RES_RE          => RES_RE,
        BP_MEM_LOAD_RE         => MEM_LOAD_RE,
        BP_EXE2MEM_EMPTY_SE    => EXE2MEM_EMPTY_SE, 
        BP_MEM2WBK_EMPTY_SM    => MEM2WBK_EMPTY_SM,
        BP_DEST_RM         => MEM_DEST_RM,
        BP_MEM_RES_RM          => MEM_RES_RM,
        BP_R1_VALID_RD      => BP_R1_VALID_RD, 
        BP_R2_VALID_RD      => BP_R2_VALID_RD,
        BP_RADR1_RD         => BP_RADR1_RD,
        BP_RADR2_RD         => BP_RADR2_RD,
        BLOCK_BP_RD         => BLOCK_BP_RD,

        CSR_WENABLE_RE      => CSR_WENABLE_RE,
        CSR_WENABLE_RM      => CSR_WENABLE_RM,
        CSR_RDATA_RE        => CSR_RDATA_RE,
        CSR_RDATA_RM        => CSR_RDATA_RM,  

        CSR_RADR_SD         => CSR_RADR_SD,
        CSR_RDATA_SC        => CSR_RDATA_SC, 

        -- Exception 
        EXCEPTION_RI                => EXCEPTION_RI, 
        ILLEGAL_INSTRUCTION_RD      => ILLEGAL_INSTRUCTION_RD,
        ADRESS_MISALIGNED_RD        => ADRESS_MISALIGNED_RD,
        ENV_CALL_U_MODE_RD          => ENV_CALL_U_MODE_RD,
        ENV_CALL_M_MODE_RD          => ENV_CALL_M_MODE_RD,
        ENV_CALL_S_MODE_RD          => ENV_CALL_S_MODE_RD, 
        ENV_CALL_WRONG_MODE_RD      => ENV_CALL_WRONG_MODE_RD, 
        INSTRUCTION_ACCESS_FAULT_RD => INSTRUCTION_ACCESS_FAULT_RD,
        MRET_RD                     => MRET_RD,
        EXCEPTION_RD                => EXCEPTION_RD, 
        EBREAK_RD                   => EBREAK_RD, 
        
        CURRENT_MODE_SM             => CURRENT_MODE_SM, 
        EXCEPTION_SM                => EXCEPTION_SM, 
        MTVEC_VALUE_RC              => MTVEC_VALUE_RC,
        MCAUSE_WDATA_SM             => MCAUSE_WDATA_SM, 
        MRET_SM                     => MRET_SM, 
        RETURN_ADRESS_SM            => RETURN_ADRESS_SM
    );

exec_i : exec 
    port map(
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        OP1_RD              => OP1_RD, 
        OP2_RD              => OP2_RD,
        RADR1_RD            => BP_RADR1_RD, 
        RADR2_RD            => BP_RADR2_RD,
        MEM_DATA_RD         => MEM_DATA_RD,
        DEST_RD             => DEST_RD,
        CMD_RD              => CMD_RD,
        MEM_SIZE_RD         => MEM_SIZE_RD,
        NEG_OP2_RD          => NEG_OP2_RD,
        WB_RD               => WB_RD,
        MEM_SIGN_EXTEND_RD  => MEM_SIGN_EXTEND_RD,
        SELECT_SHIFT_RD     => SELECT_SHIFT_RD,
        MEM_LOAD_RD         => MEM_LOAD_RD, 
        MEM_STORE_RD        => MEM_STORE_RD,
        EXE2MEM_POP_SM      => EXE2MEM_POP_SM,
        DEC2EXE_EMPTY_SD    => DEC2EXE_EMPTY_SD,
        SLT_RD              => SLT_RD, 
        SLTU_RD             => SLTU_RD,

        RES_RE              => RES_RE,    
        MEM_DATA_RE         => MEM_DATA_RE,
        DEST_RE             => DEST_RE,
        MEM_SIZE_RE         => MEM_SIZE_RE,
        WB_RE               => WB_RE,
        MEM_SIGN_EXTEND_RE  => MEM_SIGN_EXTEND_RE,
        MEM_LOAD_RE         => MEM_LOAD_RE, 
        MEM_STORE_RE        => MEM_STORE_RE,
        EXE2MEM_EMPTY_SE    => EXE2MEM_EMPTY_SE,
        DEC2EXE_POP_SE      => DEC2EXE_POP_SE,

        PC_EXE2MEM_RE       => PC_EXE2MEM_RE,

        BLOCK_BP_RD         => BLOCK_BP_RD,
        MEM_DEST_RM         => MEM_DEST_RM, 
        MEM_RES_RM          => MEM_RES_RM,

        CSR_WENABLE_RM      => CSR_WENABLE_RM,
        CSR_RDATA_RM        => CSR_RDATA_RM,

        -- CSR 
        CSR_ENABLE_RD       => CSR_ENABLE_RD,
        CSR_WADR_RD         => CSR_WADR_RD, 
        CSR_RDATA_RD        => CSR_RDATA_RD, 

        -- Exception 
        EXCEPTION_SM        => EXCEPTION_SM, 
        EXCEPTION_RD        => EXCEPTION_RD, 
        CURRENT_MODE_SM     => CURRENT_MODE_SM,
        PC_BRANCH_VALUE_RD  => PC_BRANCH_VALUE_RD, 
        PC_BRANCH_VALUE_RE  => PC_BRANCH_VALUE_RE, 

        ILLEGAL_INSTRUCTION_RD      => ILLEGAL_INSTRUCTION_RD,
        ADRESS_MISALIGNED_RD        => ADRESS_MISALIGNED_RD, 
        INSTRUCTION_ACCESS_FAULT_RD => INSTRUCTION_ACCESS_FAULT_RD, 
        ENV_CALL_U_MODE_RD          => ENV_CALL_U_MODE_RD, 
        ENV_CALL_S_MODE_RD          => ENV_CALL_S_MODE_RD, 
        ENV_CALL_M_MODE_RD          => ENV_CALL_M_MODE_RD,
        ENV_CALL_WRONG_MODE_RD      => ENV_CALL_WRONG_MODE_RD, 
        MRET_RD                     => MRET_RD,
        EBREAK_RD                   => EBREAK_RD,

        EXCEPTION_RE                => EXCEPTION_RE,
        ILLEGAL_INSTRUCTION_RE      => ILLEGAL_INSTRUCTION_RE, 
        ADRESS_MISALIGNED_RE        => ADRESS_MISALIGNED_RE, 
        INSTRUCTION_ACCESS_FAULT_RE => INSTRUCTION_ACCESS_FAULT_RE,  
        ENV_CALL_U_MODE_RE          => ENV_CALL_U_MODE_RE, 
        ENV_CALL_S_MODE_RE          => ENV_CALL_S_MODE_RE, 
        ENV_CALL_M_MODE_RE          => ENV_CALL_M_MODE_RE,
        ENV_CALL_WRONG_MODE_RE      => ENV_CALL_WRONG_MODE_RE,
        LOAD_ADRESS_MISALIGNED_RE   => LOAD_ADRESS_MISALIGNED_RE, 
        LOAD_ACCESS_FAULT_RE        => LOAD_ACCESS_FAULT_RE,
        STORE_ADRESS_MISALIGNED_RE  => STORE_ADRESS_MISALIGNED_RE,
        STORE_ACCESS_FAULT_RE       => STORE_ACCESS_FAULT_RE,
        MRET_RE                     => MRET_RE,
        EBREAK_RE                   => EBREAK_RE, 

        CSR_WENABLE_RE              => CSR_WENABLE_RE,
        CSR_WADR_RE                 => CSR_WADR_RE,     
        CSR_RDATA_RE                => CSR_RDATA_RE
    );

mem_i : mem 
    port map(
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        -- Mcache interface
        MCACHE_RESULT_SM    => MCACHE_RESULT_SM,
        MCACHE_STALL_SM     => MCACHE_STALL_SM,
        MCACHE_ADR_SM       => MCACHE_ADR_SM, 
        MCACHE_DATA_SM      => MCACHE_DATA_SM,
        MCACHE_ADR_VALID_SM => MCACHE_ADR_VALID_SM, 
        MCACHE_STORE_SM     => MCACHE_STORE_SM, 
        MCACHE_LOAD_SM      => MCACHE_LOAD_SM,
        byt_sel             => byt_sel, 

        -- Exe interface
        RES_RE              => RES_RE,
        MEM_DATA_RE         => MEM_DATA_RE,
        DEST_RE             => DEST_RE,
        MEM_SIZE_RE         => MEM_SIZE_RE,      
        WB_RE               => WB_RE, 
        SIGN_EXTEND_RE      => MEM_SIGN_EXTEND_RE, 
        LOAD_RE             => MEM_LOAD_RE,
        STORE_RE        => MEM_STORE_RE,

        PC_EXE2MEM_RE       => PC_EXE2MEM_RE,

        -- exe2mem interface
        EXE2MEM_EMPTY_SE    => EXE2MEM_EMPTY_SE,
        EXE2MEM_POP_SM      => EXE2MEM_POP_SM,

        -- mem2wbk interface
        MEM2WBK_POP_SW      => MEM2WBK_POP_SW, 
        MEM2WBK_EMPTY_SM    => MEM2WBK_EMPTY_SM,
        
        -- Wbk interface
        MEM_RES_RM          => MEM_RES_RM,
        MEM_DEST_RM         => MEM_DEST_RM,
        MEM_SIZE_RM         => MEM_SIZE_RM,
        WB_RM               => WB_RM,
        SIGN_EXTEND_RM      => SIGN_EXTEND_RM, 
        LOAD_RM             => LOAD_RM,

        -- CSR 
        CSR_WENABLE_RE      => CSR_WENABLE_RE,
        CSR_WADR_RE         => CSR_WADR_RE,     
        CSR_RDATA_RE        => CSR_RDATA_RE,    

        CSR_WADR_SM         => CSR_WADR_SM,
        CSR_WDATA_SM        => CSR_WDATA_SM,   
        CSR_ENABLE_SM       => CSR_ENABLE_SM, 

        MSTATUS_WDATA_RM    => MSTATUS_WDATA_RM,    
        MIP_WDATA_RM        => MIP_WDATA_RM,    
        MEPC_WDATA_RM       => MEPC_WDATA_RM,    
        MCAUSE_WDATA_SM     => MCAUSE_WDATA_SM,    
        MTVAL_WDATA_SM      => MTVAL_WDATA_SM,    

        MEPC_SC             => MEPC_SC,      
        MSTATUS_RC          => MSTATUS_RC,      
        MTVEC_VALUE_RC      => MTVEC_VALUE_RC,      
        MIP_VALUE_RC        => MIP_VALUE_RC,      
        MIE_VALUE_RC        => MIE_VALUE_RC,      

        -- Exception 
        EXCEPTION_RE                        => EXCEPTION_RE,    
        LOAD_ADRESS_MISALIGNED_RE           => LOAD_ADRESS_MISALIGNED_RE, 
        LOAD_ACCESS_FAULT_RE                => LOAD_ACCESS_FAULT_RE, 
        ILLEGAL_INSTRUCTION_RE              => ILLEGAL_INSTRUCTION_RE, 
        INSTRUCTION_ADRESS_MISALIGNED_RE    => INSTRUCTION_ADRESS_MISALIGNED_RE, 
        INSTRUCTION_ACCESS_FAULT_RE         => INSTRUCTION_ACCESS_FAULT_RE,
        STORE_ADRESS_MISALIGNED_RE          => STORE_ADRESS_MISALIGNED_RE, 
        STORE_ACCESS_FAULT_RE               => STORE_ACCESS_FAULT_RE, 
        ENV_CALL_U_MODE_RE                  => ENV_CALL_U_MODE_RE, 
        ENV_CALL_S_MODE_RE                  => ENV_CALL_S_MODE_RE,
        ENV_CALL_M_MODE_RE                  => ENV_CALL_M_MODE_RE,
        ENV_CALL_WRONG_MODE_RE              => ENV_CALL_WRONG_MODE_RE,
        MRET_RE                             => MRET_RE,

        PC_BRANCH_VALUE_RE                  => PC_BRANCH_VALUE_RE,

        BUS_ERROR_SX                        => BUS_ERROR_SX, 

        EXCEPTION_SM                        => EXCEPTION_SM,
        CURRENT_MODE_SM                     => CURRENT_MODE_SM,
        RETURN_ADRESS_SM                    => RETURN_ADRESS_SM,
        MRET_SM                             => MRET_SM
    );

wbk_i : wbk 
    port map( 
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        -- Mem 
        MEM_RES_RM          => MEM_RES_RM,
        MEM_DEST_RM         => MEM_DEST_RM,
        MEM_SIZE_RM         => MEM_SIZE_RM,
        WB_RM               => WB_RM,
        SIGN_EXTEND_RM      => SIGN_EXTEND_RM,
        LOAD_RM             => LOAD_RM, 

        PC_MEM2WBK_RM       => PC_MEM2WBK_RM,
        CSR_RDATA_RM        => CSR_RDATA_RM,
        CSR_WENABLE_RM      => CSR_WENABLE_RM,
        CURRENT_MODE_SM     => CURRENT_MODE_SM, 

        -- mem2wbk interface
        MEM2WBK_EMPTY_SM    => MEM2WBK_EMPTY_SM,
        MEM2WBK_POP_SW      => MEM2WBK_POP_SW,

        -- Reg interface
        DATA_SW         => REG_DATA_SW,
        DEST_SW         => REG_DEST_SW,
        WB_SW           => REG_WB_SW 
    );
 
reg_i : reg 
    port map(
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        PC_INIT             => PC_INIT,
        
        -- read
        RDATA1_SR           => RDATA1_SR, 
        RDATA2_SR           => RDATA2_SR, 
        RADR1_SD         => RADR1_SD, 
        RADR2_SD         => RADR2_SD, 
        
        -- write 
        WDATA_SW            => REG_DATA_SW, 
        WADR_SW             => REG_DEST_SW, 
        WENABLE_SW          => REG_WB_SW,
        
        WRITE_PC_SD         => WRITE_PC_SD, 
        WRITE_PC_ENABLE_SD  => WRITE_PC_ENABLE_SD,
        READ_PC_SR          => READ_PC_SR
    );

csr_i : csr 
    port map(
        -- global interface
        clk                 => clk,
        reset_n             => reset_n, 

        CSR_WADR_SM         => CSR_WADR_SM, 
        CSR_WDATA_SM        => CSR_WDATA_SM, 
        CSR_ENABLE_SM       => CSR_ENABLE_SM, 

        EXCEPTION_SM        => EXCEPTION_SM,
        MSTATUS_WDATA_RM    => MSTATUS_WDATA_RM, 
        MIP_WDATA_RM        => MIP_WDATA_RM, 
        MEPC_WDATA_RM       => MEPC_WDATA_RM, 
        MCAUSE_WDATA_SM     => MCAUSE_WDATA_SM, 
        MTVAL_WDATA_SM      => MTVAL_WDATA_SM, 

        MEPC_SC             => MEPC_SC,    
        MSTATUS_RC          => MSTATUS_RC,    
        MTVEC_VALUE_RC      => MTVEC_VALUE_RC,    
        MIP_VALUE_RC        => MIP_VALUE_RC,    
        MIE_VALUE_RC        => MIE_VALUE_RC,    
        MCAUSE_SC           => MCAUSE_SC,    
    
        CSR_RADR_SD         => CSR_RADR_SD,
        CSR_RDATA_SC        => CSR_RDATA_SC
    );

DEBUG_PC_READ <= READ_PC_SR; 

end archi;