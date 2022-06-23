library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity tb_ifetch is 
end tb_ifetch;

architecture simu of tb_ifetch is 

signal clk, reset_n : std_logic := '0';
signal IC_INST_SI, ADR_SI : std_logic_vector(31 downto 0);
signal IC_STALL_SI, ADR_VALID_SI : std_logic; 

signal DEC2IF_EMPTY_SI, DEC2IF_POP_SI : std_logic; 

signal IF2DEC_FLUSH_SI, IF2DEC_POP_SI, IF2DEC_EMPTY_SI_S1 : std_logic;

signal PC_SI, INSTR_SI : std_logic_vector(31 downto 0);

begin 

ifetch_i : entity work.ifetch
    port map(clk, reset_n, IC_INST_SI, IC_STALL_SI, ADR_SI, ADR_VALID_SI,
            DEC2IF_EMPTY_SI, DEC2IF_POP_SI, IF2DEC_FLUSH_SI, 
            IF2DEC_POP_SI, IF2DEC_EMPTY_SI_S1, PC_SI, INSTR_SI
            );


clk <= not clk after 5 ns;
reset_n <= '1', '0' after 10 ns;

IC_INST_SI <= x"0000FFFF" ;
IC_STALL_SI <= '0';

DEC2IF_EMPTY_SI <= '0';

IF2DEC_FLUSH_SI <= '0'; 

IF2DEC_POP_SI <= '1'; 

PC_SI <= x"0000AAAA";

end simu;