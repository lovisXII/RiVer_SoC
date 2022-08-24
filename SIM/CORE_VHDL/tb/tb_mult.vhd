library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_mult is
end tb_mult; 

architecture simu of tb_mult is 

signal clk, reset_n : std_logic := '0';

signal op1, op2 : std_logic_vector(31 downto 0);

signal x0x1_pop, x1x2_pop : std_logic;
signal dec2x0_empty, x0x1_empty, x1x2_empty : std_logic;

signal RES_RX0  : std_logic_vector(319 downto 0);
signal RES_RX1  : std_logic_vector(127 downto 0);
signal RES_RX2  : std_logic_vector(31 downto 0);

signal cmd : std_logic_vector(1 downto 0);

signal SELECT_MSB_RX0, SELECT_MSB_RX1 : std_logic;
signal SIGNED_RES_RX0, SIGNED_RES_RX1 : std_logic;

component x0_multiplier 
    port(
        -- global inteface 
        clk, reset_n    :   in  std_logic;
        OP1_SE, OP2_SE  :   in  std_logic_vector(31 downto 0);
        MULT_CMD_RD     :   in  std_logic_vector(1 downto 0);
        X0X1_POP_SX1    :   in  std_logic; 
        DEC2X0_EMPTY_SD :   in  std_logic;
        RES_RX0         :   out std_logic_vector(319 downto 0);
        SELECT_MSB_RX0  :   out std_logic;
        SIGNED_RES_RX0  :   out std_logic;
        X0X1_EMPTY_SX0  :   out std_logic
    );
end component;

component x1_multiplier 
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
        SIGNED_RES_RX1  :   out  std_logic;
        X1X2_EMPTY_SX1  :   out std_logic;
        X0X1_POP_SX1    :   out std_logic
    );
end component; 

component x2_multiplier 
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
end component; 

begin 

clk <= not clk after 5 ns; 
reset_n <= '0', '1' after 6 ns; 

x0 : x0_multiplier 
    port map(
        clk             =>  clk, 
        reset_n         =>  reset_n,
        OP1_SE          =>  op1, 
        OP2_SE          =>  op2,
        MULT_CMD_RD     =>  cmd,
        X0X1_POP_SX1    =>  x0x1_pop,
        DEC2X0_EMPTY_SD =>  dec2x0_empty,
        RES_RX0         =>  RES_RX0,
        SELECT_MSB_RX0  =>  SELECT_MSB_RX0,
        SIGNED_RES_RX0  =>  SIGNED_RES_RX0,
        X0X1_EMPTY_SX0  =>  x0x1_empty
    );

x1 : x1_multiplier
    port map(
        clk             =>  clk,
        reset_n         =>  reset_n,
        RES_RX0         =>  RES_RX0,
        SELECT_MSB_RX0  =>  SELECT_MSB_RX0,
        SIGNED_RES_RX0  =>  SIGNED_RES_RX0,
        X0X1_EMPTY_SX0  =>  x0x1_empty,
        X1X2_POP_SX2    =>  x1x2_pop, 
        RES_RX1         =>  RES_RX1,
        SELECT_MSB_RX1  =>  SELECT_MSB_RX1,
        SIGNED_RES_RX1  =>  SIGNED_RES_RX1, 
        X1X2_EMPTY_SX1  =>  x1x2_empty,
        X0X1_POP_SX1    =>  x0x1_pop
    );

x2 : x2_multiplier 
    port map(
        clk             =>  clk,
        reset_n         =>  reset_n,
        RES_RX1         =>  RES_RX1,
        SELECT_MSB_RX1  =>  SELECT_MSB_RX1,
        SIGNED_RES_RX1  =>  SIGNED_RES_RX1,
        X1X2_EMPTY_SX1  =>  x1x2_empty,
        RES_RX2         =>  RES_RX2,
        X1X2_POP_SX2    =>  x1x2_pop
    );


op1 <= x"00000003"; 
op2 <= x"00000004"; 

cmd <= "01";
dec2x0_empty <= '0'; 
end simu; 