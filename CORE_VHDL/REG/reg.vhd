library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity reg is 
    port(
        -- global interface
        clk, reset_n : in std_logic;
        PC_INIT : in std_logic_vector(31 downto 0);

        -- read ports
        RDATA1_SR, RDATA2_SR : out std_logic_vector(31 downto 0);
        RADR1_SD, RADR2_SD : in std_logic_vector(5 downto 0);
        
        -- write ports
        WDATA_SW : in std_logic_vector(31 downto 0);
        WADR_SW : in std_logic_vector(5 downto 0);
        WENABLE_SW : in std_logic; 

        WRITE_PC_SD : in std_logic_vector(31 downto 0);
        WRITE_PC_ENABLE_SD : in std_logic;

        -- PC
        READ_PC_SR : out std_logic_vector(31 downto 0)
    );
end reg;

architecture archi of reg is 

type reg_t is array(0 to 32) of std_logic_vector(31 downto 0);
signal reg : reg_t;

begin 

process(clk)
begin 
    if(rising_edge(clk)) then
        if reset_n = '0' then
            reg(32) <= PC_INIT; 
            -- init all registers except PC to 0
            for i in 0 to 31 loop 
                reg(i) <= x"00000000";
            end loop;
        else 
            if WRITE_PC_ENABLE_SD = '1' then 
                reg(32) <= WRITE_PC_SD; 
            end if;
            if(WADR_SW /= "111111") or WRITE_PC_ENABLE_SD = '0' then 
                if WADR_SW /= "000000" then 
                    if WENABLE_SW = '1' then 
                        reg(to_integer(unsigned(WADR_SW))) <= WDATA_SW; 
                    end if;
                end if;
            end if;
        end if;
    end if;
end process; 

RDATA1_SR <= reg(to_integer(unsigned(RADR1_SD)));
RDATA2_SR <= reg(to_integer(unsigned(RADR2_SD)));
READ_PC_SR <= reg(32);

end archi;