library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity tb_reg is 
end tb_reg;

architecture simu of tb_reg is 
signal clk, reset_n : std_logic := '0';
signal PC_INIT, RDATA1_SD, RDATA2_SD, WDATA_SW, WRITE_PC_SD, READ_PC_SR : std_logic_vector(31 downto 0);
signal RADR1_SR, RADR2_SR, WADR_SW : std_logic_vector(5 downto 0);
signal WENABLE_SW, WRITE_PC_ENABLE_SD : std_logic;

begin 

reg_i : entity work.reg 
    port map(
        clk, reset_n,
        PC_INIT, RDATA1_SD, RDATA2_SD, 
        RADR1_SR, RADR2_SR, 
        WDATA_SW, WADR_SW, WENABLE_Sw,
        WRITE_PC_SD, WRITE_PC_ENABLE_SD, 
        READ_PC_SR
    );

reset_n <= '0', '1' after 10 ns;
clk <= not clk after 5 ns;

PC_INIT <= x"000000BC"; 

RADR1_SR <= "000000";
RADR2_SR <= "000011", "000100" after 25 ns;

WDATA_SW <= x"ABCDEF01";
WADR_SW <= "000000", "0000001" after 35 ns, "000100" after 45 ns;
WENABLE_SW <= '0', '1' after 35 ns;

WRITE_PC_SD <= x"000000BF";
WRITE_PC_ENABLE_SD <= '0', '1' after 35 ns, '0' after 45 ns;


end simu;