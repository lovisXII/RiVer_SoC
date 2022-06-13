library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity tb_shifter is 
end tb_shifter;

architecture simu of tb_shifter is 
signal DIN_SE, DOUT_SE : std_logic_vector(31 downto 0);
signal SHIFT_VAL_SE : std_logic_vector(4 downto 0);
signal CMD_SE : std_logic_vector(1 downto 0);

begin 

shifter_i : entity work.shifter 
    port map(DIN_SE, SHIFT_VAL_SE, CMD_SE, DOUT_SE);

DIN_SE <= x"0000F000", x"8000F000" after 30 ns;
SHIFT_VAL_SE <= "00100";
CMD_SE <= "00", "01" after 10 ns, "10" after 20 ns, "11" after 30 ns;

end simu;