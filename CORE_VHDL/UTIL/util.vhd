library ieee; 
use ieee.std_logic_1164.all;

package util is 
    constant kernel_adr : std_logic_vector(31 downto 0) := x"F0000000"; 
    constant one_ext_32 : std_logic_vector(31 downto 0) := x"00000001"; 

    type tab_32_t   is array(0 to 31) of std_logic_vector(63 downto 0);
    type tab_20_t   is array(0 to 19) of std_logic_vector(63 downto 0);
    type tab_14_t   is array(0 to 13) of std_logic_vector(63 downto 0);
    type tab_10_t   is array(0 to 9)  of std_logic_vector(63 downto 0);
    type tab_6_t    is array(0 to 5)  of std_logic_vector(63 downto 0);
    type tab_5_t    is array(0 to 5)  of std_logic_vector(63 downto 0);
    type tab_4_t    is array(0 to 3)  of std_logic_vector(63 downto 0);
    type tab_2_t    is array(0 to 1)  of std_logic_vector(63 downto 0);

end util; 

package body util is 
    
end util; 