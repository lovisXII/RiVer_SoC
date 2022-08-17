library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity wb_lcd is
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
end wb_lcd;

architecture archi of wb_lcd is

constant NB_CMD : integer := 6;

constant adr1   :   std_logic_vector(31 downto 0)   :=  x"00000004"; 
constant adr2   :   std_logic_vector(31 downto 0)   :=  x"00000008";
constant adr3   :   std_logic_vector(31 downto 0)   :=  x"0000000C"; 

constant reg_adr0 : std_logic_vector(31 downto 0)   :=  BASE_ADR; 
constant reg_adr1 : std_logic_vector(31 downto 0)   :=  std_logic_vector(unsigned(BASE_ADR) + unsigned(adr1));
constant reg_adr2 : std_logic_vector(31 downto 0)   :=  std_logic_vector(unsigned(BASE_ADR) + unsigned(adr2));
constant reg_adr3 : std_logic_vector(31 downto 0)   :=  std_logic_vector(unsigned(BASE_ADR) + unsigned(adr3));

signal adr_reg, data_reg : std_logic_vector(31 downto 0); 

signal slv_reg0, slv_reg1, slv_reg2, slv_reg3 : std_logic_vector(31 downto 0);

signal wenable : std_logic; 

type state is (idle, w, cmd_lsb, cmd_delay0, cmd_msb, cmd_delay1, data_lsb, data_delay0, data_msb, data_delay1);
signal EP, EF : state;
 
type buf_t is array (0 to 79) of std_logic_vector(7 downto 0); 
signal buf_lcd : buf_t;
signal cpt_buf_o : integer range 0 to 80;
signal cpt_buf_i : integer range 0 to 20;
signal next_data : std_logic;  
signal cpt_buf_en : std_logic;
signal buf_full : std_logic;
signal buf_empty  : std_logic;

type cmd_t is array (0 to 4) of std_logic_vector(7 downto 0);
constant cmd_lcd : cmd_t := (x"38",x"0c",x"06",x"01",x"C0");
signal cpt_cmd : integer range 0 to (NB_CMD-1);
signal end_cmd  : std_logic;
signal next_cmd : std_logic;

signal cpt_delay : integer range 0 to (DELAY-1); 
signal cpt_delay_en     : std_logic; 
signal cpt_delay_end    : std_logic; 

signal lcd_data_buf : std_logic_vector(3 downto 0);
signal lcd_data_cmd : std_logic_vector(3 downto 0);

signal lsb_msb  : std_logic;

signal lcd_rs_tmp : std_logic; 
begin


fsm_transition : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        EP  <=  idle; 
    elsif rising_edge(clk) then 
        EP  <=  EF;
    end if; 

    case EP is 
        when idle       =>  EF  <=  idle;
            if STB_I = '1' and WE_I = '1' then 
                EF  <=  w; 
            end if; 
        when w          =>  EF  <=  idle;
            if buf_full = '1' then 
                EF  <=  cmd_lsb;
            end if;  

        when cmd_lsb    =>  EF  <=  cmd_delay0; 

        when cmd_delay0 =>  EF  <=  cmd_delay0;
            if cpt_delay_end = '1' then 
                EF  <=  cmd_msb; 
            end if; 

        when cmd_msb    =>  EF  <=  cmd_delay1; 

        when cmd_delay1 =>  EF  <=  cmd_delay1; 
            if cpt_delay_end = '1' and end_cmd = '0' then 
                EF  <=  cmd_lsb; 
            elsif cpt_delay_end = '1' and end_cmd = '1' then 
                EF  <=  data_lsb;
            end if; 

        when data_lsb   =>  EF  <=  data_delay0; 
        
        when data_delay0 => EF  <=  data_delay0; 
            if cpt_delay_end = '1' then 
                EF  <=  data_msb; 
            end if;
        
        when data_msb   =>  EF  <=  data_delay1;

        when data_delay1 => EF  <=  data_delay1; 
            if cpt_delay_end = '1' and buf_empty = '0' then 
                EF  <=  data_lsb; 
            elsif cpt_delay_end = '1' and buf_empty = '1' then 
                EF  <=  idle; 
            end if; 
    end case; 
end process; 

