library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all;

entity x0_multiplier is 
    port map(
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
end x0_multiplier; 

architecture archi of x0_multiplier is 

type product_32_t is array(0 to 31) of std_logic_vector(63 downto 0);
type product_s1_t is array(0 to 19) of std_logic_vector(63 downto 0);
type product_s2_t is array(0 to 13) of std_logic_vector(63 downto 0);
type product_s3_t is array(0 to 9)  of std_logic_vector(63 downto 0);
type product_s4_t is array(0 to 5)  of std_logic_vector(63 downto 0);
type product_s5_t is array(0 to 3)  of std_logic_vector(63 downto 0);


signal product      : product_32_t; 
signal product_s1   : product_s1_t;
signal product_s2   : product_s2_t;
signal product_s3   : product_s3_t;
signal product_s4   : product_s4_t;
signal product_s5   : product_s5_t;

signal op1, op2     : std_logic_vector(31 downto 0);

signal signed_type      : std_logic;
signal signed_res_sx0   : std_logic;
signal select_msb_sx0   : std_logic;

signal stall_sx0        : std_logic;

signal x0x1_din, x0x1_dout  : std_logic_vector(321 downto 0);
signal x0x1_push, x0x1_full : std_logic;

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

x0x1 : fifo 
    generic map(N => 322)
    port map(
        clk     => clk, 
        reset_n => reset_n,
        DIN     => x0x1_din, 
        PUSH    => x0x1_push, 
        POP     => X0X1_POP_SX1, 
        FULL    => x0x1_full, 
        EMPTY   => X0X1_EMPTY_SX0, 
        DOUT    => x0x1_dout
    );


signed_type     <=  '1' when (MULT_CMD_RD /= "11") and ((OP1_SE(31) and OP2_SE(31)) = '0')  else '0';
signed_res_sx0  <=  '0' when (OP1_SE(31) and OP2_SE(31) = '1') else '1'; -- else  ?? 
select_msb_sx0  <=  '1' when MULT_CMD_RD /= "01" else '0'; 

op1             <=  std_logic_vector(unsigned(not(OP1_SE)) + unsigned(one_ext_32)) when (OP1_SE(31) = '1' and OP2_SE(31) = '1') else 
                    OP2_SE when OP1_SE = '0' and OP2_SE = '1' else 
                    OP1_SE; 

op2             <=  std_logic_vector(unsigned(not(OP2_SE)) + unsigned(one_ext_32)) when (OP1_SE(31) = '1' and OP2_SE(31) = '1') else 
                    OP1_SE when OP1_SE = '0' and OP2_SE = '1' else 
                    OP2_SE; 

partial_product : process(op1, op2, signed_type)
variable prod : std_logic_vector(63 downto 0);
begin 
    l0 : for i in 0 to 31 loop 
        if op2[i] /= '0' then 
            l1 : for j in 0 to i+31 loop 
                prod := op1(j-i) and op2(i);
            end loop; 
            if signed_type = '1' and op(31) = '1' then 
                l2 : for k in i+31 to 63 loop
                    prod(j) := '1'; 
                end loop; 
            end if; 
        end if; 
        product(i) <= prod; 
    end loop; 
end process;

---------------------------
-- CSA instanciation 
---------------------------
stage1 : for i in 0 to 9 generate 
begin
    csa1 : csa 
        port map(
            A   =>  product(0 + 3*i),
            B   =>  product(1 + 3*i),
            C   =>  product(2 + 3*i),
            S0  =>  product_s1(0 + 2*i), 
            S1  =>  product_s1(1 + 2*i)
        );
end generate; 

stage2 : for i in 0 to 5 generate 
begin
    csa2 : csa 
        port map(
            A   => product_s1(0 + 3*i),
            B   => product_s1(1 + 3*i),
            C   => product_s1(2 + 3*i),
            S0  => product_s2(0 + 2*i),
            S1  => product_s2(1 + 2*i)        
        );
end generate; 

csa2_6 : csa 
    port map(
        A   => product_s1(18),
        B   => product_s1(19),
        C   => product(30),
        S0  => product_s2(12),
        S1  => product_s2(13)        
    );

stage3 : for i in 0 to 3 generate
begin
    csa3 : csa 
    port map(
        A   => product_s2(0 + 3*i),
        B   => product_s2(1 + 3*i),
        C   => product_s2(2 + 3*i),
        S0  => product_s3(0 + 2*i),
        S1  => product_s3(1 + 2*i)        
    );
end generate; 

csa3_4 : csa 
    port map(
        A   => product_s2(18),
        B   => product_s2(19),
        C   => product(31),
        S0  => product_s3(12),
        S1  => product_s3(13)        
    );

stage4 : for i in 0 to 2 generate
begin
    csa4: csa 
    port map(
        A   => product_s3(0 + 3*i),
        B   => product_s3(1 + 3*i),
        C   => product_s3(2 + 3*i),
        S0  => product_s4(0 + 2*i),
        S1  => product_s4(1 + 2*i)        
    );
end generate; 

stage5 : for i in 0 to 1 generate
begin
    csa5: csa 
    port map(
        A   => product_s4(0 + 3*i),
        B   => product_s4(1 + 3*i),
        C   => product_s4(2 + 3*i),
        S0  => product_s5(0 + 2*i),
        S1  => product_s5(1 + 2*i)        
    );
end generate; 


stall_sx0       <=  x0x1_full_sx0 or DEC2X0_EMPTY_SD; 
x0x1_push_sx0   <=  not stall_sx0; 

-- fifo x0x1 input 
x0x1_din(321)               <=  select_msb_sx0; 
x0x1_din(320)               <=  signed_res_sx0; 
x0x1_din(319 downto 256)    <=  product_s3(9); 
x0x1_din(255 downto 192)    <=  product_s5(3);
x0x1_din(191 downto 128)    <=  product_s5(2);
x0x1_din(127 downto 64)     <=  product_s5(1);
x0x1_din(63  downto 0)      <=  product_s5(0);

-- fifo x0x1 input 
SELECT_MSB_RX0              <=  x0x1_dout(321);
SIGNED_RES_RX0              <=  x0x1_dout(320);
RES_RX0                     <=  x0x1_dout(319 downto 0);

end archi;
