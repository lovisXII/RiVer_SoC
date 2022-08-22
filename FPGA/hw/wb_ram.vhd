library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity wb_ram is
    generic(SIZE : integer := 4096);
    port(
        -- global interface
        clk, reset_n    :   in  std_logic;
        -- wb interface
        ACK_O           :   out std_logic; 
        STB_I           :   in  std_logic;
        WE_I            :   in  std_logic;
       
        DAT_I           :   in  std_logic_vector(31 downto 0); 
        SEL_I           :   in  std_logic_vector(3 downto 0);
        DAT_O           :   out std_logic_vector(31 downto 0);
        ADR_I           :   in  std_logic_vector(31 downto 0);
        -- bram interface 
        ram_adr         :   out std_logic_vector(31 downto 0);
        ram_en          :   out std_logic;
        ram_di          :   out std_logic_vector(31 downto 0);
        ram_do          :   in  std_logic_vector(31 downto 0);
        ram_byt_sel     :   out std_logic_vector(3 downto 0)       
    );
end wb_ram;

architecture archi of wb_ram is

constant adr_max : std_logic_vector(31 downto 0) := std_logic_vector(to_unsigned(SIZE, 32));

--type tab32 is array (0 to SIZE-1) of std_logic_vector(31 downto 0);
--signal ram : tab32; 
--
signal wenable, enable : std_logic;

type state is (idle, r, w, done);
signal EP, EF : state; 

signal data_reg, adr_reg    :   std_logic_vector(31 downto 0);
signal sel_reg              :   std_logic_vector(3 downto 0);

signal adr_adapt : std_logic_vector(31 downto 0);

begin

reg_wb : process(clk, reset_n)
begin 
    if reset_n = '0' then  
        data_reg    <=  x"00000000";
        adr_reg     <=  x"00000000";
        sel_reg     <=  "0000";
    elsif rising_edge(clk) then 
        data_reg    <=  DAT_I; 
        adr_reg     <=  adr_adapt; 
        sel_reg     <=  SEL_I; 
    end if; 
end process; 

fsm_transition : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        EP  <=  idle; 
    elsif rising_edge(clk) then 
        EP  <=  EF; 
    end if;  
        
    case EP is 
        when idle   =>  EF  <=  idle; 
            if STB_I = '1' and WE_I = '1' then 
                EF  <=  w; 
            elsif STB_I = '1' and WE_I = '0' then 
                EF  <=  r;
            end if; 
      
        when w      =>  EF  <=  done; 
      
        when r      =>  EF  <=  done; 
      
        when done   =>  EF  <=  idle; 
    end case;

end process; 


fsm_output : process(EP)
begin 
    case EP is 
        when    idle    =>  
            ACK_O   <=  '0'; 
            wenable <=  '0';     
            enable  <=  '0';  
        when    w       =>
            ACK_O   <=  '0'; 
            wenable <=  '1';    
            enable  <=  '1';  
        when    r       =>
            ACK_O   <=  '0'; 
            wenable <=  '0';
            enable  <=  '1';
        when    done    =>
            ACK_O   <=  '1'; 
            wenable <=  '0'; 
            enable  <=  '0';
    end case; 
end process; 
    
ram_byt_sel(0) <=  sel_reg(0) and wenable; 
ram_byt_sel(1) <=  sel_reg(1) and wenable; 
ram_byt_sel(2) <=  sel_reg(2) and wenable; 
ram_byt_sel(3) <=  sel_reg(3) and wenable; 

adr_adapt <=    adr_max when ADR_I > adr_max else 
                ADR_I; 
                
DAT_O   <=  ram_do;
ram_di  <=  data_reg;
ram_adr <=  adr_reg;
ram_en  <=  enable;
end archi;
