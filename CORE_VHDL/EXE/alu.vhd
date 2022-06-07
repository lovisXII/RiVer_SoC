library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity alu is 
    port(
        OP1_SE, OP2_SE : in std_logic_vector(31 downto 0);
        CIN_SE : in std_logic;
        CMD_SE : in std_logic_vector(1 downto 0);
        RES_SE : out std_logic_vector(31 downto 0)
    );
end alu;

architecture archi of alu is
begin 

process(CMD_SE, OP1_SE, OP2_SE, CIN_SE)
begin 
    case CMD_SE is
        when "00" => RES_SE <= std_logic_vector(signed(OP1_SE) + signed(OP2_SE) + signed("0000000000000000000000000000000" & CIN_SE));
        when "01" => RES_SE <= OP1_SE and OP2_SE;
        when "10" => RES_SE <= OP1_SE or OP2_SE;
        when "11" => RES_SE <= OP1_SE xor OP2_SE;
        when others => RES_SE <= (others => '0');
    end case; 
end process; 

end archi;