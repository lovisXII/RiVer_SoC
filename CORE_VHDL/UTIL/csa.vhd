library ieee; 
use ieee.std_logic_1164.all;

entity csa is 
    port(
        A   :   in  std_logic_vector(63 downto 0);
        B   :   in  std_logic_vector(63 downto 0);
        C   :   in  std_logic_vector(63 downto 0);
        S0  :   out std_logic_vector(63 downto 0);
        S1  :   out std_logic_vector(63 downto 0);
    );
end csa; 

architecture archi of csa is 

signal AxB  : std_logic_vector(63 downto 0);
signal shf  : std_logic_vector(63 downto 0);

begin 

AxB <=  A xor B; 
shf <=  (A and B) or (AxB and C);

S0  <=  AxB xor C;

S1(63 downto 1) <=  shf(63 downto 1);
S1(0)           <=  '0';  

end archi; 