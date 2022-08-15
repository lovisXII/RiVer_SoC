library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity top is
    port(
        -- global
        clk0, reset_n   :   in  std_logic;
        
        -- leds 
        LED             :   out std_logic_vector(15 downto 0);
        rgb0, rgb1      :   out std_logic_vector(2 downto 0);
        
        -- lcd
        lcd_rw          :   out std_logic;  
        lcd_e           :   out std_logic;  
        lcd_rs          :   out std_logic;  
        lcd_data        :   out std_logic_vector(3 downto 0);
        
        -- ram
        ram_adr         :   out std_logic_vector(31 downto 0);
        ram_en          :   out std_logic;
        ram_di          :   out std_logic_vector(31 downto 0);
        ram_do          :   in  std_logic_vector(31 downto 0);
        ram_byt_sel     :   out std_logic_vector(3 downto 0)
    );
end top;

architecture archi of top is

constant led_base_adr   :   std_logic_vector(31 downto 0)   :=  x"40000000";
constant led_end_adr    :   std_logic_vector(31 downto 0)   :=  x"4000000C";

constant lcd_base_adr   :   std_logic_vector(31 downto 0)   :=  x"60000000";
constant lcd_end_adr    :   std_logic_vector(31 downto 0)   :=  x"6000000C";

signal clk : std_logic; 

signal ack_ram  :   std_logic;
signal ack_led  :   std_logic; 
signal ack_lcd  :   std_logic; 
signal ack_slv  :   std_logic; 

signal stb_cpu  :   std_logic;
signal we_cpu   :   std_logic;

signal data_cpu :   std_logic_vector(31 downto 0);
signal data_ram :   std_logic_vector(31 downto 0);
signal data_led :   std_logic_vector(31 downto 0);
signal data_lcd :   std_logic_vector(31 downto 0);
signal data_slv :   std_logic_vector(31 downto 0);

signal sel_cpu  :   std_logic_vector(3 downto 0);

signal adr_cpu  :   std_logic_vector(31 downto 0);


signal stb_ram  :   std_logic;
signal stb_led  :   std_logic;
signal stb_lcd  :   std_logic;  

signal w_ram    :   std_logic;
signal w_led    :   std_logic; 
signal w_lcd    :   std_logic;

signal select_ram   :   std_logic; 
signal select_led   :   std_logic; 
signal select_lcd   :   std_logic;

component wb_cpu is
    port(
        clk, reset_n    :   in  std_logic; 
        
        ACK_I           :   in  std_logic;
        STB_O           :   out std_logic;
        WE_O            :   out std_logic;

        DAT_I           :   in  std_logic_vector(31 downto 0);
        DAT_O           :   out std_logic_vector(31 downto 0);
        SEL_O           :   out std_logic_vector(3 downto 0);
        ADR_O           :   out std_logic_vector(31 downto 0)
    );
end component;

component wb_ram
    generic(SIZE : integer);
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
end component;


component wb_led
    generic(
        BASE_ADR        :   std_logic_vector(31 downto 0)
    );
    port(
        clk, reset_n    :   in  std_logic;
        
        ACK_O           :   out std_logic; 
        STB_I           :   in  std_logic;
        WE_I            :   in  std_logic;
          
        DAT_I           :   in  std_logic_vector(31 downto 0); 
        SEL_I           :   in  std_logic_vector(3 downto 0);
        DAT_O           :   out std_logic_vector(31 downto 0);
        ADR_I           :   in  std_logic_vector(31 downto 0);
        
        leds            :   out std_logic_vector(15 downto 0); 
        rgb0, rgb1      :   out std_logic_vector(2 downto 0)
    );
end component;

component wb_lcd
    generic(
        BASE_ADR        :   std_logic_vector(31 downto 0);
        DELAY           :   integer
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
        lcd_rw          :   out std_logic;  --  read / write
        lcd_e           :   out std_logic;  --  enable

        lcd_rs          :   out std_logic;  --  data / command
        lcd_data        :   out std_logic_vector(3 downto 0)
    );
end component;

begin

clk <= clk0;  

wb_master_cpu : wb_cpu
    port map(    
        clk             =>  clk,
        reset_n         =>  reset_n, 
        
        ACK_I           =>  ack_slv,
        STB_O           =>  stb_cpu, 
        WE_O            =>  we_cpu, 

        DAT_I           =>  data_slv,
        DAT_O           =>  data_cpu,
        SEL_O           =>  sel_cpu,
        ADR_O           =>  adr_cpu
    );

wb_slave_ram : wb_ram 
    generic map(SIZE => 4096)   -- 262144
    port map(
        clk             =>  clk,
        reset_n         =>  reset_n,

        ACK_O           =>  ack_ram,
        STB_I           =>  stb_ram,
        WE_I            =>  w_ram,

        DAT_I           =>  data_cpu,
        SEL_I           =>  sel_cpu,
        DAT_O           =>  data_ram,
        ADR_I           =>  adr_cpu,
        
        ram_adr         =>  ram_adr, 
        ram_en          =>  ram_en,
        ram_di          =>  ram_di, 
        ram_do          =>  ram_do,
        ram_byt_sel     =>  ram_byt_sel     

    );

wb_slave_led : wb_led 
    generic map(
        BASE_ADR    =>  led_base_adr
    )
    port map(
        clk         =>  clk,
        reset_n     =>  reset_n,
        
        ACK_O       =>  ack_led,
        STB_I       =>  stb_led,
        WE_I        =>  w_led,
          
        DAT_I       =>  data_cpu,
        SEL_I       =>  sel_cpu,
        DAT_O       =>  data_led,
        ADR_I       =>  adr_cpu,
        
        leds        =>  LED,
        rgb0        =>  rgb0,
        rgb1        =>  rgb1        
    );
    
wb_slave_lcd : wb_lcd 
    generic map(
        BASE_ADR    =>  lcd_base_adr,
        DELAY       =>  1000
    )
    port map(
        clk         =>  clk,
        reset_n     =>  reset_n,
        
        ACK_O       =>  ack_lcd,
        STB_I       =>  stb_lcd, 
        WE_I        =>  w_lcd, 
        
        DAT_I       =>  data_cpu, 
        SEL_I       =>  sel_cpu, 
        DAT_O       =>  data_lcd, 
        ADR_I       =>  adr_cpu,
        
        lcd_rw      =>  lcd_rw,
        lcd_e       =>  lcd_e,

        lcd_rs      =>  lcd_rs,  
        lcd_data    =>  lcd_data
    );
    
   
-- adress comparator
select_ram  <=  '1' when    adr_cpu < led_base_adr  else 
                '0'; 
stb_ram     <=  select_ram and stb_cpu;
w_ram       <=  select_ram and we_cpu;    
            
select_led  <=  '1' when    adr_cpu >= led_base_adr and adr_cpu <= led_end_adr  else 
                '0'; 
stb_led     <=  select_led and stb_cpu; 
w_led       <=  select_led and we_cpu; 


select_lcd  <=  '1' when    adr_cpu >=  lcd_base_adr and adr_cpu <= lcd_end_adr else 
                '0'; 
stb_lcd     <=  select_lcd and stb_cpu; 
w_lcd       <=  select_lcd and we_cpu; 
                
data_slv    <=  data_ram    when select_ram =   '1' else 
                data_led    when select_led =   '1' else 
                data_lcd    when select_lcd =   '1' else
                x"00000000";
                
ack_slv     <=  ack_ram     when select_ram =   '1' else 
                ack_led     when select_led =   '1' else 
                ack_lcd     when select_lcd =   '1' else
                '0'; 
                                
end archi;
