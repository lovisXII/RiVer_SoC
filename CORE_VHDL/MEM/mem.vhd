library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity mem is
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

        PC_EXE2MEM_RE : in std_logic_vector(31 downto 0);

        -- exe2mem interface
        EXE2MEM_EMPTY_SE : in std_logic;
        EXE2MEM_POP_SM : out std_logic;

        -- mem2wbk interface
        MEM2WBK_POP_SW : in std_logic;
        MEM2WBK_EMPTY_SM : out std_logic;
        
        -- Wbk interface
        MEM_RES_RM : out std_logic_vector(31 downto 0);
        MEM_DEST_RM : out std_logic_vector(5 downto 0);
        WB_RM : out std_logic;
        CSR_RDATA_RM : out std_logic_vector(31 downto 0);
        CSR_WENABLE_RM : out std_logic;
        
        -- CSR 
        CSR_WENABLE_RE  : in std_logic;
        CSR_WADR_RE     : in std_logic_vector(11 downto 0);
        CSR_RDATA_RE    : in std_logic_vector(31 downto 0);

        CSR_WADR_SM     : out std_logic_vector(11 downto 0);
        CSR_WDATA_SM    : out std_logic_vector(31 downto 0);
        CSR_ENABLE_SM   : out std_logic; 

        MSTATUS_WDATA_SM    : out std_logic_vector(31 downto 0);
        MIP_WDATA_SM        : out std_logic_vector(31 downto 0);
        MEPC_WDATA_SM       : out std_logic_vector(31 downto 0);
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
        EBREAK_RE : in std_logic; 
        PC_BRANCH_VALUE_RE : in std_logic_vector(31 downto 0);

        BUS_ERROR_SX : in std_logic;

        EXCEPTION_SM : out std_logic;
        CURRENT_MODE_SM : out std_logic_vector(1 downto 0);
        RETURN_ADRESS_SM : out std_logic_vector(31 downto 0);
        MRET_SM : out std_logic
    ); 
end mem;

architecture archi of mem is 
signal mem2wbk_din, mem2wbk_dout : std_logic_vector(71 downto 0);
signal mem2wbk_push, mem2wbk_full : std_logic;
signal stall_sm, wb : std_logic;

signal load_data : std_logic_vector(31 downto 0);
signal load_byte, load_halfword, load_word : std_logic_vector(31 downto 0);

signal data_sm, data_store_sm, data_byte_store_sm, data_half_store_sm : std_logic_vector(31 downto 0);

signal byt_sel_sm : std_logic_vector(3 downto 0);

-- CSR & Exception 
signal exception : std_logic := '0'; 
signal mode_sm : std_logic_vector(1 downto 0) := "11";
signal new_mode : std_logic_vector(1 downto 0);
signal machine_mode_condition : std_logic;

signal mstatus_x : std_logic_vector(31 downto 0);
signal mcause_x  : std_logic_vector(31 downto 0);
signal mtval_x   : std_logic_vector(31 downto 0);

component fifo
    generic(N : integer);
    port(
        clk     : in    std_logic; 
        reset_n : in    std_logic; 
        DIN     : in    std_logic_vector(N-1 downto 0);
        PUSH    : in    std_logic;
        POP     : in    std_logic;
        FULL    : out   std_logic;
        EMPTY   : out   std_logic;
        DOUT    : out   std_logic_vector(N-1 downto 0)
    );
end component;


begin

-- Intanciation 
mem2wbk : fifo
    generic map(N => 72)
    port map(
        clk => clk,
        reset_n => reset_n,
        DIN => mem2wbk_din, 
        PUSH => mem2wbk_push,
        POP => MEM2WBK_POP_SW, 
        FULL => mem2wbk_full, 
        EMPTY => MEM2WBK_EMPTY_SM, 
        DOUT => mem2wbk_dout
    );
    
