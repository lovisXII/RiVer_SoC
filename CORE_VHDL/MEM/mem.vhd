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

        -- exe2mem interface
        EXE2MEM_EMPTY_SM : in std_logic;
        EXE2MEM_POP_SM : out std_logic;

        -- mem2wbk interface
        MEM2WBK_POP_SM : in std_logic;
        MEM2WBK_EMPTY_SM : out std_logic;
        
        -- Wbk interface
        MEM_RES_RM : out std_logic_vector(31 downto 0);
        MEM_DEST_RM : out std_logic_vector(5 downto 0);
        MEM_SIZE_RM : out std_logic_vector(1 downto 0);
        WB_RM, SIGN_EXTEND_RM, LOAD_RM : out std_logic
    ); 
end mem;

architecture archi of mem is 
signal mem2wbk_din, mem2wbk_dout : std_logic_vector(42 downto 0);
signal mem2wbk_push, mem2wbk_full : std_logic;
signal stall, wb : std_logic;

signal load_data : std_logic_vector(31 downto 0);
signal load_byte, load_halfword, load_word : std_logic_vector(31 downto 0);

signal data : std_logic_vector(31 downto 0);

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
mem2wbk_din(37 downto 32) <= DEST_RE; 
mem2wbk_din(39 downto 38) <= MEM_SIZE_RE;
mem2wbk_din(40) <= wb;
mem2wbk_din(41) <= SIGN_EXTEND_RE;
mem2wbk_din(42) <= LOAD_RE; 

-- fifo unconcat 
MEM_RES_RM <= mem2wbk_dout(31 downto 0);
MEM_DEST_RM <= mem2wbk_dout(37 downto 32);
MEM_SIZE_RM <= mem2wbk_dout(39 downto 38);
WB_RM <= mem2wbk_dout(40);
SIGN_EXTEND_RM <= mem2wbk_dout(41);
LOAD_RM <= mem2wbk_dout(42);

-- fifo manage 
stall <= MCACHE_STALL_SM or mem2wbk_full or EXE2MEM_EMPTY_SM;
wb <= WB_RE or LOAD_RE;
mem2wbk_push <= (not stall) and wb;
EXE2MEM_POP_SM <= not stall;

-- Mcache 
MCACHE_DATA_SM <= MEM_DATA_RE; 
MCACHE_ADR_SM <= RES_RE;
MCACHE_LOAD_SM <= LOAD_RE;
MCACHE_STORE_SM <= STORE_RE;
MCACHE_ADR_VALID_SM <= not EXE2MEM_EMPTY_SM;

-- sign extend and load size
load_byte <= (x"00000000" or (x"000000"&MCACHE_RESULT_SM(7 downto 0))) when SIGN_EXTEND_RE = '0' else
             (x"FFFFFFFF" and (x"FFFFFF"&MCACHE_RESULT_SM(7 downto 0)));

load_halfword <= (x"00000000" or (x"0000"&MCACHE_RESULT_SM(15 downto 0))) when SIGN_EXTEND_RE = '0' else 
                 (x"FFFFFFFF") and (x"FFFF"&MCACHE_RESULT_SM(15 downto 0));

load_word <= MCACHE_RESULT_SM; 

load_data <= load_byte when MEM_SIZE_RE = "10" else 
             load_halfword when MEM_SIZE_RE = "01" else
             load_word; 

-- byte select 
byt_sel <=  "0001" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "00") else 
            "0010" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "01") else   
            "0100" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "10") else 
            "1000" when (MEM_SIZE_RE = "10" and RES_RE(1 downto 0) = "11") else
            "0011" when (MEM_SIZE_RE = "01" and RES_RE(1 downto 0) = "00") else 
            "1100" when (MEM_SIZE_RE = "01" and RES_RE(1 downto 0) = "10") else
            "1111" when (MEM_SIZE_RE = "00" and RES_RE(1 downto 0) = "00") else
            "0000";   

-- Data selection to be written in register file
data <= load_data when LOAD_RE = '1' else 
        RES_RE;

end archi;