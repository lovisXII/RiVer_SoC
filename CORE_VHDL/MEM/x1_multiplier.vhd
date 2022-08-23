library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all;

entity x1_multiplier is 
    port(
        -- global interface
        clk, reset_n    :   in  std_logic;

        RES_RX0         :   in  std_logic_vector(319 downto 0);
        SELECT_MSB_RX0  :   in  std_logic;
        SIGNED_RES_RX0  :   in  std_logic;
        X0X1_EMPTY_SX0  :   in  std_logic;
        X1X2_POP_SX2    :   in  std_logic;

        RES_RX1         :   out std_logic_vector(127 downto 0);
        SELECT_MSB_RX1  :   out std_logic;
        SIGNED_RES_RX1  :   out std_logic;
        X1X2_EMPTY_SX1  :   out std_logic;
        X0X1_POP_SX1    :   out std_logic
    );
end x1_multiplier;

architecture archi of x1_multiplier is

signal x0_data      :   tab_5_t; 

signal product_s6   :   tab_2_t;
signal product_s7   :   tab_2_t;
signal product_s8   :   tab_2_t;

signal stall_sx1     :   std_logic;

signal x1x2_din, x1x2_dout  :   std_logic_vector(129 downto 0);
signal x1x2_push, x1x2_full :   std_logic;

component fifo
    generic(N : integer);
    port(
        clk     : in    std_logic; 
        reset_n : in    std_logic; 
        DIN     : in    std_logic_vector(N-1 downto 0);
        PUSH    : in    std_logic;
        POP     : in    std_logic;
        FULL    : out   std_logic;
        EMPTY   : out   std_logic;
        DOUT    : out   std_logic_vector(N-1 downto 0)
    );
end component;

component csa 
    port(
        A   :   in  std_logic_vector(63 downto 0);
        B   :   in  std_logic_vector(63 downto 0);
        C   :   in  std_logic_vector(63 downto 0);
        S0  :   out std_logic_vector(63 downto 0);
        S1  :   out std_logic_vector(63 downto 0)
    );
end component;


begin 

x1x2 : fifo 
    generic map(N => 130)
    port map(
        clk     => clk, 
        reset_n => reset_n,
        DIN     => x1x2_din, 
        PUSH    => x1x2_push, 
        POP     => x1X2_POP_SX2, 
        FULL    => x1x2_full, 
        EMPTY   => X1X2_EMPTY_SX1, 
        DOUT    => x1x2_dout
    );


x0_data(4)  <=  RES_RX0(319 downto 256);
x0_data(3)  <=  RES_RX0(255 downto 192);
x0_data(2)  <=  RES_RX0(191 downto 128);
x0_data(1)  <=  RES_RX0(127 downto 64);
x0_data(0)  <=  RES_RX0(63 downto 0);

csa6 : csa
    port map(
        A   => x0_data(0),
        B   => x0_data(1),
        C   => x0_data(2),
        S0  => product_s6(0),
        S1  => product_s6(1)        
    );

csa7 : csa 
    port map(
        A   => product_s6(0),
        B   => product_s6(1),
        C   => x0_data(3),
        S0  => product_s7(0),
        S1  => product_s7(1)       
    );

csa8 : csa 
    port map(
        A   => product_s7(0),
        B   => product_s7(1),
        C   => x0_data(4),
        S0  => product_s8(0),
        S1  => product_s8(1)  
    );

stall_sx1       <=  x1x2_full or X0X1_EMPTY_SX0; 
x1x2_push       <=  not stall_sx1; 
x0x1_POP_SX1    <=  not stall_sx1; 

-- fifo x1x2 input
x1x2_din(129)           <=  SIGNED_RES_RX0;
x1x2_din(128)           <=  SELECT_MSB_RX0; 
x1x2_din(127 downto 64) <=  product_s8(1);
x1x2_din(63  downto 0)  <=  product_s8(0);

-- fifo x1x2 ouput
SIGNED_RES_RX1          <=  x1x2_dout(129);
SELECT_MSB_RX1          <=  x1x2_dout(128);
RES_RX1                 <=  x1x2_dout(127 downto 0);

end archi; 