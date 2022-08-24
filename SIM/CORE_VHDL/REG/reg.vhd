library ieee; 
use ieee.std_logic_1164.all;
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
signal registers : reg_t;


-- debug 
signal reg0, reg1, reg2, reg3, reg4, reg5, reg6, reg7, reg8, reg9, reg10, reg11, reg12, reg13, reg14, reg15, reg16, reg17, reg18, reg19, reg20, reg21, reg22, reg23, reg24, reg25, reg26, reg27, reg28, reg29, reg30, reg31 : std_logic_vector(31 downto 0);

begin 

process(clk)
begin        
    if reset_n = '0' then
        -- init all registers except PC to 0
        for i in 0 to 31 loop 
            registers(i) <= x"00000000";
        end loop;
        registers(32) <= PC_INIT; 

    elsif(rising_edge(clk)) then

            if WRITE_PC_ENABLE_SD = '1' then 
                registers(32) <= WRITE_PC_SD; 
            end if;
            if(WADR_SW /= "111111") or WRITE_PC_ENABLE_SD = '0' then 
                if WADR_SW /= "000000" then 
                    if WENABLE_SW = '1' then 
                        registers(to_integer(unsigned(WADR_SW))) <= WDATA_SW; 
                    end if;
                end if;
            end if;
    end if;
end process; 
 

RDATA1_SR <= registers(to_integer(unsigned(RADR1_SD)));
RDATA2_SR <= registers(to_integer(unsigned(RADR2_SD)));
READ_PC_SR <= registers(32);

-- debug 
-- because some people here want to use gtkwave
-- what a shame 
reg0 <= registers(0);
reg1 <= registers(1);
reg2 <= registers(2);
reg3 <= registers(3);
reg4 <= registers(4);
reg5 <= registers(5);
reg6 <= registers(6);
reg7 <= registers(7);
reg8 <= registers(8);
reg9 <= registers(9);
reg10 <= registers(10);
reg11 <= registers(11);
reg12 <= registers(12);
reg13 <= registers(13);
reg14 <= registers(14);
reg15 <= registers(15);
reg16 <= registers(16);
reg17 <= registers(17);
reg18 <= registers(18);
reg19 <= registers(19);
reg20 <= registers(20);
reg21 <= registers(21);
reg22 <= registers(22);
reg23 <= registers(23);
reg24 <= registers(24);
reg25 <= registers(25);
reg26 <= registers(26);
reg27 <= registers(27);
reg28 <= registers(28);
reg29 <= registers(29);
reg30 <= registers(30);
reg31 <= registers(31);
-- i should be paid more for doing this...

end archi;