library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.all;

entity core_tb is 
end core_tb;

architecture simu of core_tb is 

-- functions 
function read_mem(adr : integer) return integer is 
begin 
    assert false severity failure;
end read_mem; 
attribute foreign of read_mem : function is "VHPIDIRECT read_mem";    

function write_mem(adr : integer; data : integer; byte_select : integer; time : integer) return integer is 
begin 
    assert false severity failure;
end write_mem; 
attribute foreign of write_mem : function is "VHPIDIRECT write_mem";    

function get_startpc(a : integer) return integer is 
begin 
    assert false severity failure; 
end get_startpc;
attribute foreign of get_startpc : function is "VHPIDIRECT get_startpc";

function get_good(a : integer) return integer is 
begin 
    assert false severity failure; 
end get_good; 
attribute foreign of get_good : function is  "VHPIDIRECT get_good";

function get_bad(a : integer) return integer is 
begin 
    assert false severity failure; 
end get_bad; 
attribute foreign of get_bad : function is  "VHPIDIRECT get_bad";

function end_simulation(result : integer; riscof_enable : integer) return integer is 
begin
    assert false severity failure; 
end end_simulation; 
attribute foreign of end_simulation : function is  "VHPIDIRECT end_simulation";

function get_riscof_en(z : integer) return integer is 
begin 
    assert false severity failure;
end get_riscof_en; 
attribute foreign of get_riscof_en : function is "VHPIDIRECT get_riscof_en";    

function get_end_riscof(z : integer) return integer is 
begin 
    assert false severity failure;
end get_end_riscof; 
attribute foreign of get_end_riscof : function is "VHPIDIRECT get_end_riscof";    

function to_string ( a: std_logic_vector) return string is
variable b : string (1 to a'length) := (others => NUL);
variable stri : integer := 1; 
begin
    for i in a'range loop
        b(stri) := std_logic'image(a((i)))(2);  
        stri := stri+1;
    end loop;
    return b;
end function;

------------------------------
-- core signals instance
------------------------------
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

-- Simulation 
constant NCYCLES : integer := 100000000; 
signal CYCLES : integer := 0; 
signal good_adr, bad_adr, exception_adr : std_logic_vector(31 downto 0);
signal end_simu : std_logic := '0'; 
signal result : integer := 0;  
signal timeout : integer := 0; 
signal time : integer := 0; 
-- riscof
signal riscof_en : integer := 0; 
signal riscof_end_adr : std_logic_vector(31 downto 0);
signal cpt_end : integer := 0;
constant cpt_max : integer := 10;
signal riscof_end : integer := 0;
begin 

good_adr        <=  std_logic_vector(to_signed(get_good(0), 32));
bad_adr         <=  std_logic_vector(to_signed(get_bad(0), 32));
exception_adr   <=  x"00011064"; -- because of flemme, TODO in ram_sim.c
riscof_en       <=  get_riscof_en(0);
riscof_end_adr  <=  std_logic_vector(to_signed(get_end_riscof(0), 32));

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
variable r0 : integer;
variable un : integer := 1;
begin         
    clk <= '0'; 
    wait for 5 ns; 
    clk <= '1'; 
     CYCLES <= CYCLES + 1; 
    wait for 5 ns; 
    if CYCLES = 1 then 
        if riscof_en = 1 then 
            assert false report "RISCOF simulation begin" severity note; 
        else
            assert false report "simulation begin" severity note; 
        end if;
    end if; 
    if end_simu = '1' or cpt_end = cpt_max then 
        assert false report "end of simulation" severity note; 
        r0 := end_simulation(result,un);
        wait; 
    end if; 
    if riscof_end = 1 then 
        cpt_end <= cpt_end + 1;
    end if; 
    if CYCLES = NCYCLES then 
        timeout <= 1; 
        assert false report "end of simulation (timeout)" severity note; 
        r0 := end_simulation(un,0);
       wait; 
    end if;
    
    -- if ADR_SI = riscof_end then   
    -- report "end riscof test" severity note; 
    --     r0 := end_simulation(0,1);
    -- end if;
end process; 


process(clk)
begin 
    time <= time + 5; 
end process; 

reset_n <= '0', '1' after 6 ns;

MCACHE_STALL_SM <= '0';

IC_STALL_SI <= '0';
PC_INIT <= std_logic_vector(to_signed(get_startpc(0), 32));

icache : process(ADR_SI, ADR_VALID_SI)
variable adr_int : integer; 
variable inst_int : integer; 
variable intermed : signed(ADR_SI'range); 
begin
    if riscof_end_adr = (riscof_end_adr'range => '0') then
        if ADR_VALID_SI = '1' then 
            if ADR_SI = bad_adr then 
                assert false report "Test failed" severity error; 
                result <= 1;
                end_simu <= '1';              
            elsif ADR_SI = good_adr then 
                assert false report "Test success" severity note; 
                result <= 0;
                end_simu <= '1';  
            elsif ADR_SI = exception_adr then 
                assert false report "Exception occured" severity warning; 
                result <= 2;    
                end_simu <= '1'; 
            else
                intermed    := signed(ADR_SI); 
                adr_int     := to_integer(intermed);
                inst_int    := read_mem(adr_int);
                IC_INST_SI  <= std_logic_vector(to_signed(inst_int, 32));

            end if; 

        end if; 
    else 
        if ADR_VALID_SI = '1' then 
            if ADR_SI = riscof_end_adr then 
                assert false report "RISCOF test end" severity note; 
                result <= 0 ;
                riscof_end <= 1;
            else
                intermed    := signed(ADR_SI); 
                adr_int     := to_integer(intermed);
                inst_int    := read_mem(adr_int);
                IC_INST_SI  <= std_logic_vector(to_signed(inst_int, 32));
            end if;
        end if;
    end if;
end process; 


dcache : process(clk, MCACHE_ADR_VALID_SM, MCACHE_STORE_SM, MCACHE_LOAD_SM, MCACHE_DATA_SM, MCACHE_ADR_SM, byt_sel)
variable read0      : integer; -- ignore 
variable adr_u      : signed(MCACHE_ADR_SM'range); 
variable adr_int    : integer := 0;
variable data_u     : signed(MCACHE_DATA_SM'range);
variable data_int   : integer := 0;
variable byt_sel_u  : unsigned(byt_sel'range);
variable byt_sel_i  : integer := 0;
begin 
    adr_u       := signed(MCACHE_ADR_SM);
    adr_int     := to_integer(adr_u);
    data_u      := signed(MCACHE_DATA_SM);
    data_int    := to_integer(data_u);
    byt_sel_u   := unsigned(byt_sel);
    byt_sel_i   := to_integer(byt_sel_u);

    if reset_n = '0' then 
    -- hoping to find a better solution to avoid not wanted mem access
    elsif falling_edge(clk) then 
        if MCACHE_ADR_VALID_SM = '1' then 
            if MCACHE_STORE_SM = '1' then  
                read0 := write_mem(adr_int, data_int, byt_sel_i, time);
            elsif MCACHE_LOAD_SM = '1' then 
                MCACHE_RESULT_SM <= std_logic_vector(to_signed(read_mem(adr_int), 32));
            else 
                assert false report "Adr mem access valid but no command" severity error; 
            end if; 
        end if; 
    end if; 

end process;

end simu;