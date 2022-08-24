library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_divider is 
end entity; 

architecture simu of tb_divider is 

signal clk, reset_n : std_logic := '0'; 
signal op1, op2 : std_logic_vector(31 downto 0);

signal cmd : std_logic_vector(1 downto 0);

signal START_DIV : std_logic;
signal DONE_DIV : std_logic;
signal BUSY_DIV : std_logic;
signal RES_DIV : std_logic_vector(31 downto 0);

component divider 
    port(
        -- global interface
        clk, reset_n    :   in  std_logic;

        OP1_SE, OP2_SE  :   in  std_logic_vector(31 downto 0);
        CMD_RD          :   in  std_logic_vector(1 downto 0);
        START_DIV       :   in  std_logic;

        DONE_DIV        :   out std_logic;
        BUSY_DIV        :   out std_logic;
        RES_DIV     :   out std_logic_vector(31 downto 0)
    );
end component; 


begin 


div : divider 
    port map(
        clk         => clk,
        reset_n     => reset_n,

        OP1_SE      => op1, 
        OP2_SE      => op2,
        CMD_RD      => cmd,

        START_DIV   => START_DIV,

        DONE_DIV    => DONE_DIV,
        BUSY_DIV    => BUSY_DIV,
        RES_DIV     => RES_DIV
    );


clk <= not clk after 5 ns;
reset_n <= '0', '1' after 6 ns; 


START_DIV   <= '0', '1' after 45 ns, '0' after 55 ns; 
cmd      <= "10";

op1         <=  x"0000000A";
op2         <=  x"00000002";


end simu; 