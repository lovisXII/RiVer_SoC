library ieee;
use ieee.std_logic_1164.all;

entity buffer_cache is
    generic (
        depth : integer := 2
    );
    port (
        -- global interface
        clk, reset_n : in std_logic;

        -- fifo commands & status
        PUSH, POP : in std_logic;
        EMPTY, FULL : out std_logic;

        -- fifo input data      TODO : remove LOAD and change all indexes
        DATA_C : in std_logic_vector(31 downto 0);
        ADR_C : in std_logic_vector(31 downto 0);
        STORE_C, LOAD_C : in std_logic; -- might be simplified to a single signal
        SIZE_C : in std_logic_vector(1 downto 0); -- 0b10 for byte  
                                                  -- 0b01 for short
                                                  -- 0b00 for word

        -- output
        DATA_BC : out std_logic_vector(31 downto 0);
        ADR_BC : out std_logic_vector(31 downto 0);
        STORE_BC, LOAD_BC : out std_logic; 
        SIZE_BC : out std_logic_vector(1 downto 0)
    );
end buffer_cache;

architecture archi of buffer_cache is

-- buffer cache is simply a FIFO of with 68 bits
type t_fifo_data is array (0 to depth-1) of std_logic_vector(67 downto 0);
signal data : t_fifo_data;

signal wr_index : integer range 0 to depth-1;
signal rd_index : integer range 0 to depth-1;
signal count : integer range 0 to depth;

signal f_full : std_logic;
signal f_empty : std_logic;

begin
    process (clk) 
    begin
        if(rising_edge(clk)) then 
            if reset_n = '0' then
                wr_index <= 0;
                rd_index <= 0;
                count <= 0;
            else 
                if (PUSH='1' and f_full='0') then
                    count <= count + 1;
                elsif (POP='1' and f_empty='0') then
                    count <= count - 1;
                end if;

                if (PUSH='1' and f_full='0') then
                    if wr_index = depth-1 then
                        wr_index <= 0;
                    else
                        wr_index <= wr_index + 1;
                    end if;
                end if;

                if (POP='1' and f_empty='0') then
                    if rd_index = depth-1 then
                        rd_index <= 0;
                    else
                        rd_index <= rd_index + 1;
                    end if;
                end if;

                if (PUSH='1' and f_full='0') then
                    data(wr_index)(67 downto 36) <= DATA_C;
                    data(wr_index)(35 downto 4) <= ADR_C;
                    data(wr_index)(3) <= STORE_C;
                    data(wr_index)(2) <= LOAD_C;
                    data(wr_index)(1 downto 0) <= SIZE_C;
                end if;
            end if;
        end if;
    end process;

    DATA_BC     <= data(rd_index)(67 downto 36);
    ADR_BC      <= data(rd_index)(35 downto 4);
    STORE_BC    <= data(rd_index)(3);
    LOAD_BC     <= data(rd_index)(2);
    SIZE_BC     <= data(rd_index)(1 downto 0);

    f_full  <= '1' when count = depth else '0';
    f_empty <= '1' when count = 0 else '0';

    FULL    <= f_full;
    EMPTY   <= f_empty;
end archi;