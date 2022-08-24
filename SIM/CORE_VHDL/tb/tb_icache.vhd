library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity tb_icache is 
end entity;

architecture simu of tb_icache is 
signal clk, reset_n : std_logic := '0'; 

signal ADR_SI, IC_INST_SI : std_logic_vector(31 downto 0);
signal ADR_VALID_SI, IC_STALL_SI : std_logic;

signal RAM_DATA, RAM_ADR : std_logic_vector(31 downto 0);
signal RAM_VALID, RAM_ACK : std_logic;

component icache 
    port(
        -- global interface
        clk, reset_n    :   in  std_logic;

        -- core interface
        ADR_SI          :   in  std_logic_vector(31 downto 0);
        ADR_VALID_SI    :   in  std_logic;
        IC_INST_SI      :   out std_logic_vector(31 downto 0);
        IC_STALL_SI     :   out std_logic;

        -- ram inteface
        RAM_DATA        :   in  std_logic_vector(31 downto 0);
        RAM_ADR         :   out std_logic_vector(31 downto 0);
        RAM_VALID       :   out std_logic;
        RAM_ACK         :   in  std_logic
    );
end component;

begin

icache_i : icache
    port map(
        clk             =>  clk, 
        reset_n         =>  reset_n,
        ADR_SI          =>  ADR_SI,
        ADR_VALID_SI    =>  ADR_VALID_SI, 
        IC_INST_SI      =>  IC_INST_SI, 
        IC_STALL_SI     =>  IC_STALL_SI, 
        
        RAM_DATA        =>  RAM_DATA, 
        RAM_ADR         =>  RAM_ADR, 
        RAM_VALID       =>  RAM_VALID, 
        RAM_ACK         =>  RAM_ACK
    );


clk <= not clk after 5 ns;
reset_n <= '0', '1' after 6 ns;  

ADR_SI <=   x"00000000", x"00000004" after 35 ns, x"00000008" after 45 ns; 
ADR_VALID_SI <= '0', '1' after 15 ns; 

RAM_DATA <= x"AAAABBBB", x"ABCDEF01" after 35 ns; 
RAM_ACK <= '0', '1' after 25 ns, '0' after 55 ns;

end simu; 