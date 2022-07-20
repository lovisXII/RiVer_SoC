library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all;

entity divider is 
    port(
        -- global interface
        clk, reset_n    :   in  std_logic;

        OP1_SE, OP2_SE  :   in  std_logic_vector(31 downto 0);
        CMD_RD          :   in  std_logic_vector(1 downto 0);
        START_SE        :   in  std_logic;

        DONE_DIV        :   out std_logic;
        BUSY_DIV        :   out std_logic;
        DIVISER_RES     :   out std_logic_vector(31 downto 0)
    );
end divider; 

architecture archi of divider is 

type state is (idle, run);
signal EP, EF : state; 

signal op2 : std_logic_vector(31 downto 0);
signal shift_cpt : std_logic_vector(5 downto 0);

signal debug_state : std_logic;

begin 

-- state transition
fsm_state : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        EP  <=  idle; 
    elsif rising_edge(clk) then 
        EP  <=  EF;
    end if; 

    case EP is 
        when idle   =>  EF  <=  idle;
            if START_SE = '1' and op2 /= x"00000000" then 
                EF  <=  run;   
            end if; 
        when run    =>  EF  <=  run;
            if shift_cpt = "111111" then 
                EF  <=  idle; 
            end if; 
    end case; 
end process; 





end archi; 