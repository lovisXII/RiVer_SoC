library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity wbk is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Mem 
        MEM_DATA_SW : in std_logic_vector(31 downto 0);
        MEM_DEST_SW : in std_logic_vector(5 downto 0);
        MEM_SIZE_SW : in std_logic_vector(1 downto 0);
        MEM_WB_SW : in std_logic;
        SIGN_EXTEND_SW : in std_logic;
        LOAD_SW : in std_logic;

        -- mem2wbk interface
        MEM2WBK_EMPTY_SW : in std_logic;
        MEM2WBK_POP_SW : out std_logic;

        -- Reg interface
        REG_DATA_SW : out std_logic_vector(31 downto 0);
        REG_DEST_SW : out std_logic_vector(5 downto 0);
        REG_WB_SW : std_logic 

    );
end wbk;

architecture archi of wbk is 

begin 

REG_DEST_SW <= MEM_DEST_SW; 

REG_WB_SW <= '1' when (MEM_WB_SW = '1' and MEM2WBK_EMPTY_SM = '0') else '0';

MEM2WBK_POP_SW <= not MEM2WBK_EMPTY_SM; 
REG_DATA_SW <= MEM_DATA_SW; 

end archi;