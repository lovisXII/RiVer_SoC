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
signal enable0, enable1, enable2, enable3 : std_logic;

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
    
ram_byt_sel(0) <=  sel_reg(0) and enable; 
ram_byt_sel(1) <=  sel_reg(1) and enable; 
ram_byt_sel(2) <=  sel_reg(2) and enable; 
ram_byt_sel(3) <=  sel_reg(3) and enable; 

--bram_0 : entity work.bram 
--    generic map(
--        SIZE    =>  SIZE,
--        LENGTH  =>  32
--    )
--    port map(
--        clk     =>  clk,
--        we      =>  wenable, 
--        en      =>  enable0, 
--        addr    =>  adr_adapt(31 downto 2), 
--        di      =>  DAT_I, 
--        do      =>  DAT_O   
--   );

--bram_1 : entity work.bram 
--    generic map(SIZE => SIZE)
--    port map(clk, wenable, enable1, adr_adapt(31 downto 2), DAT_I(15 downto 8), DAT_O(15 downto 8));
  
--bram_2 : entity work.bram 
--    generic map(SIZE => SIZE)
--    port map(clk, wenable, enable2, adr_adapt(31 downto 2), DAT_I(23 downto 16), DAT_O(23 downto 16));
  
--bram_3 : entity work.bram 
--    generic map(SIZE => SIZE)
--    port map(clk, wenable, enable3, adr_adapt(31 downto 2), DAT_I(31 downto 24), DAT_O(31 downto 24));
      
