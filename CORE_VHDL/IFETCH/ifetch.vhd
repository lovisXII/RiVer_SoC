library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
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
        DEC2IF_EMPTY_SI : in std_logic;
        DEC2IF_POP_SI : out std_logic;

        -- if2dec interface 
        IF2DEC_FLUSH_SI : in std_logic;
        IF2DEC_POP_SI : in std_logic; 
        IF2DEC_EMPTY_SI : out std_logic; 

        PC_SI : in std_logic_vector(31 downto 0);
        INSTR_SI : out std_logic_vector(31 downto 0)
    );
end ifetch;

architecture archi of ifetch is 
signal if2dec_push, if2dec_full : std_logic;
signal stall : std_logic;

begin 

-- Instanciation 
if2dec : entity work.fifo
    generic map(N => 32)
    port map(
        clk => clk,
        reset_n => reset_n,
        DIN => IC_INST_SI,
        PUSH => if2dec_push, 
        POP => IF2DEC_POP_SI,
        FULL => if2dec_full, 
        EMPTY => IF2DEC_EMPTY_SI, 
        DOUT => INSTR_SI
    );

stall <= IC_STALL_SI or if2dec_full or DEC2IF_EMPTY_SI;

if2dec_push <= not stall when IF2DEC_FLUSH_SI = '0' else '0'; 
DEC2IF_POP_SI <= not stall when IF2DEC_FLUSH_SI = '0' else '1';
ADR_VALID_SI <= not DEC2IF_EMPTY_SI when IF2DEC_FLUSH_SI = '0' else '0';

ADR_SI <= PC_SI;

end archi;