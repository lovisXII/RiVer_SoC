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
signal ADR_DEST_SR : std_logic_vector(5 downto 0);
signal RADR1_SD, RADR2_SD : std_logic_vector(5 downto 0);
signal WRITE_PC_SD : std_logic_vector(31 downto 0);
signal WRITE_PC_ENABLE_SD : std_logic;
signal READ_PC_SR : std_logic_vector(31 downto 0); 

-- exe2mem
signal EXE2MEM_POP_SM, EXE2MEM_EMPTY_SE : std_logic;
signal RES_RE : std_logic_vector(31 downto 0);
signal DATA_RE : std_logic_vector(31 downto 0);
signal DEST_RE : std_logic_vector(5 downto 0);
signal MEM_SIZE_RE : std_logic_vector(1 downto 0);
signal WB_RE, MEM_SIGN_EXTEND_RE, MEM_LOAD_RE, MEM_STORE_RE : std_logic;

-- mem2wbk 
signal DATA_RM : std_logic_vector(31 downto 0);
signal DEST_RM : std_logic_vector(5 downto 0);
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
signal BP_MEM_LOAD_RE, BP_EXE2MEM_EMPTY : std_logic;
signal BP_DEST_RM : std_logic_vector(5 downto 0);
signal BP_MEM_RES_RM : std_logic_vector(31 downto 0);
signal BP_R1_VALID_RD, BP_R2_VALID_RD : std_logic;
signal BP_RADR1_RD, BP_RADR2_RD : std_logic_vector(5 downto 0);

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
        DEC2IF_EMPTY_SI : in std_logic;
        DEC2IF_POP_SI : out std_logic;

        -- if2dec interface 
        IF2DEC_FLUSH_SD : in std_logic;
        IF2DEC_POP_SD : in std_logic; 
        IF2DEC_EMPTY_SI : out std_logic; 

        PC_RD : in std_logic_vector(31 downto 0);
        INSTR_RI : out std_logic_vector(31 downto 0);
        PC_IF2DEC_RI : out std_logic_vector(31 downto 0)
    );
end component; 

component dec 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Reg interface
        RDATA1_SR, RDATA2_SR : in std_logic_vector(31 downto 0);
        ADR_DEST_SR : out std_logic_vector(5 downto 0);
        RADR1_SR, RADR2_SR : out std_logic_vector(5 downto 0);
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
        BP_EXE2MEM_EMPTY : in std_logic;
        BP_DEST_RM : in std_logic_vector(5 downto 0);
        BP_MEM_RES_RM : in std_logic_vector(31 downto 0);
        BP_R1_VALID_RD, BP_R2_VALID_RD : out std_logic;
        BP_RADR1_RD, BP_RADR2_RD : out std_logic_vector(5 downto 0)    
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
        DATA_RE : out std_logic_vector(31 downto 0);
        DEST_RE : out std_logic_vector(5 downto 0);
        MEM_SIZE_RE : out std_logic_vector(1 downto 0);
        WB_RE : out std_logic;
        MEM_SIGN_EXTEND_RE : out std_logic;
        MEM_LOAD_RE, MEM_STORE_RE : out std_logic;
        EXE2MEM_EMPTY_SE : out std_logic;
        DEC2EXE_POP_SE : out std_logic
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
        RES_RE, DATA_RE : in std_logic_vector(31 downto 0);
        DEST_RE : in std_logic_vector(5 downto 0);
        MEM_SIZE_RE : in std_logic_vector(1 downto 0);
        WB_RE, SIGN_EXTEND_RE, LOAD_RE, STORE_RE : in std_logic;
        
        -- exe2mem interface
        EXE2MEM_EMPTY_SM : in std_logic;
        EXE2MEM_POP_SM : out std_logic;

        -- mem2wbk interface
        MEM2WBK_POP_SM : in std_logic;
        MEM2WBK_EMPTY_SM : out std_logic;
        
        -- Wbk interface
        DATA_RM : out std_logic_vector(31 downto 0);
        DEST_RM : out std_logic_vector(5 downto 0);
        MEM_SIZE_RM : out std_logic_vector(1 downto 0);
        WB_RM, SIGN_EXTEND_RM, LOAD_RM : out std_logic
    ); 
end component; 

component wbk 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Mem 
        DATA_RM : in std_logic_vector(31 downto 0);
        DEST_RM : in std_logic_vector(5 downto 0);
        MEM_SIZE_RM : in std_logic_vector(1 downto 0);
        WB_RM : in std_logic;
        SIGN_EXTEND_RM : in std_logic;
        LOAD_RM : in std_logic;

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

begin 

