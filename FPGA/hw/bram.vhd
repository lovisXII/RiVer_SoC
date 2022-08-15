library ieee;

use ieee.std_logic_1164.all;

use ieee.numeric_std.all;

 

entity bram is
generic(
    SIZE    :   integer := 2048;
    LENGTH  :   integer := 32
);
port(

 clk  : in  std_logic;

 we   : in  std_logic;

 en   : in  std_logic;

 addr : in  std_logic_vector(29 downto 0);

 di   : in  std_logic_vector((LENGTH-1) downto 0);

 do   : out std_logic_vector((LENGTH-1) downto 0)

);

end bram;

 

architecture syn of bram is

type ram_type is array ((SIZE-1) downto 0) of std_logic_vector((LENGTH-1) downto 0);

signal RAM : ram_type;

begin

process(clk)

begin

 if clk'event and clk = '1' then

   if en = '1' then

    if we = '1' then

    RAM(to_integer(unsigned(addr))) <= di;

   else

    do <= RAM(to_integer(unsigned(addr)));

   end if;

  end if;

 end if;

end process;

 

end syn;