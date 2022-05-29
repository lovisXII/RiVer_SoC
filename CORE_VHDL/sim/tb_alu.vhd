library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_alu is 
end tb_alu;

architecture simu of tb_alu is 
signal OP1_SE, OP2_SE, RES_SE : std_logic_vector(31 downto 0);
signal CIN_SE : std_logic := '0';
signal CMD_SE : std_logic_vector(1 downto 0); 

begin 

alu_i : entity work.alu
    port map(OP1_SE, OP2_SE, CIN_SE, CMD_SE, RES_SE);

OP1_SE <= x"0F0F0F0F", x"00FFFF00" after 10 ns, x"0F0F0F0F" after 30 ns; 
OP2_SE <= x"F0F0F0F0", x"0FF00FFF" after 10 ns, x"F0F0F0F0" after 30 ns; 
CIN_SE <= not CIN_SE after 5 ns; 
CMD_SE <= "00", "01" after 10 ns, "10" after 20 ns, "11" after 30 ns; 

end simu;