-- fifo concat   
mem2wbk_din(31 downto 0) <= data_sm; 
mem2wbk_din(37 downto 32) <= DEST_RE; 
mem2wbk_din(38) <= wb;
--mem2wbk_din(70 downto 39) <= PC_EXE2MEM_RE;
mem2wbk_din(39) <= CSR_WENABLE_RE; 
mem2wbk_din(71 downto 40) <= CSR_RDATA_RE;

-- fifo unconcat 
MEM_RES_RM <= mem2wbk_dout(31 downto 0);
MEM_DEST_RM <= mem2wbk_dout(37 downto 32);
WB_RM <= mem2wbk_dout(38);
--PC_MEM2WBK_RM <= mem2wbk_dout(70 downto 39);
CSR_WENABLE_RM <= mem2wbk_dout(39);
CSR_RDATA_RM <= mem2wbk_dout(71 downto 40);

-- fifo manage 
stall_sm <= MCACHE_STALL_SM or mem2wbk_full or EXE2MEM_EMPTY_SE;
wb <= WB_RE or LOAD_RE;
mem2wbk_push <= (not stall_sm) and wb;
EXE2MEM_POP_SM <= not stall_sm;


data_byte_store_sm(31 downto 8)     <= (others => '0');
data_byte_store_sm(7 downto 0)      <= MEM_DATA_RE(7 downto 0);

data_half_store_sm(31 downto 16)    <= (others => '0');
data_half_store_sm(15 downto 0)     <= MEM_DATA_RE(15 downto 0);

data_store_sm       <=  data_byte_store_sm when MEM_SIZE_RE = "10" else 
                        data_half_store_sm when MEM_SIZE_RE = "01" else 
                        MEM_DATA_RE;

-- Mcache 
MCACHE_DATA_SM <= data_store_sm; 
MCACHE_ADR_SM <= RES_RE;
MCACHE_LOAD_SM <= LOAD_RE;
MCACHE_STORE_SM <= STORE_RE;
MCACHE_ADR_VALID_SM <= (not EXE2MEM_EMPTY_SE) and (STORE_RE or LOAD_RE);

-- sign extend and load size 
load_byte(31 downto 8)  <=  x"000000" when SIGN_EXTEND_RE = '0' else 
                            x"FFFFFF"; 
load_byte(7 downto 0)   <=  MCACHE_RESULT_SM(31 downto 24)  when byt_sel_sm = "1000" else
                            MCACHE_RESULT_SM(23 downto 16)  when byt_sel_sm = "0100" else 
                            MCACHE_RESULT_SM(15 downto 8)   when byt_sel_sm = "0010" else
                            MCACHE_RESULT_SM(7 downto 0)    when byt_sel_sm = "0001" else
                            x"00"; 

load_halfword(31 downto 16) <=  x"0000" when SIGN_EXTEND_RE = '0' else 
                                x"FFFF"; 
load_halfword(15 downto 0)  <=  MCACHE_RESULT_SM(15 downto 0)   when byt_sel_sm = "0011" else 
                                MCACHE_RESULT_SM(31 downto 16)  when byt_sel_sm = "1100" else
                                x"0000";

load_word <= MCACHE_RESULT_SM; 

load_data <= load_byte when MEM_SIZE_RE = "10" else 
             load_halfword when MEM_SIZE_RE = "01" else
             load_word; 

-- byte select 
byt_sel_sm  <=  "0001" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "00") else 
                "0010" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "01") else   
                "0100" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "10") else 
                "1000" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "11") else
                "0011" when (MEM_SIZE_RE = "01" and RES_RE(1 downto 0) = "00") else 
                "1100" when (MEM_SIZE_RE = "01" and RES_RE(1 downto 0) = "10") else
                "1111" when (MEM_SIZE_RE = "00" and RES_RE(1 downto 0) = "00") else
                "0000";   
byt_sel     <=  byt_sel_sm;     
-- Data selection to be written in register file
data_sm <=  load_data when LOAD_RE = '1' else 
            RES_RE;

