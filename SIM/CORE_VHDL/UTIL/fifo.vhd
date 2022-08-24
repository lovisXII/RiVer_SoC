library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity fifo is 
    generic(N : integer);
    port(
        clk, reset_n : in std_logic;
        DIN : in std_logic_vector(N-1 downto 0);
        PUSH, POP : in std_logic;
        FULL, EMPTY : out std_logic;
        DOUT : out std_logic_vector(N-1 downto 0)
    );
end fifo;

architecture archi of fifo is 
signal data : std_logic_vector(N-1 downto 0);
signal data_v : std_logic := '0';

begin 

process(clk)
begin 
    if(rising_edge(clk)) then 
        if reset_n = '0' then
            data_v <= '0';
        else 
            if data_v <= '0' then 
                if PUSH = '1' then 
                    data_v <= '1'; 
                else 
                    data_v <= '0';
                end if;
            else 
                if POP = '1' then 
                    if PUSH = '1' then 
                        data_v <= '1';
                    else 
                        data_v <= '0';
                    end if;
                else 
                    data_v <= '1'; 
                end if;
            end if;
        end if;

        if data_v <= '0' then 
            if PUSH = '1' then 
                data <= DIN;
            end if;
        elsif PUSH = '1' and POP = '1' then 
            data <= DIN;
        end if;
    end if;    
end process;

FULL <= '1' when data_v = '1' and POP = '0' else '0';
EMPTY <= not data_v;
DOUT <= data;

end archi;