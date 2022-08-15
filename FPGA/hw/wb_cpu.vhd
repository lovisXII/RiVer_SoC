library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;
 
entity wb_cpu is
    port(
        clk, reset_n    :   in  std_logic; 
        
        ACK_I           :   in  std_logic;
        STB_O           :   out std_logic;
        WE_O            :   out std_logic;

        DAT_I           :   in  std_logic_vector(31 downto 0);
        DAT_O           :   out std_logic_vector(31 downto 0);
        SEL_O           :   out std_logic_vector(3 downto 0);
        ADR_O           :   out std_logic_vector(31 downto 0)
    );
end wb_cpu;

architecture archi of wb_cpu is

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

signal dcache_adr_v                 :   std_logic; 
signal dcache_store, dcache_load    :   std_logic; 
signal dcache_data, dcache_adr      :   std_logic_vector(31 downto 0);
signal dcache_stall                 :   std_logic;

signal icache_adr_v                 :   std_logic; 
signal icache_adr                   :   std_logic_vector(31 downto 0);
signal icache_stall                 :   std_logic;

signal start_pc, debug_pc           :   std_logic_vector(31 downto 0);

signal trans_i, trans_d             :   std_logic; 
signal no_data                      :   std_logic; 

type state is (idle, iread, dread, dwrite);
signal EP, EF : state; 

constant pc0 : std_logic_vector(31 downto 0) := x"000001a0";
constant four : std_logic_vector(31 downto 0) := x"00000004";

begin

start_pc <= x"00000000";--std_logic_vector(unsigned(pc0) - unsigned(four));

dcache_stall    <=  (not(ACK_I) and trans_d) or no_data;
icache_stall    <=  (not(ACK_I) and trans_i) or no_data;


river_core0 : core 
    port map(
        clk                     =>  clk,
        reset_n                 =>  reset_n,

        
        MCACHE_RESULT_SM        =>  DAT_I,
        MCACHE_STALL_SM         =>  icache_stall,

        MCACHE_ADR_VALID_SM     =>  dcache_adr_v,
        MCACHE_STORE_SM         =>  dcache_store,
        MCACHE_LOAD_SM          =>  dcache_load, 
        MCACHE_DATA_SM          =>  dcache_data,
        MCACHE_ADR_SM           =>  dcache_adr,
        byt_sel                 =>  SEL_O,

        
        IC_INST_SI              =>  DAT_I,
        IC_STALL_SI             =>  icache_stall,
        
        ADR_SI                  =>  icache_adr,         
        ADR_VALID_SI            =>  icache_adr_v,

        -- Debug 
        PC_INIT                 =>  start_pc,
        DEBUG_PC_READ           =>  debug_pc
    );

fsm_transition : process(clk, reset_n)    
begin 
    if reset_n = '0' then 
        EP  <=  idle; 
    elsif rising_edge(clk) then 
        EP  <=  EF; 
    end if;

    case EP is 
        when idle   =>  EF  <=  idle; 
            if dcache_adr_v = '1' and dcache_store = '1' then 
                EF  <=  dwrite; 
            elsif dcache_adr_v = '1' and dcache_load = '1' then 
                EF  <=  dread; 
            elsif icache_adr_v = '1' then 
                EF  <=  iread; 
            end if; 

        when dwrite =>  EF  <=  dwrite; 
            if ACK_I = '1' then 
                if icache_adr_v = '1' then 
                    EF  <=  iread;
                else
                    EF  <=  idle; 
                end if;
            end if; 

        when dread  =>  EF  <=  dread; 
            if ACK_I = '1' then 
                EF  <=  idle; 
            end if; 

        when iread  =>  EF  <=  iread; 
            if ACK_I = '1' then 
                EF  <=  idle;
            end if; 
    end case; 
end process; 

fsm_output : process(EP)
begin 
    case EP is 
        when    idle    =>  
            STB_O   <=  '0'; 
            WE_O    <=  '0'; 
            trans_d <=  '0'; 
            trans_i <=  '0';
            no_data <=  '1'; 
        when    dwrite  =>
            STB_O   <=  '1';
            WE_O    <=  '1';
            trans_d <=  '1';
            trans_i <=  '0';
            no_data <=  '1';
        when    dread   =>
            STB_O   <=  '1';
            WE_O    <=  '0';
            trans_d <=  '1';
            trans_i <=  '0';
            no_data <=  '0';
        when    iread   =>
            STB_O   <=  '1';
            WE_O    <=  '0';
            trans_d <=  '0';
            trans_i <=  '1';
            no_data <=  '0';
        end case; 
end process; 


DAT_O   <=  dcache_data when    trans_d = '1'   else 
            x"00000000"; 

ADR_O   <=  dcache_adr  when    trans_d = '1'   else 
            icache_adr  when    trans_i = '1'   else 
            x"00000000"; 


end archi;
