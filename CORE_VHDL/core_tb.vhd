library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.all;

entity core_tb is 
end core_tb;

architecture simu of core_tb is 

-- functions 
function get_inst(adr : integer) return integer is 
begin 
    assert false severity failure;
end get_inst; 
attribute foreign of get_inst : function is "VHPIDIRECT get_inst";    

function get_mem(adr : integer) return integer is 
begin 
    assert false severity failure;
end get_mem; 
attribute foreign of get_mem : function is "VHPIDIRECT get_mem";    

function write_mem(adr : integer; data : integer) return integer is 
begin 
    assert false severity failure;
end write_mem; 
attribute foreign of write_mem : function is "VHPIDIRECT write_mem";    

function get_startpc(a : integer) return integer is 
begin 
    assert false severity failure; 
end get_startpc;
attribute foreign of get_startpc : function is "VHPIDIRECT get_startpc";

-- global interface
signal clk : std_logic := '1';
signal reset_n : std_logic := '0';

-- Mcache interface
signal MCACHE_RESULT_SM : std_logic_vector(31 downto 0);
signal MCACHE_STALL_SM : std_logic;

signal MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM : std_logic;
signal MCACHE_DATA_SM : std_logic_vector(31 downto 0);
signal MCACHE_ADR_SM : std_logic_vector(31 downto 0);
signal byt_sel : std_logic_vector(3 downto 0);

-- Icache interface
signal IC_INST_SI : std_logic_vector(31 downto 0);
signal IC_STALL_SI : std_logic; 

signal ADR_SI : std_logic_vector(31 downto 0);
signal ADR_VALID_SI : std_logic; 

-- Debug 
signal PC_INIT : std_logic_vector(31 downto 0);
signal DEBUG_PC_READ : std_logic_vector(31 downto 0);

constant NCYCLES : integer := 100; 
signal CYCLES : integer range 0 to NCYCLES+1 := 0; 

component core
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
end component; 

begin 

core0 : core
    port map(
        -- global interface
        clk, reset_n,

        -- Mcache interface
        MCACHE_RESULT_SM,
        MCACHE_STALL_SM,

        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM,
        MCACHE_DATA_SM,
        MCACHE_ADR_SM,
        byt_sel, 
        -- Icache interface
        IC_INST_SI,
        IC_STALL_SI, 

        ADR_SI,
        ADR_VALID_SI, 

        -- Debug 
        PC_INIT,
        DEBUG_PC_READ
    );


clk_gen : process
begin 
    clk <= '0'; 
    wait for 5 ns; 
    clk <= '1'; 
    CYCLES <= CYCLES + 1; 
    wait for 5 ns; 
    if CYCLES = NCYCLES then 
        assert false report "end of simulation" severity note; 
        wait; 
    end if; 
end process; 

reset_n <= '0', '1' after 6 ns;

MCACHE_STALL_SM <= '0';

IC_STALL_SI <= '0';
PC_INIT <= std_logic_vector(to_unsigned(get_startpc(0), 32));

process(ADR_SI, ADR_VALID_SI)
begin
    if ADR_VALID_SI = '1' then 
        IC_INST_SI <= std_logic_vector(to_unsigned(get_inst(to_integer(unsigned(ADR_SI))), 32));
    end if; 
end process; 


mem_access : process(MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM, MCACHE_DATA_SM, MCACHE_ADR_SM)
variable read : integer;
begin 
    if MCACHE_ADR_VALID_SM = '1' then 
        if MCACHE_STORE_SM = '1' then 
            read := write_mem(to_integer(unsigned(MCACHE_ADR_SM)), to_integer(unsigned(MCACHE_DATA_SM)));
        elsif MCACHE_LOAD_SM = '1' then 
            MCACHE_RESULT_SM <= std_logic_vector(to_unsigned(get_mem(to_integer(unsigned(MCACHE_ADR_SM))), 32));
        end if; 

    end if; 
end process;

end simu;