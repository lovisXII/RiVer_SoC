library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

library std; 
use std.textio.all;


entity tb_core is 
end tb_core;

architecture simu of tb_core is 

-- global interface
signal clk, reset_n : std_logic := '0';

-- Mcache interface
signal MCACHE_RESULT_SM : std_logic_vector(31 downto 0);
signal MCACHE_STALL_SM : std_logic;

signal MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM : std_logic;
signal MCACHE_DATA_SM : std_logic_vector(31 downto 0);
signal MCACHE_ADR_SM : std_logic_vector(31 downto 0);

-- Icache interface
signal IC_INST_SI : std_logic_vector(31 downto 0);
signal IC_STALL_SI : std_logic; 

signal ADR_SI : std_logic_vector(31 downto 0);
signal ADR_VALID_SI : std_logic; 

-- Debug 
signal PC_INIT : std_logic_vector(31 downto 0);
signal DEBUG_PC_READ : std_logic_vector(31 downto 0);

-- file
signal eof : std_logic := '0';
file program : text;
constant filename : string := "../../CORE/a.out.txt";


begin 

process 
begin 

read_file : process 
variable fstatus : file_open_status; 
variable file_line : line; 
variable var_instr : std_logic_vector(31 downto 0); 
variable end_of_line : boolean; 

begin 
    file_open(fstatus, program, filename, read_mode);
    wait until reset_n = '1'; 
    while not endfile(program) loop 
        wait until clk = '1'; 
        hreadline(program, inline); 
        read(inline, char, end_of_line);
    end loop;
    wait;
end process; 
            

core0 : entity work.core 
    port map(
        -- global interface
        clk, reset_n,

        -- Mcache interface
        MCACHE_RESULT_SM,
        MCACHE_STALL_SM,

        MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM,
        MCACHE_DATA_SM,
        MCACHE_ADR_SM,

        -- Icache interface
        IC_INST_SI,
        IC_STALL_SI, 

        ADR_SI,
        ADR_VALID_SI, 

        -- Debug 
        PC_INIT,
        DEBUG_PC_READ
    );

clk <= not clk after 5 ns; 
reset_n <= '0', '1' after 10 ns;

MCACHE_RESULT_SM <= x"0C0C0C0C";
MCACHE_STALL_SM <= '0';

IC_INST_SI <= x"00408113"; -- 0000 0000 0100 0000 1000 0001 0001 0011 addi r2, r1, 4
IC_STALL_SI <= '0';

PC_INIT <= x"AAAAAAAA";

end simu; 