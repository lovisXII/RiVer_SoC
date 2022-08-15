library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity wb_led is
    generic(
        BASE_ADR        :   std_logic_vector(31 downto 0)
    );
    port(
        clk, reset_n    :   in  std_logic;
        
        -- wishbone interface
        ACK_O           :   out std_logic; 
        STB_I           :   in  std_logic;
        WE_I            :   in  std_logic;
          
        DAT_I           :   in  std_logic_vector(31 downto 0); 
        SEL_I           :   in  std_logic_vector(3 downto 0);
        DAT_O           :   out std_logic_vector(31 downto 0);
        ADR_I           :   in  std_logic_vector(31 downto 0);
        
        -- physical interface
        leds            :   out std_logic_vector(15 downto 0); 
        rgb0, rgb1      :   out std_logic_vector(2 downto 0)
    );
end wb_led;

architecture archi of wb_led is

constant adr1   :   std_logic_vector(31 downto 0)   :=  x"00000004"; 
constant adr2   :   std_logic_vector(31 downto 0)   :=  x"00000008";
constant adr3   :   std_logic_vector(31 downto 0)   :=  x"0000000C"; 

constant reg_adr1 : std_logic_vector(31 downto 0)   :=  std_logic_vector(unsigned(BASE_ADR) + unsigned(adr1));
constant reg_adr2 : std_logic_vector(31 downto 0)   :=  std_logic_vector(unsigned(BASE_ADR) + unsigned(adr2));
constant reg_adr3 : std_logic_vector(31 downto 0)   :=  std_logic_vector(unsigned(BASE_ADR) + unsigned(adr3));

signal slv_reg0, slv_reg1, slv_reg2, slv_reg3 : std_logic_vector(31 downto 0); 

type state is (idle, w, r); 
signal EP, EF : state; 

signal wenable : std_logic; 

signal data_reg, adr_reg    :   std_logic_vector(31 downto 0);
signal sel_reg              :   std_logic_vector(3 downto 0);
begin

fsm_transition : process(clk, reset_n) 
begin 
    if reset_n = '0' then 
        EP  <=  idle;
    elsif rising_edge(clk) then 
        EP  <=  EF;
    end if; 
    
    case EP is 
        when    idle    =>
            EF  <=  idle; 
            if STB_I = '1' and WE_I = '1' then 
                EF  <=  w;
            elsif STB_I = '1' and WE_I = '0' then 
                EF  <=  r;
            end if; 
        when    w       =>  EF  <=  idle; 
        when    r       =>  EF  <=  idle; 
    end case;
end process;    

fsm_ouput : process(EP)
begin 
    case EP is 
        when    idle    =>
            wenable <=  '0'; 
            ACK_O   <=  '0'; 
        when    w       =>
            wenable <=  '1';
            ACK_O   <=  '1'; 
        when    r       =>
            wenable <=  '0';
            ACK_O   <=  '1'; 
    end case; 
end process;

write_reg : process(clk, reset_n)
begin 
    if reset_n = '0' then
        slv_reg0    <=  x"00000000";
        slv_reg1    <=  x"00000000";
        slv_reg2    <=  x"00000000";
        slv_reg3    <=  x"00000000";
    elsif rising_edge(clk) then 
        if wenable = '1' then 
            if adr_reg  =   BASE_ADR        then 
                if sel_reg(0) = '1' then 
                    slv_reg0(31 downto 24)  <=   data_reg(31 downto 24);    end if; 

                if sel_reg(1) = '1' then 
                    slv_reg0(23 downto 16)  <=   data_reg(23 downto 16);    end if; 
                
                if sel_reg(2) = '1' then 
                    slv_reg0(15 downto 8)   <=   data_reg(15 downto 8);     end if;
                    
                if sel_reg(3) = '1' then 
                    slv_reg0(7 downto 0)    <=   data_reg(7 downto 0);      end if;
                    
            elsif adr_reg   =   reg_adr1    then
                if sel_reg(0) = '1' then 
                    slv_reg1(31 downto 24)  <=   data_reg(31 downto 24);    end if; 

                if sel_reg(1) = '1' then 
                    slv_reg1(23 downto 16)  <=   data_reg(23 downto 16);    end if; 
                
                if sel_reg(2) = '1' then 
                    slv_reg1(15 downto 8)   <=   data_reg(15 downto 8);     end if;
                    
                if sel_reg(3) = '1' then 
                    slv_reg1(7 downto 0)    <=   data_reg(7 downto 0);      end if;
                    
            elsif adr_reg   =   reg_adr2    then
                if sel_reg(0) = '1' then 
                    slv_reg2(31 downto 24)  <=   data_reg(31 downto 24);    end if; 

                if sel_reg(1) = '1' then 
                    slv_reg2(23 downto 16)  <=   data_reg(23 downto 16);    end if; 
                
                if sel_reg(2) = '1' then 
                    slv_reg2(15 downto 8)   <=   data_reg(15 downto 8);     end if;
                    
                if sel_reg(3) = '1' then 
                    slv_reg2(7 downto 0)    <=   data_reg(7 downto 0);      end if;
                    
            elsif adr_reg   =   reg_adr3    then
                if sel_reg(0) = '1' then 
                    slv_reg3(31 downto 24)  <=   data_reg(31 downto 24);    end if; 

                if sel_reg(1) = '1' then 
                    slv_reg3(23 downto 16)  <=   data_reg(23 downto 16);    end if; 
                
                if sel_reg(2) = '1' then 
                    slv_reg3(15 downto 8)   <=   data_reg(15 downto 8);     end if;
                    
                if sel_reg(3) = '1' then 
                    slv_reg3(7 downto 0)    <=   data_reg(7 downto 0);      end if;        
            end if;    
         end if; 
    end if; 
end process; 

reg_wb : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        adr_reg     <=  x"00000000"; 
        data_reg    <=  x"00000000"; 
        sel_reg     <=  "0000"; 
    elsif rising_edge(clk) then 
        adr_reg     <=  ADR_I; 
        data_reg    <=  DAT_I; 
        sel_reg     <=  SEL_I; 
    end if;
end process; 

DAT_O   <=  slv_reg0    when    adr_reg = BASE_ADR  else
            slv_reg1    when    adr_reg = reg_adr1  else 
            slv_reg2    when    adr_reg = reg_adr2  else 
            slv_reg3    when    adr_reg = reg_adr3  else 
            x"00000000"; 

leds    <=  slv_reg0(15 downto 0);
rgb0    <=  slv_reg1(2 downto 0);
rgb1    <=  slv_reg2(2 downto 0);

end archi;
