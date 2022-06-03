library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity tb_mem is 
end tb_mem;

architecture simu of tb_mem is 
-- global interface
signal clk, reset_n : std_logic := '0';

-- Mcache interface
signal MCACHE_RESULT_SM : std_logic_vector(31 downto 0);
signal MCACHE_STALL_SM : std_logic;
signal MCACHE_ADR_SM, MCACHE_DATA_SM : std_logic_vector(31 downto 0);
signal MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM : std_logic;

-- Exe interface
signal EXE_RES_RE, MEM_DATA_RE : std_logic_vector(31 downto 0);
signal EXE_DEST_RE : std_logic_vector(5 downto 0);
signal EXE_MEM_SIZE_RE : std_logic_vector(1 downto 0);
signal EXE_WB_RE, SIGN_EXTEND_RE, LOAD_RE, STORE_RE :std_logic;

-- exe2mem interface
signal EXE2MEM_EMPTY_SM : std_logic;
signal EXE2MEM_POP_SM : std_logic;

-- mem2wbk interface
signal MEM2WBK_POP_SM : std_logic;
signal MEM2WBK_EMPTY_SM : std_logic;

-- Wbk interface
signal WBK_DATA_RM : std_logic_vector(31 downto 0);
signal WBK_DEST_RM : std_logic_vector(5 downto 0);
signal WBK_MEM_SIZE_RM : std_logic_vector(1 downto 0);
signal WBK_WB_RM, WBK_SIGN_EXTEND_RM, WBK_LOAD_RM : std_logic;

begin 

mem_i : entity work.mem 
    port map(        
        clk, reset_n,
        MCACHE_RESULT_SM, MCACHE_STALL_SM,
        MCACHE_ADR_SM, MCACHE_DATA_SM, MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM,
        EXE_RES_RE, MEM_DATA_RE, EXE_DEST_RE, EXE_MEM_SIZE_RE, EXE_WB_RE, SIGN_EXTEND_RE, LOAD_RE, STORE_RE, 
        EXE2MEM_EMPTY_SM, EXE2MEM_POP_SM, MEM2WBK_POP_SM, MEM2WBK_EMPTY_SM,        
        WBK_DATA_RM, WBK_DEST_RM, WBK_MEM_SIZE_RM,
        WBK_WB_RM, WBK_SIGN_EXTEND_RM, WBK_LOAD_RM
    );

clk <= not clk after 5 ns;
reset_n <= '1', '0' after 10 ns;

MCACHE_RESULT_SM <= x"ABCDEF00";
MCACHE_STALL_SM <= '0';

EXE_RES_RE <= x"EEEEEEEE";
MEM_DATA_RE <= x"BBBBBBBB";
EXE_DEST_RE <= "000011";

EXE_MEM_SIZE_RE <= "00", "01" after 65 ns, "10" after 75 ns, "01" after 105 ns, "10" after 115 ns;

EXE_WB_RE <= '1';
SIGN_EXTEND_RE <= '0', '1' after 95 ns; 
LOAD_RE <= '0', '1' after 55 ns;
STORE_RE <= '0';

EXE2MEM_EMPTY_SM <= '0';

MEM2WBK_POP_SM <= '1'; 

end simu;