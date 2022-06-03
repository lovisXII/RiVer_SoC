library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity wbk is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Mem 
        DATA_RM : in std_logic_vector(31 downto 0);
        DEST_RM : in std_logic_vector(5 downto 0);
        MEM_SIZE_RM : in std_logic_vector(1 downto 0);
        WB_RM : in std_logic;
        SIGN_EXTEND_RM : in std_logic;
        LOAD_RM : in std_logic;

        -- mem2wbk interface
        MEM2WBK_EMPTY_SM : in std_logic;
        MEM2WBK_POP_SW : out std_logic;

        -- Reg interface
        DATA_SW : out std_logic_vector(31 downto 0);
        DEST_SW : out std_logic_vector(5 downto 0);
        WB_SW : out std_logic 

    );
end wbk;

architecture archi of wbk is 

begin 

DEST_SW <= DEST_RM; 

WB_SW <= '1' when (WB_RM = '1' and MEM2WBK_EMPTY_SM = '0') else '0';

MEM2WBK_POP_SW <= not MEM2WBK_EMPTY_SM; 
DATA_SW <= DATA_RM; 

end archi;