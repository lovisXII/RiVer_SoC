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
        START_DIV       :   in  std_logic;

        DONE_DIV        :   out std_logic;
        BUSY_DIV        :   out std_logic;
        RES_DIV         :   out std_logic_vector(31 downto 0)
    );
end divider; 

architecture archi of divider is 

constant one_ext_6 : std_logic_vector(5 downto 0) := "000001";

type state is (idle, run);
signal EP, EF : state; 

signal op1, op2 : std_logic_vector(31 downto 0);
signal shift_cpt, shift_cpt_reg : std_logic_vector(5 downto 0) := "000000";

signal divisor_se, reminder_se : std_logic_vector(63 downto 0);
signal divisor_reg, reminder_reg : std_logic_vector(63 downto 0);

signal quotient_se, quotient_reg, quotient : std_logic_vector(31 downto 0);

signal quotient_sign_se, reminder_sign_se : std_logic;

signal remind : std_logic_vector(31 downto 0);

signal sign_debug : std_logic;

signal debug_div_remind : std_logic := '0'; 
signal debug_process : integer := 0; 
begin 

-- state transition
fsm_state : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        EP  <=  idle; 
        divisor_reg     <=  x"0000000000000000";
        quotient_reg    <=  x"00000000";
        reminder_reg    <=  x"0000000000000000";
        shift_cpt_reg   <=  "000000";
        
    elsif rising_edge(clk) then 
        EP  <=  EF;
        divisor_reg     <= divisor_se; 
        quotient_reg    <= quotient_se; 
        reminder_reg    <= reminder_se; 
        shift_cpt_reg   <=  shift_cpt; 
    end if; 

    case EP is 
        when idle   =>  EF  <=  idle;
            if START_DIV = '1' and op2 /= x"00000000" then 
                EF  <=  run;   
            end if; 
        when run    =>  EF  <=  run;
            if shift_cpt_reg = "011111" then 
                EF  <=  idle; 
            end if; 
    end case; 
end process; 

fsm_output : process(EP, START_DIV, OP1_SE, OP2_SE, CMD_RD, reminder_reg, divisor_reg, quotient_reg, shift_cpt_reg)
variable signed_inst : std_logic; 
variable reminder_sign : std_logic; 
variable quotient_sign : std_logic; 
variable op1_var, op2_var : std_logic_vector(31 downto 0);

variable div : std_logic_vector(63 downto 0) := x"0000000000000000";


begin 
    case EP is 
        when idle => 
            if START_DIV = '1' then
                if OP2_SE /= x"00000000" then 
                    if CMD_RD = "11" or CMD_RD = "01" then 
                        signed_inst := '1'; 
                    else 
                        signed_inst := '0';
                    end if; 
                    quotient_sign := (OP1_SE(31) xor OP2_SE(31)) and signed_inst; 
                    reminder_sign := OP1_SE(31) and signed_inst; 
                    
                    if OP1_SE(31) = '1' and signed_inst = '1' then 
                        op1_var := std_logic_vector(unsigned(not(OP1_SE)) + unsigned(one_ext_32));
                    else 
                        op1_var := OP1_SE;
                    end if; 

                    if OP2_SE(31) = '1' and signed_inst = '1' then 
                        op2_var := std_logic_vector(unsigned(not(OP2_SE)) + unsigned(one_ext_32));
                    else 
                        op2_var := OP2_SE; 
                    end if; 
    
                    div(62 downto 31) := op2_var; 
                    
                    divisor_se  <= div; 
                    quotient_se <= x"00000000"; 
                    reminder_se <= x"00000000" & op1_var; 
                    BUSY_DIV    <= '1'; 
                    DONE_DIV    <= '0'; 

                    shift_cpt   <= "000000";
                    quotient_sign_se <= quotient_sign; 
                    reminder_sign_se <= reminder_sign; 
                else 
                    quotient_sign_se <= '0'; 
                    reminder_sign_se <= '0'; 
                    reminder_se <= x"00000000" & OP1_SE; 
                    quotient_se <= x"FFFFFFFF"; 
                    DONE_DIV    <= '1'; 
                    BUSY_DIV    <= '1'; 
                    shift_cpt   <= "100000";
                end if; 
            else 
                BUSY_DIV <= '0'; 
                DONE_DIV    <= '1'; 

--                shift_cpt <= "000000";
--                divisor_se     <=  x"0000000000000000";
--                quotient_se    <=  x"00000000";
--                reminder_se    <=  x"ABCDEF0100000000";

            end if; 
        
        when run => 
            if shift_cpt_reg < "100000" then 
                if divisor_reg > reminder_reg then 
                    quotient_se <= quotient_reg(30 downto 0) & '0';
                    debug_div_remind <= '1';  
                    debug_process <= debug_process + 1;

                else 
                    quotient_se <= quotient_reg(30 downto 0) & '1';
                    reminder_se <= std_logic_vector(unsigned(reminder_reg) - unsigned(divisor_reg)); 
                    debug_div_remind <= '0'; 
                end if; 
                shift_cpt <= std_logic_vector(unsigned(shift_cpt_reg) + unsigned(one_ext_6));
                divisor_se  <= '0' & divisor_reg(63 downto 1);
                DONE_DIV <= '0'; 
                BUSY_DIV <= '1'; 

            end if; 
         --  if shift_cpt_reg >= "011111" then 
         --       DONE_DIV <= '1'; 
         --       BUSY_DIV <= '1'; 
         --   end if;

        when others => 
            BUSY_DIV    <= '0'; 
            DONE_DIV    <= '0'; 
            shift_cpt   <= "000000";
            quotient_se <= x"00000000"; 
            reminder_se <= x"ABCDEF0100000000";--x"0000000000000000"; 
            divisor_se  <= x"0000000000000000";
    end case; 
    op1 <= op1_var; 
    op2 <= op2_var;
    sign_debug <= signed_inst;
end process; 


-- Ouput
remind      <=  reminder_reg(31 downto 0) when reminder_sign_se = '0' else 
                std_logic_vector(unsigned(not(reminder_reg(31 downto 0))) + unsigned(one_ext_32));

quotient    <=  quotient_reg    when    quotient_sign_se = '0'  else 
                std_logic_vector(unsigned(not(quotient_reg)) + unsigned(one_ext_32));    

RES_DIV <=  remind  when    (CMD_RD = "11" or CMD_RD = "00")    else 
                quotient; 

end archi; 