-------------------------
-- Instanciation 
-------------------------
ifetch_i : ifetch 
    port map(
        -- global interface
        clk, reset_n,

        -- Icache interface
        IC_INST_SI,
        IC_STALL_SI,
        ADR_SI,
        ADR_VALID_SI,

        -- dec2if interface 
        DEC2IF_EMPTY_SD,
        DEC2IF_POP_SI,

        -- if2dec interface 
        IF2DEC_FLUSH_SD,
        IF2DEC_POP_SD, 
        IF2DEC_EMPTY_SI, 

        PC_RD,
        INSTR_RI,
        PC_IF2DEC_RI
    );

dec_i : dec
    port map(
        -- global interface
        clk, reset_n,

        -- Reg interface
        RDATA1_SR, RDATA2_SR,
        ADR_DEST_SR,
        RADR1_SD, RADR2_SD,
        WRITE_PC_SD,
        WRITE_PC_ENABLE_SD,
        READ_PC_SR,

        -- Exe interface 
        OP1_RD, OP2_RD,
        CMD_RD,
        NEG_OP2_RD, 
        WB_RD,
        DEST_RD,
        SELECT_SHIFT_RD,
        SLT_RD, SLTU_RD,    
        MEM_DATA_RD,
        MEM_LOAD_RD, MEM_STORE_RD, MEM_SIGN_EXTEND_RD,
        MEM_SIZE_RD,

        -- dec2if interface
        DEC2IF_POP_SI, 
        DEC2IF_EMPTY_SD,
        PC_RD,

        -- if2dec interface
        INSTR_RI, PC_IF2DEC_RI,
        IF2DEC_EMPTY_SI,
        IF2DEC_POP_SD,
        IF2DEC_FLUSH_SD,

        -- dec2exe interface
        DEC2EXE_POP_SE,
        DEC2EXE_EMPTY_SD,

        -- Bypasses
        BP_DEST_RE,
        BP_EXE_RES_RE,
        BP_MEM_LOAD_RE,
        BP_EXE2MEM_EMPTY,
        BP_DEST_RM,
        BP_MEM_RES_RM,
        BP_R1_VALID_RD, BP_R2_VALID_RD,
        BP_RADR1_RD, BP_RADR2_RD
    );

exec_i : exec 
    port map(
        -- global interface 
        clk, reset_n,

        OP1_RD, OP2_RD,
        RADR1_RD, RADR2_RD,
        MEM_DATA_RD,
        DEST_RD,
        CMD_RD,
        MEM_SIZE_RD,
        NEG_OP2_RD,
        WB_RD,
        MEM_SIGN_EXTEND_RD,
        SELECT_SHIFT_RD,
        MEM_LOAD_RD, MEM_STORE_RD,
        EXE2MEM_POP_SM,
        DEC2EXE_EMPTY_SD,
        SLT_RD, SLTU_RD,

        RES_RE,    
        DATA_RE,
        DEST_RE,
        MEM_SIZE_RE,
        WB_RE,
        MEM_SIGN_EXTEND_RE,
        MEM_LOAD_RE, MEM_STORE_RE,
        EXE2MEM_EMPTY_SE,
        DEC2EXE_POP_SE
    );

mem_i : mem 
    port map(
        -- global interface
        clk, reset_n,

        -- Mcache interface
        MCACHE_RESULT_SM,
        MCACHE_STALL_SM,
        MCACHE_ADR_SM, MCACHE_DATA_SM,
        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM,
        byt_sel, 

        -- Exe interface
        RES_RE, DATA_RE,
        DEST_RE,
        MEM_SIZE_RE,
        WB_RE, MEM_SIGN_EXTEND_RE, MEM_LOAD_RE, MEM_STORE_RE,

        -- exe2mem interface
        EXE2MEM_EMPTY_SE,
        EXE2MEM_POP_SM,

        -- mem2wbk interface
        MEM2WBK_POP_SW, 
        MEM2WBK_EMPTY_SM,
        
        -- Wbk interface
        DATA_RM,
        DEST_RM,
        MEM_SIZE_RM,
        WB_RM, SIGN_EXTEND_RM, LOAD_RM
    );

wbk_i : wbk 
    port map(
        -- global interface
        clk, reset_n,

        -- Mem 
        DATA_RM,
        DEST_RM,
        MEM_SIZE_RM,
        WB_RM,
        SIGN_EXTEND_RM,
        LOAD_RM, 

        -- mem2wbk interface
        MEM2WBK_EMPTY_SM,
        MEM2WBK_POP_SW,

        -- Reg interface
        REG_DATA_SW,
        REG_DEST_SW,
        REG_WB_SW 
    );
 
reg_i : reg 
    port map(
        clk, reset_n,
        PC_INIT, 
        
        -- read
        RDATA1_SR, RDATA2_SR, 
        RADR1_SD, RADR2_SD, 
        
        -- write 
        REG_DATA_SW, 
        REG_DEST_SW, 
        WB_RM,
        
        WRITE_PC_SD, 
        WRITE_PC_ENABLE_SD,
        READ_PC_SR
    );

DEBUG_PC_READ <= READ_PC_SR; 

end archi;