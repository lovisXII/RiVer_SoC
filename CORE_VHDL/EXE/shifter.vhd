library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity shifter is 
    port(
        DIN_SE : in std_logic_vector(31 downto 0);
        SHIFT_VAL_SE : in std_logic_vector(4 downto 0);
        CMD_SE : in std_logic_vector(1 downto 0);
        DOUT_SE : out std_logic_vector(31 downto 0)
    );
end shifter;

architecture archi of shifter is 
signal lshift16, lshift8, lshift4, lshift2, lshift1 : std_logic_vector(31 downto 0);
signal rshift16, rshift8, rshift4, rshift2, rshift1 : std_logic_vector(31 downto 0);

signal bits16 : std_logic_vector(15 downto 0);
signal bits8  : std_logic_vector(7 downto 0);
signal bits4  : std_logic_vector(3 downto 0);
signal bits2  : std_logic_vector(1 downto 0);
signal bit1   : std_logic; 

begin 

-- select right shifting bits in case of arthmetic shift
bits16 <= (others => DIN_SE(31)) when CMD_SE(1) = '1' else (others => '0');
bits8  <= (others => DIN_SE(31)) when CMD_SE(1) = '1' else (others => '0');
bits4  <= (others => DIN_SE(31)) when CMD_SE(1) = '1' else (others => '0');
bits2  <= (others => DIN_SE(31)) when CMD_SE(1) = '1' else (others => '0');
bit1   <= DIN_SE(31) when CMD_SE(1) = '1' else '0';

--------------------------------
--  Shift Right
--------------------------------
rshift16 <= (bits16 & DIN_SE(31 downto 16)) when SHIFT_VAL_SE(4) = '1' else
            DIN_SE; 

rshift8  <= (bits8 & rshift16(31 downto 8)) when SHIFT_VAL_SE(3) = '1' else
            rshift16;

rshift4  <= (bits4 & rshift8(31 downto 4)) when SHIFT_VAL_SE(2) = '1' else
            rshift8;

rshift2  <= (bits2 & rshift4(31 downto 2)) when SHIFT_VAL_SE(1) = '1' else
            rshift4;

rshift1  <= (bit1 & rshift2(31 downto 1)) when SHIFT_VAL_SE(0) = '1' else
            rshift2;


--------------------------------
--  Shift Left
--------------------------------
lshift16 <= (DIN_SE(15 downto 0) & x"0000") when SHIFT_VAL_SE(4) = '1' else 
            DIN_SE;

lshift8 <= (lshift16(23 downto 0) & x"00") when SHIFT_VAL_SE(3) = '1' else 
            lshift16;

lshift4 <= (lshift8(27 downto 0) & x"0") when SHIFT_VAL_SE(2) = '1' else 
            lshift8;

lshift2 <= (lshift4(29 downto 0) & "00") when SHIFT_VAL_SE(1) = '1' else 
            lshift4;

lshift1 <= (lshift2(30 downto 0) & '0') when SHIFT_VAL_SE(0) = '1' else 
            lshift2;

-- Ouput selection
DOUT_SE <= lshift1 when CMD_SE = "00" else rshift1;

end archi;