--ram_mem : process(clk, reset_n)
--begin 
--    if reset_n = '0' then 
--    ram(0)   <=  x"00052503";
--    ram(1)   <=  x"00008067";
--    ram(2)   <=  x"00b52023";
--    ram(3)   <=  x"00008067";
--    ram(4)   <=  x"00008067";
--    ram(5)   <=  x"000105b7";
--    ram(6)   <=  x"ff010113";
--    ram(7)   <=  x"fff58593";
--    ram(8)   <=  x"40000537";
--    ram(9)   <=  x"00112623";
--    ram(10)   <=  x"00812423";
--    ram(11)   <=  x"fddff0ef";
--    ram(12)   <=  x"40000437";
--    ram(13)   <=  x"00440513";
--    ram(14)   <=  x"00700593";
--    ram(15)   <=  x"fcdff0ef";
--    ram(16)   <=  x"00840513";
--    ram(17)   <=  x"00812403";
--    ram(18)   <=  x"00c12083";
--    ram(19)   <=  x"00700593";
--    ram(20)   <=  x"01010113";
--    ram(21)   <=  x"fb5ff06f";
--    ram(22)   <=  x"ff010113";
--    ram(23)   <=  x"00000593";
--    ram(24)   <=  x"40000537";
--    ram(25)   <=  x"00112623";
--    ram(26)   <=  x"00812423";
--    ram(27)   <=  x"f9dff0ef";
--    ram(28)   <=  x"40000437";
--    ram(29)   <=  x"00440513";
--    ram(30)   <=  x"00000593";
--    ram(31)   <=  x"f8dff0ef";
--    ram(32)   <=  x"00840513";
--    ram(33)   <=  x"00812403";
--    ram(34)   <=  x"00c12083";
--    ram(35)   <=  x"00000593";
--    ram(36)   <=  x"01010113";
--    ram(37)   <=  x"f75ff06f";
--    ram(38)   <=  x"fe010113";
--    ram(39)   <=  x"00812c23";
--    ram(40)   <=  x"00912a23";
--    ram(41)   <=  x"01212823";
--    ram(42)   <=  x"01312623";
--    ram(43)   <=  x"01412423";
--    ram(44)   <=  x"00112e23";
--    ram(45)   <=  x"01512223";
--    ram(46)   <=  x"00050a13";
--    ram(47)   <=  x"00159493";
--    ram(48)   <=  x"00100993";
--    ram(49)   <=  x"00100413";
--    ram(50)   <=  x"00000913";
--    ram(51)   <=  x"02994463";
--    ram(52)   <=  x"01c12083";
--    ram(53)   <=  x"01812403";
--    ram(54)   <=  x"00c12983";
--    ram(55)   <=  x"00812a03";
--    ram(56)   <=  x"00412a83";
--    ram(57)   <=  x"02010113";
--    ram(58)   <=  x"00008067";
--    ram(59)   <=  x"01000a93";
--    ram(60)   <=  x"02098863";
--    ram(61)   <=  x"00141413";
--    ram(62)   <=  x"00040593";
--    ram(63)   <=  x"40000537";
--    ram(64)   <=  x"f01ff0ef";
--    ram(65)   <=  x"000a0513";
--    ram(66)   <=  x"fffa8a93";
--    ram(67)   <=  x"efdff0ef";
--    ram(68)   <=  x"fe0a90e3";
--    ram(69)   <=  x"0019c993";
--    ram(70)   <=  x"00190913";
--    ram(71)   <=  x"fa9ff06f";
--    ram(72)   <=  x"40145413";
--    ram(73)   <=  x"fd5ff06f";
--    ram(74)   <=  x"fe010113";
--    ram(75)   <=  x"00812c23";
--    ram(76)   <=  x"00912a23";
--    ram(77)   <=  x"01212823";
--    ram(78)   <=  x"01312623";
--    ram(79)   <=  x"00112e23";
--    ram(80)   <=  x"00050493";
--    ram(81)   <=  x"00058913";
--    ram(82)   <=  x"00000413";
--    ram(83)   <=  x"05000993";
--    ram(84)   <=  x"02000593";
--    ram(85)   <=  x"01245663";
--    ram(86)   <=  x"008487b3";
--    ram(87)   <=  x"0007c583";
--    ram(88)   <=  x"60000537";
--    ram(89)   <=  x"00140413";
--    ram(90)   <=  x"e99ff0ef";
--    ram(91)   <=  x"ff3412e3";
--    ram(92)   <=  x"01c12083";
--    ram(93)   <=  x"01812403";
--    ram(95)   <=  x"01412483";
--    ram(96)   <=  x"01012903";
--    ram(97)   <=  x"00c12983";
--    ram(98)   <=  x"02010113";
--    ram(99)   <=  x"00008067";
--    ram(100)   <=  x"01700593";
--    ram(101)   <=  x"24c00513";
--    ram(102)   <=  x"f95ff06f";
--    ram(103)   <=  x"ff010113";
--    ram(104)   <=  x"00812423";
--    ram(105)   <=  x"00112623";
--    ram(106)   <=  x"00a00413";
--    ram(107)   <=  x"e65ff0ef";
--    ram(108)   <=  x"0c800513";
--    ram(109)   <=  x"e59ff0ef";
--    ram(110)   <=  x"e9dff0ef";
--    ram(111)   <=  x"0c800513";
--    ram(112)   <=  x"fff40413";
--    ram(113)   <=  x"e49ff0ef";
--    ram(114)   <=  x"fe0412e3";
--    ram(115)   <=  x"00c12083";
--    ram(116)   <=  x"00812403";
--    ram(117)   <=  x"00000513";
--    ram(118)   <=  x"01010113";
--    ram(119)   <=  x"00008067";
--    ram(120)   <=  x"3a434347";
--    ram(121)   <=  x"4e472820";
--    ram(122)   <=  x"31202955";
--    ram(123)   <=  x"2e322e30";
--    ram(124)   <=  x"1b410030";
--    ram(125)   <=  x"72000000";
--    ram(126)   <=  x"76637369";
--    ram(127)   <=  x"00110100";
--    ram(128)   <=  x"10040000";
--    ram(129)   <=  x"33767205";
--    ram(130)   <=  x"70326932";
--    ram(131)   <=  x"1b410030";
--    ram(132)   <=  x"72000000";
--    ram(133)   <=  x"76637369";
--    ram(134)   <=  x"00110100";
--    ram(135)   <=  x"10040000";
--    ram(136)   <=  x"33767205";
--    ram(137)   <=  x"70326932";
--    ram(138)   <=  x"1b410030";
--    ram(139)   <=  x"72000000";
--    ram(140)   <=  x"76637369";
--    ram(141)   <=  x"00110100";
--    ram(142)   <=  x"10040000";
--    ram(143)   <=  x"33767205";
--    ram(144)   <=  x"70326932";
--    ram(145)   <=  x"00000030";
--    ram(146)   <=  x"6c6c6568";
--    ram(147)   <=  x"6f77206f";
--    ram(148)   <=  x"20646c72";
--    ram(149)   <=  x"6d6f7266";
--    ram(150)   <=  x"56695220";
--    ram(151)   <=  x"00217265";
--    ram(152)   <=  x"00001b41";
--    ram(153)   <=  x"73697200";
--    ram(154)   <=  x"01007663";
--    ram(155)   <=  x"00000011";
--    ram(156)   <=  x"72051004";
--    ram(157)   <=  x"69323376";
--    ram(158)   <=  x"00307032";
--
--    for i in 159 to (SIZE-1) loop
--        ram(i)  <=  x"00000000";
--    end loop;
--    elsif rising_edge(clk) then 
--        if wenable = '1' then 
--            if sel_reg(0) = '1' then 
--                ram(to_integer(unsigned(adr_reg(31 downto 2))))(7 downto 0)     <=  data_reg(7 downto 0);     end if; 
--            if sel_reg(1) = '1' then 
--                ram(to_integer(unsigned(adr_reg(31 downto 2))))(15 downto 8)    <=  data_reg(15 downto 8);    end if; 
--            if sel_reg(2) = '1' then 
--                ram(to_integer(unsigned(adr_reg(31 downto 2))))(23 downto 16)   <=  data_reg(23 downto 16);   end if; 
--            if sel_reg(3) = '1' then 
--                ram(to_integer(unsigned(adr_reg(31 downto 2))))(31 downto 24)   <=  data_reg(31 downto 24);   end if; 
--        end if; 
--    end if;          
--end process; 
--
adr_adapt <=    adr_max when ADR_I > adr_max else 
                ADR_I; 
                
--DAT_O  <=  ram(to_integer(unsigned(adr_adapt(31 downto 2))));
DAT_O   <=  ram_do;
ram_di  <=  DAT_I;
ram_adr <=  adr_adapt;
end archi;
