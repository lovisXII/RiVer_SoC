library ieee;
use ieee.std_logic_1164.all; 
use ieee.numeric_std.all;

entity ram is 
    port(
        clk, reset_n : in std_logic; 
        adr : in std_logic_vector(29 downto 0);
        byt_sel : in std_logic_vector(3 downto 0);
        data_w : in std_logic_vector(31 downto 0);
        data_r : out std_logic_vector(31 downto 0)
    );
end ram; 

architecture archi of ram is 
constant SIZE : integer := (1024);
type tab is array(0 to SIZE-1) of std_logic_vector(7 downto 0);
signal ram0, ram1, ram2, ram3 : tab; 

begin 

process(clk)
begin
    if rising_edge(clk) then 
        if reset_n = '0' then 
            data_r <= x"00000000";
            for i in 0 to SIZE-1 loop 
                ram0(i) <= x"00";
                ram1(i) <= x"00";
                ram2(i) <= x"00";
                ram3(i) <= x"00";
            end loop; 
        else 
        if byt_sel(0) = '1' then 
                ram0(to_integer(unsigned(adr)))     <= data_w(7 downto 0); end if; 
            if byt_sel(1) = '1' then 
                ram1(to_integer(unsigned(adr)+1))   <= data_w(15 downto 8); end if; 
            if byt_sel(2) = '1' then  
                ram2(to_integer(unsigned(adr)+2))   <= data_w(23 downto 16); end if; 
            if byt_sel(3) = '1' then 
                ram3(to_integer(unsigned(adr)+3))   <= data_w(31 downto 24); end if; 
        end if; 
    end if; 
end process; 

data_r <= ram3(to_integer(unsigned(adr)+3)) & ram2(to_integer(unsigned(adr)+2)) & ram1(to_integer(unsigned(adr)+1)) & ram0(to_integer(unsigned(adr)));

end archi; 