fsm_output : process(EP)
begin 
    case EP is 
        when    idle    =>  
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '0'; 
            lcd_e       <=  '0'; 
            wenable     <=  '0'; 
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0'; 
            cpt_buf_en  <=  '0';
            next_data   <=  '0'; 
            lsb_msb     <=  '0';
        when    w       => 
            ACK_O       <=  '1'; 
            lcd_rs_tmp  <=  '0'; 
            lcd_e       <=  '0'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0'; 
            cpt_buf_en  <=  '1';
            next_data   <=  '0'; 
            lsb_msb     <=  '0';
        when    cmd_lsb => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '0'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0'; 
            cpt_buf_en  <=  '0';
            next_data   <=  '0'; 
            lsb_msb     <=  '1';
        when    cmd_delay0  => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '0'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '1'; 
            next_cmd    <=  '0';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '0';
            lsb_msb     <=  '1';
        when    cmd_msb => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '0'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '1';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '0';
            lsb_msb     <=  '0';
        when    cmd_delay1  => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '0'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '1'; 
            next_cmd    <=  '0';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '0';
            lsb_msb     <=  '0';
        when    data_lsb    => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '1'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '0';
            lsb_msb     <=  '1';
        when    data_delay0 => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '1'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '0';
            lsb_msb     <=  '1';
        when    data_msb    => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '1'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '1';
            lsb_msb     <=  '0';
        when    data_delay1 => 
            ACK_O       <=  '0'; 
            lcd_rs_tmp  <=  '1'; 
            lcd_e       <=  '1'; 
            wenable     <=  '0';
            cpt_delay_en      <=  '0'; 
            next_cmd    <=  '0';
            cpt_buf_en  <=  '0'; 
            next_data   <=  '0';
            lsb_msb     <=  '0';
    end case;
            
end process; 

reg_write : process(clk, reset_n) 
begin 
    if reset_n = '0' then 
        slv_reg0    <=  x"00000000"; 
        slv_reg1    <=  x"00000000"; 
        slv_reg2    <=  x"00000000"; 
        slv_reg3    <=  x"00000000"; 
    elsif rising_edge(clk) then 
        if wenable = '1' then 
            if adr_reg = reg_adr0 then 
                slv_reg0    <=  data_reg; 
            elsif adr_reg = reg_adr1 then 
                slv_reg1    <=  data_reg; 
            elsif adr_reg = reg_adr2 then 
                slv_reg2    <=  data_reg; 
            elsif adr_reg = reg_adr3 then 
                slv_reg3    <=  data_reg; 
            end if; 
        end if; 
    end if; 
end process; 

reg_wb : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        adr_reg     <=  x"00000000"; 
        data_reg    <=  x"00000000"; 
    elsif rising_edge(clk) then 
        adr_reg     <=  ADR_I; 
        data_reg    <=  DAT_I; 
    end if; 
end process; 

buf : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        reset_buf : for i in 0 to 79 loop
            buf_lcd(i)  <=  x"00";
        end loop;
        buf_full    <=  '0';
        buf_empty   <=  '1';
        cpt_buf_i   <=  0;
        cpt_buf_o   <=  0;
    elsif rising_edge(clk) then
        if cpt_buf_en = '1' then 
            buf_lcd(cpt_buf_i)      <=  slv_reg0(7 downto 0);
            buf_lcd(cpt_buf_i + 1)  <=  slv_reg0(15 downto 8);
            buf_lcd(cpt_buf_i + 2)  <=  slv_reg0(23 downto 16);
            buf_lcd(cpt_buf_i + 3)  <=  slv_reg0(31 downto 24);
            cpt_buf_i               <=  cpt_buf_i + 1; 
            if cpt_buf_i = 19 then
                buf_full <= '1';
                cpt_buf_i <= 0; 
            else 
                buf_full <= '0';
            end if;
        elsif next_data = '1' then 
            cpt_buf_o   <=  cpt_buf_o + 1;
            if cpt_buf_o = 79 then 
                buf_empty <= '1';
            else 
                buf_empty <= '0';
            end if; 
        end if;
    end if;
end process; 

cmd_reg : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        cpt_cmd <=  0;
        end_cmd <=  '0';

    elsif rising_edge(clk) then 
        if next_cmd = '1' then 
            cpt_cmd <=  cpt_cmd + 1;
            if cpt_cmd = (NB_CMD-1) then 
                cpt_cmd <= 0;
                end_cmd <= '1';
            else 
                end_cmd <= '0';
            end if;
        end if;
    end if;
end process;

cpt : process(clk, reset_n)
begin 
    if reset_n = '0' then 
        cpt_delay       <=  0;
        cpt_delay_end   <=  '0';
    elsif rising_edge(clk) then 
        if cpt_delay_en = '1' then 
            cpt_delay   <=  cpt_delay + 1;
            if cpt_delay = (DELAY-1) then 
                cpt_delay_end   <=  '1';
            else 
                cpt_delay_end   <=  '0';
            end if;
        else 
            cpt_delay   <=  0;
        end if;
    end if;
end process;

lcd_data_buf    <=  buf_lcd(cpt_buf_o)(3 downto 0)  when lsb_msb = '1' else 
                    buf_lcd(cpt_buf_o)(7 downto 4);

lcd_data_cmd    <=  cmd_lcd(cpt_cmd)(3 downto 0)    when lsb_msb = '1' else 
                    cmd_lcd(cpt_cmd)(7 downto 4);

lcd_data        <=  lcd_data_buf    when    lcd_rs_tmp = '1'    else 
                    lcd_data_cmd;

lcd_rs      <=  lcd_rs_tmp;
lcd_rw      <=  '0';            -- write only 

DAT_O       <=  x"00000000";    -- nothing to read

end archi;
