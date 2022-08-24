library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all;

entity x2_multiplier is 
    port(
        -- global interface
        clk, reset_n    :   in  std_logic;

        RES_RX1         :   in  std_logic_vector(127 downto 0);
        SELECT_MSB_RX1  :   in  std_logic;
        SIGNED_RES_RX1  :   in  std_logic;
        X1X2_EMPTY_SX1  :   in  std_logic;

        RES_RX2         :   out std_logic_vector(31 downto 0);
        X1X2_POP_SX2    :   out std_logic
    );
end x2_multiplier;

architecture archi of x2_multiplier is 

signal  res     :   std_logic_vector(63 downto 0);

begin 

res <=  std_logic_vector(unsigned(RES_RX1(63 downto 0)) + unsigned(RES_RX1(127 downto 64)));

RES_RX2 <=  res(63 downto 32)   when SELECT_MSB_RX1 = '1' else 
            res(31 downto 0);

X1X2_POP_SX2 <= not X1X2_EMPTY_SX1;

end archi;