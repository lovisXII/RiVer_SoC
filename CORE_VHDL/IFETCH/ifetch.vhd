library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ifetch is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Icache interface
        IC_INST_SI : in std_logic_vector(31 downto 0);
        IC_STALL_SI : in std_logic;
        ADR_SI : out std_logic_vector(31 downto 0);
        ADR_VALID_SI : out std_logic;

        -- dec2if interface 
        DEC2IF_EMPTY_SD : in std_logic;
        DEC2IF_POP_SI : out std_logic;

        -- if2dec interface 
        IF2DEC_FLUSH_SD : in std_logic;
        IF2DEC_POP_SD : in std_logic; 
        IF2DEC_EMPTY_SI : out std_logic; 

        PC_RD : in std_logic_vector(31 downto 0);
        INSTR_RI : out std_logic_vector(31 downto 0);
        PC_IF2DEC_RI : out std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_SM : in std_logic; 
        EXCEPTION_RI : out std_logic

        -- Interrupts 
        --INTERRUPTION_SE : in std_logic; 
        --CURRENT_MODE_SM : in std_logic_vector(1 downto 0);
        --MRET_SM : in std_logic;
        --RETURN_ADRESS_SM : in std_logic_vector(31 downto 0)
    );
end ifetch;

architecture archi of ifetch is 
signal if2dec_push_si, if2dec_full_si : std_logic;
signal stall_si : std_logic;

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

constant nop_i : std_logic_vector(31 downto 0) := x"00000013"; 
signal if2dec_din : std_logic_vector(63 downto 0);

begin 

-- Instanciation 
if2dec : fifo
    generic map(N => 64)
    port map(
        clk                 => clk,
        reset_n             => reset_n,
        DIN                 => if2dec_din, 
        PUSH                => if2dec_push_si, 
        POP                 => IF2DEC_POP_SD,
        FULL                => if2dec_full_si, 
        EMPTY               => IF2DEC_EMPTY_SI, 
        DOUT(63 downto 32)  => INSTR_RI, 
        DOUT(31 downto 0)   => PC_IF2DEC_RI
    );

stall_si <= IC_STALL_SI or if2dec_full_si or DEC2IF_EMPTY_SD;

if2dec_push_si <= not stall_si when IF2DEC_FLUSH_SD = '0' else '0'; 
DEC2IF_POP_SI <= not stall_si when IF2DEC_FLUSH_SD = '0' else '1';
ADR_VALID_SI <= not DEC2IF_EMPTY_SD when (IF2DEC_FLUSH_SD = '0' and EXCEPTION_SM = '0') else '0';

--if2dec_push_si  <=  '1' when EXCEPTION_SM = '1'     else 
--                    '0' when IF2DEC_FLUSH_SD = '1'  else 
--                    not(stall_si);
--DEC2IF_POP_SI   <=  '1' when (IF2DEC_FLUSH_SD = '1' or EXCEPTION_SM = '1') else not(stall_si);
--ADR_VALID_SI    <=  '0' when (IF2DEC_FLUSH_SD = '1' or EXCEPTION_SM = '1') else not(DEC2IF_EMPTY_SD);

ADR_SI <= PC_RD;

if2dec_din(63 downto 32)    <=  nop_i when EXCEPTION_SM = '1' else 
                                IC_INST_SI;
if2dec_din(31 downto 0)     <=  PC_RD; 

EXCEPTION_RI <= '0'; 


end archi;