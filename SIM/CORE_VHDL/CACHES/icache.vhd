library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;

entity icache is 
    generic (
        WAYS : integer := 256
    );
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
        RAM_ADR_VALID   :   out std_logic;
        RAM_ACK         :   in  std_logic
    );
end icache;

architecture archi of icache is 

constant N_BITS_OFFSET : integer := 4;
constant N_BITS_WAYS : integer := integer(ceil(log2(real(WAYS))));
constant N_BITS_TAG : integer := 32 - N_BITS_WAYS - N_BITS_OFFSET;

type tag_tab is array (0 to (WAYS - 1)) of std_logic_vector((N_BITS_TAG - 1) downto 0);
signal tags : tag_tab;

type data_t is array (0 to (WAYS - 1)) of std_logic_vector(31 downto 0);

signal data0, data1, data2, data3 : data_t; 
signal data_valid :   std_logic_vector((WAYS - 1) downto 0);

signal adr_tag      :   std_logic_vector((N_BITS_TAG - 1) downto 0);
signal adr_index    :   std_logic_vector((N_BITS_WAYS - 1) downto 0);
signal adr_offset   :   std_logic_vector((N_BITS_OFFSET - 1) downto 0);

signal hit : std_logic := '0';

type state is (idle, wait_mem, update);
signal EP, EF : state; 

signal dbg_cpt : std_logic_vector(3 downto 0);
signal dbg_st : std_logic_vector(1 downto 0);

begin 

adr_tag     <=  ADR_SI(31 downto (N_BITS_OFFSET + N_BITS_WAYS)); 
adr_index   <=  ADR_SI((N_BITS_WAYS + N_BITS_OFFSET- 1) downto N_BITS_OFFSET);
adr_offset  <=  ADR_SI((N_BITS_OFFSET - 1) downto 0);

-- miss detection 
hit <=  '1' when adr_tag = tags(to_integer(unsigned(adr_index))) and data_valid(to_integer(unsigned(adr_index))) = '1' else 
        '0';

IC_INST_SI  <=  data0(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "00" and hit = '1' else 
                data1(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "01" and hit = '1' else
                data2(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "10" and hit = '1' else
                data3(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "11" and hit = '1' else
                x"00000000";

IC_STALL_SI <=  not(hit);

-- succession des etats 
fsm_transition : process(clk, reset_n)
begin  
    if reset_n = '0' then 
        EP  <=  idle; 
    elsif rising_edge(clk) then 
        EP  <=  EF; 
    end if; 
end process; 

fsm_output : process(clk, EP, hit, RAM_ACK, ADR_SI, ADR_VALID_SI, RAM_DATA)

variable current_adr_tag    :   std_logic_vector((N_BITS_TAG - 1) downto 0);
variable current_adr_index  :   std_logic_vector((N_BITS_WAYS - 1) downto 0);
variable current_adr_offset :   std_logic_vector((N_BITS_OFFSET - 1) downto 0);

variable cpt : integer;

begin 
    RAM_ADR_VALID <= '0';
    case EP is 
        when idle =>
            if ADR_VALID_SI = '1' and reset_n = '1' and hit = '0' then 
                EF <= wait_mem;
                
                -- align address then send it to the RAM
                RAM_ADR(31 downto 4)    <=  ADR_SI(31 downto 4);
                RAM_ADR(3 downto 0)     <=  "0000";
                RAM_ADR_VALID           <=  '1';
                
                current_adr_tag         :=  ADR_SI(31 downto (N_BITS_WAYS + N_BITS_OFFSET));
                current_adr_index       :=  ADR_SI((N_BITS_WAYS + N_BITS_OFFSET - 1) downto N_BITS_OFFSET);
                
                cpt := 0;
            else
                EF <= idle;
            end if;
        when wait_mem =>
            if RAM_ACK = '1' then 
                EF <= update; 

                if cpt = 0 then 
                    data0(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA; 
                elsif cpt = 1 then 
                    data1(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA;
                elsif cpt = 2 then 
                    data2(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA;
                elsif cpt = 3 then 
                    data3(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA;
                else 
                    report "cpt 103 > 3" severity error; 
                end if; 

                tags(to_integer(unsigned(current_adr_index)))       <=  current_adr_tag; 
                data_valid(to_integer(unsigned(current_adr_index))) <=  '0';  
            else
                EF  <=  wait_mem; 
            end if;
        when update =>      
            if RAM_ACK = '0' then 
                EF <= idle; 
                data_valid(to_integer(unsigned(current_adr_index))) <=  '1';  
            elsif rising_edge(clk) then -- temp solution because we send 1 word/cycle
                                        -- basically, we need to find a proper way to still increment cpt event if RAM_DATA didn't change
                                        -- (we can have to consecutive same words)
                EF <= update;
                if cpt = 0 then 
                    data0(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA; 
                elsif cpt = 1 then 
                    data1(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA;
                elsif cpt = 2 then 
                    data2(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA;
                elsif cpt = 3 then 
                    data3(to_integer(unsigned(current_adr_index)))  <=  RAM_DATA;
                else 
                    report "cpt 125 > 3" severity error; 
                end if; 
                cpt := cpt + 1;
            end if;  
    end case; 
    dbg_cpt <= std_logic_vector(to_signed(cpt, 4));
end process; 

dbg_st <= "00" when EP = idle else
          "01" when EP = wait_mem else
          "10" when EP = update else
          "11";

end archi;