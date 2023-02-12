library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_buff_cache is 
end entity;

architecture behavior of tb_buff_cache is

component buffer_cache is
    generic (
        depth : integer := 2
    );
    port (
        -- global interface
        clk, reset_n : in std_logic;

        -- fifo commands & status
        PUSH, POP : in std_logic;
        EMPTY, FULL : out std_logic;

        -- fifo input data
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
end component;

signal clk : std_logic := '0';
signal reset_n : std_logic := '0';

signal push : std_logic;
signal pop : std_logic;
signal empty : std_logic;
signal full : std_logic;

signal data_c : std_logic_vector(31 downto 0);
signal adr_c : std_logic_vector(31 downto 0);
signal store_c : std_logic;
signal load_c : std_logic;
signal size_c : std_logic_vector(1 downto 0);

signal data_bc : std_logic_vector(31 downto 0);
signal adr_bc : std_logic_vector(31 downto 0);
signal store_bc : std_logic;
signal load_bc : std_logic;
signal size_bc : std_logic_vector(1 downto 0);

begin
    buff_cache_inst : buffer_cache 
    generic map (
        depth => 2
    )
    port map (
        clk, reset_n,
        push, pop, empty, full,
        data_c, adr_c, store_c, load_c, size_c,
        data_bc, adr_bc, store_bc, load_bc, size_bc
    );

    clk <= not clk after 5 ns;

    tb: process is begin 
        wait until clk = '1';
        reset_n <= '1';

        wait until clk = '1';
        data_c <= x"deadbeef";
        adr_c <= x"00001000";
        store_c <= '1';
        load_c <= '0';
        size_c <= "11";
        push <= '1';

        wait until clk = '1';
        push <= '0';

        wait until clk = '1';
        data_c <= x"cafebabe";
        adr_c <= x"00001004";
        store_c <= '0';
        load_c <= '1';
        size_c <= "11";
        push <= '1';

        wait until clk = '1';
        push <= '0';
        pop <= '1';

        wait until clk = '1';
        wait until clk = '1';
        report "end" severity failure;
    end process;
end architecture;
