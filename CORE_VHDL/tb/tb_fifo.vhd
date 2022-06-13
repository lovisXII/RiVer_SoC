library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity tb_fifo is 
end tb_fifo;

architecture simu of tb_fifo is 
signal clk, reset_n : std_logic := '0';
signal din, dout : std_logic_vector(31 downto 0);
signal push, pop, full, empty : std_logic;

begin 

fifo_i : entity worK.fifo
    generic map(N => 32)
    port map(clk, reset_n, din, push, pop, full, empty, dout);

clk <= not clk after 5 ns;
reset_n <= '0', '1' after 10 ns;    
din <= x"0000FFFF", x"FFFF0000" after 30 ns, x"F0F0F0F0" after 60 ns, x"AAAAAAAA" after 80 ns;
push <= '0', '1' after 10 ns, '0' after 15 ns, '1' after 20 ns;
pop <= '0', '1' after 15 ns, '0' after 20 ns, '1' after 30 ns, '0' after 80 ns, '1' after 95 ns;   

end simu;