--------------------
-- CSR & Exception 
--------------------
exception <=    '1' when (EXCEPTION_RE = '1' or BUS_ERROR_SX = '1') else 
                '0'; 

-- CSR write
CSR_WADR_SM     <= CSR_WADR_RE; 
CSR_WDATA_SM    <= RES_RE; 
CSR_ENABLE_SM   <= CSR_WENABLE_RE and not exception;  

-- MSTATUS 
mstatus_x(31)           <= '0'; 
mstatus_x(30 downto 13) <= MSTATUS_RC(30 downto 13); 
mstatus_x(12 downto 11) <= mode_sm; 
mstatus_x(10 downto 8)  <= MSTATUS_RC(10 downto 8);
mstatus_x(7)            <= MSTATUS_RC(3);
mstatus_x(6 downto 4)   <= MSTATUS_RC(6 downto 4);
mstatus_x(3)            <= '0'; 
mstatus_x(2 downto 0)   <= MSTATUS_RC(2 downto 0);

-- MCAUSE
mcause_x    <=  x"00000018" when ENV_CALL_WRONG_MODE_RE = '1'       else 
                x"00000007" when STORE_ACCESS_FAULT_RE  = '1'       else
                x"00000005" when ENV_CALL_WRONG_MODE_RE = '1'       else 
                x"00000006" when STORE_ADRESS_MISALIGNED_RE = '1'  else 
                x"00000004" when LOAD_ADRESS_MISALIGNED_RE = '1'   else 
                x"0000000B" when ENV_CALL_M_MODE_RE     = '1'       else 
                x"00000009" when ENV_CALL_S_MODE_RE     = '1'       else 
                x"00000008" when ENV_CALL_U_MODE_RE     = '1'       else 
                x"00000003" when EBREAK_RE              = '1'       else 
                x"00000000" when INSTRUCTION_ADRESS_MISALIGNED_RE = '1' else 
                x"00000002" when ILLEGAL_INSTRUCTION_RE = '1'       else 
                x"00000001" when INSTRUCTION_ACCESS_FAULT_RE = '1'  else 
                x"00000000"; -- or debug value

-- MTVAL
mtval_x     <=  RES_RE when ((STORE_ACCESS_FAULT_RE or LOAD_ACCESS_FAULT_RE or STORE_ADRESS_MISALIGNED_RE or LOAD_ADRESS_MISALIGNED_RE) = '1') else 
                PC_BRANCH_VALUE_RE when INSTRUCTION_ADRESS_MISALIGNED_RE = '1' else
                x"00000000"; 

machine_mode_condition <= ENV_CALL_WRONG_MODE_RE or STORE_ACCESS_FAULT_RE or LOAD_ACCESS_FAULT_RE or STORE_ADRESS_MISALIGNED_RE or LOAD_ADRESS_MISALIGNED_RE or ENV_CALL_M_MODE_RE or ENV_CALL_S_MODE_RE or ENV_CALL_U_MODE_RE or EBREAK_RE or INSTRUCTION_ADRESS_MISALIGNED_RE or ILLEGAL_INSTRUCTION_RE or INSTRUCTION_ACCESS_FAULT_RE;   

new_mode <= "11" when machine_mode_condition = '1' else 
            "00";


-- to verify
mode : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        mode_sm <= "11"; 
    elsif rising_edge(clk) then 
        if exception = '1' then 
            mode_sm <= new_mode;
        end if; 
    end if; 
end process; 


RETURN_ADRESS_SM <= MEPC_SC when MRET_RE = '1' else 
                    x"00000000";


-- Ouput affectation 
EXCEPTION_SM <= exception; 
MEPC_WDATA_SM <= PC_EXE2MEM_RE;
MSTATUS_WDATA_SM <= mstatus_x; 
CURRENT_MODE_SM <= mode_sm; 
MRET_SM <= MRET_RE and exception;
MIP_WDATA_SM <= x"00000000"; 
MTVAL_WDATA_SM <= mtval_x; 
MCAUSE_WDATA_SM <= mcause_x; 
end archi;