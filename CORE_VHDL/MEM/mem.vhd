library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
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

        -- Exe interface
        EXE_RES_SM, MEM_DATA_SM : in std_logic_vector(31 downto 0);
        EXE_DEST_SM : in std_logic_vector(5 downto 0);
        EXE_MEM_SIZE_SM : in std_logic_vector(1 downto 0);
        EXE_WB_SM, SIGN_EXTEND_SM, LOAD_SM, STORE_SM : in std_logic;

        -- exe2mem interface
        EXE2MEM_EMPTY_SM : in std_logic;
        EXE2MEM_POP_SM : out std_logic;

        -- mem2wbk interface
        MEM2WBK_POP_SM : in std_logic;
        MEM2WBK_EMPTY_SM : out std_logic;
        
        -- Wbk interface
        WBK_DATA_SM : out std_logic_vector(31 downto 0);
        WBK_DEST_SM : out std_logic_vector(5 downto 0);
        WBK_MEM_SIZE_SM : out std_logic_vector(1 downto 0);
        WBK_WB_SM, WBK_SIGN_EXTEND_SM, WBK_LOAD_SM : out std_logic
    ); 
end mem;

architecture archi of mem is 
signal mem2wbk_din, mem2wbk_dout : std_logic_vector(42MEM2WBK_POP_SM downto 0);
signal mem2wbk_push, mem2wbk_full : std_logic;
signal stall, wb : std_logic;

signal load_data : std_logic_vector(31 downto 0);
signal load_byte, load_halfword, load_word : std_logic_vector(31 downto 0);

signal data : std_logic_vector(31 downto 0);

begin

-- Intanciation 
mem2wbk : entity work.fifo
    generic map(N => 43)
    port map(
        clk => clk,
        reset_n => reset_n,
        DIN => mem2wbk_din, 
        PUSH => mem2wbk_push,
        POP => MEM2WBK_POP_SM, 
        FULL => mem2wbk_full, 
        EMPTY => MEM2WBK_EMPTY_SM, 
        DOUT => mem2wbk_dout
    );
    
-- fifo concat   
mem2wbk_din(31 downto 0) <= data; 
mem2wbk_din(37 downto 32) <= EXE_DEST_SM; 
mem2wbk_din(39 downto 38) <= EXE_MEM_SIZE_SM;
mem2wbk_din(40) <= wb;
mem2wbk_din(41) <= SIGN_EXTEND_SM;
mem2wbk_din(42) <= LOAD_SM; 

-- fifo unconcat 
WBK_DATA_SM <= mem2wbk_dout(31 downto 0);
WBK_DEST_SM <= mem2wbk_dout(37 downto 32);
WBK_MEM_SIZE_SM <= mem2wbk_dout(39 downto 38);
WBK_WB_SM <= mem2wbk_dout(40);
WBK_SIGN_EXTEND_SM <= mem2wbk_dout(41);
WBK_LOAD_SM <= mem2wbk_dout(42);

-- fifo manage 
stall <= MCACHE_STALL_SM or mem2wbk_full or EXE2MEM_EMPTY_SM;
wb <= EXE_WB_SM or LOAD_SM;
mem2wbk_push <= (not stall) and wb;
EXE2MEM_POP_SM <= not stall;

-- Mcache 
MCACHE_DATA_SM <= MEM_DATA_SM; 
MCACHE_ADR_SM <= EXE_RES_SM;
MCACHE_LOAD_SM <= LOAD_SM;
MCACHE_STORE_SM <= STORE_SM;
MCACHE_ADR_VALID_SM <= not EXE2MEM_EMPTY_SM;

-- sign extend and load size
load_byte <= (x"00000000" or (x"000000"&MCACHE_RESULT_SM(7 downto 0))) when SIGN_EXTEND_SM = '0' else
             (x"FFFFFFFF" and (x"FFFFFF"&MCACHE_RESULT_SM(7 downto 0)));

load_halfword <= (x"00000000" or (x"0000"&MCACHE_RESULT_SM(15 downto 0))) when SIGN_EXTEND_SM = '0' else 
                 (x"FFFFFFFF") and (x"FFFF"&MCACHE_RESULT_SM(15 downto 0));

load_word <= MCACHE_RESULT_SM; 

load_data <= load_byte when EXE_MEM_SIZE_SM = "10" else 
             load_halfword when EXE_MEM_SIZE_SM = "01" else
             load_word; 

-- Data selection to be written in register
data <= load_data when LOAD_SM = '1' else 
        EXE_RES_SM;

end archi;