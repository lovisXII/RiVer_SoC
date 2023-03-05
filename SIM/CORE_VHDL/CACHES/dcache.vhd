library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dcache is
	port(
        -- global interface
        clk, reset_n    :   in  std_logic;

        --core interface
        ADR_SM          :   in  std_logic_vector(31 downto 0);
        DATA_SM          :   in  std_logic_vector(31 downto 0);
        ADR_VALID_SM    :   in  std_logic; 	--not sure if useful, as we have load and store. If we combine them, then useful
        LOAD_SM			:   in  std_logic;
        STORE_SM		:   in  std_logic;

        DATA_SC			:	out 	std_logic_vector(31 downto 0);
        STALL_SC		:	out 	std_logic;


        -- buffer cache interface



        --bus wrapper interface

            --read

            RAM_ADR : out std_logic_vector(31 downto 0);
            RAM_ADR_VALID          : out  std_logic;

            RAM_ACK      : in std_logic;
            RAM_DATA     : in std_logic_vector(31 downto 0);

            --write

            RAM_WRITE_ADR   : out std_logic_vector(31 downto 0);
            RAM_WRITE_DATA  : out std_logic_vector(31 downto 0);
            RAM_WRITE_SIZE  : out std_logic_vector(1  downto 0);
            RAM_STORE       : out std_logic;

            RAM_BUFFER_CACHE_POP : in std_logic








	);
end dcache;


architecture archi of dcache is

constant LINES : integer := 128;
constant WIDTH : integer := 4;


type tag_tab is array(0 to (LINES - 1)) of std_logic_vector(20 downto 0);

type data_t is array (0 to (WIDTH - 1)) of std_logic_vector(31 downto 0);
type ways_t is array (0 to (LINES - 1)) of data_t;


signal w0_tags: tag_tab;
signal w0_data : ways_t;
signal w0_data_valid : std_logic_vector((LINES - 1) downto 0);

signal w1_tags: tag_tab;
signal w1_data : ways_t;
signal w1_data_valid : std_logic_vector((LINES - 1)  downto 0);

signal lru_tab: std_logic_vector((LINES - 1) downto 0);

signal adr_tag      :   std_logic_vector(20 downto 0);
signal adr_index    :   std_logic_vector(6 downto 0);
signal adr_offset   :   std_logic_vector(3 downto 0);


signal hit_w0: std_logic;
signal hit_w1: std_logic;

signal hit: std_logic;

signal w0_data_res: std_logic_vector(31 downto 0);
signal w1_data_res: std_logic_vector(31 downto 0);
signal data_res: std_logic_vector(31 downto 0);

component buffer_cache
    port(        clk, reset_n : in std_logic;

        -- fifo commands & status
        PUSH, POP : in std_logic;
        EMPTY, FULL : out std_logic;

        -- fifo input data
        DATA_C : in std_logic_vector(31 downto 0);
        ADR_C : in std_logic_vector(31 downto 0);
        STORE_C, LOAD_C : in std_logic; -- might be simplified to a single signal
        SIZE_C : in std_logic_vector(1 downto 0); -- 0b10 for byte  
                                                  -- 0b01 for short
                                                  -- 0b00 for word

        -- output
        DATA_BC : out std_logic_vector(31 downto 0);
        ADR_BC : out std_logic_vector(31 downto 0);
        STORE_BC, LOAD_BC : out std_logic; 
        SIZE_BC : out std_logic_vector(1 downto 0)
    );
end component;


signal buffer_PUSH, buffer_POP : std_logic;
signal buffer_EMPTY, buffer_FULL : std_logic;

        -- fifo input data
signal buffer_DATA_C : std_logic_vector(31 downto 0);
signal buffer_ADR_C : std_logic_vector(31 downto 0);
signal buffer_STORE_C, buffer_LOAD_C : std_logic;
signal buffer_SIZE_C : std_logic_vector(1 downto 0); -- 0b10 for byte  
                                                  -- 0b01 for short
                                                  -- 0b00 for word

        -- fifo output data
signal buffer_DATA_BC : std_logic_vector(31 downto 0);
signal buffer_ADR_BC :  std_logic_vector(31 downto 0);
signal buffer_STORE_BC, buffer_LOAD_BC : std_logic; 
signal buffer_SIZE_BC : std_logic_vector(1 downto 0);



--state machine
type state is (idle, wait_mem);
signal EP, EF: state;



begin

--buffer cache to ram/wrapper

buffer_cache_inst: buffer_cache port map(clk,reset_n,buffer_PUSH, buffer_POP, buffer_EMPTY, buffer_FULL, 
    buffer_DATA_C, buffer_ADR_C, buffer_STORE_C, buffer_LOAD_C, buffer_SIZE_C, 
    buffer_DATA_BC, buffer_ADR_BC, buffer_STORE_BC, buffer_LOAD_BC, buffer_SIZE_BC);

RAM_WRITE_ADR  <= buffer_ADR_BC;
RAM_WRITE_DATA <= buffer_DATA_BC;
RAM_WRITE_SIZE <= buffer_SIZE_BC;
RAM_STORE      <= not buffer_EMPTY;

buffer_POP     <= RAM_BUFFER_CACHE_POP;

-- read to ram

-- align address then send it to the RAM
RAM_ADR(31 downto 4)    <=  ADR_SM(31 downto 4);
RAM_ADR(3 downto 0)     <=  "0000";

    -- RAM_ADR_VALID set in the fsm

--


adr_tag     <=  ADR_SM(31 downto 11); 
adr_index   <=  ADR_SM(10 downto 4);
adr_offset  <=  ADR_SM(3 downto 0);


hit_w0 <= '1' when ( (w0_tags(to_integer(unsigned(adr_index))) = adr_tag) and (w0_data_valid(to_integer(unsigned(adr_index)))) = '1') else
		  '0';

hit_w1 <= '1' when ( (w1_tags(to_integer(unsigned(adr_index))) = adr_tag) and (w1_data_valid(to_integer(unsigned(adr_index)))) = '1') else
		  '0';

hit <= hit_w0 or hit_w1;


w0_data_res  <=  w0_data(to_integer(unsigned(adr_index)))(to_integer(unsigned(adr_offset)));

--w0_data0(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "00" else 
--                w0_data1(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "01" else
--                w0_data2(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "10" else
--                w0_data3(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "11" else
--                x"00000000";

w1_data_res  <=  w1_data(to_integer(unsigned(adr_index)))(to_integer(unsigned(adr_offset)));


--w1_data_res  <=  w1_data0(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "00" else 
                --w1_data1(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "01" else
                --w1_data2(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "10" else
                --w1_data3(to_integer(unsigned(adr_index)))  when adr_offset(3 downto 2) = "11" else
                --x"00000000";

data_res <= w0_data_res when hit_w0 = '1' else
			w1_data_res when hit_w1 = '1' else
			x"00000000";

DATA_SC <= data_res;

STALL_SC <= not hit when LOAD_SM = '1' else
			buffer_FULL when STORE_SM = '1' else 
			'0'; --value doesn't matter if the proc doesn't ask anything to the cache.

fsm_transition : process(clk, reset_n)
begin  
    if reset_n = '0' then 
        EP  <=  idle; 
    elsif rising_edge(clk) then 
        EP  <=  EF; 
    end if; 
end process; 


fsm_output: process(clk, EP, hit,ADR_VALID_SM,RAM_ACK,RAM_DATA)
variable cpt : integer;
begin
    RAM_ADR_VALID <=  '0';
	case EP is
		when idle =>
			if LOAD_SM = '1' and ADR_VALID_SM = '1'then -- read
				if (hit = '0') then

                    cpt := 0;
					EF <= wait_mem;

                    RAM_ADR_VALID <=  '1';



				else
                    if hit_w0 = '1' then
                        lru_tab(to_integer(unsigned(adr_index))) <= '1';
                    else
                        lru_tab(to_integer(unsigned(adr_index))) <= '0';
                    end if;
					EF <= idle;

                    RAM_ADR_VALID <= '0';

				end if;
			else --on a write, we always stay idle
				EF <= idle;
			end if;

		when wait_mem =>
            if RAM_ACK = '1' then

                if lru_tab(to_integer(unsigned(adr_index))) = '0' then
                    w0_data((to_integer(unsigned(adr_index))))(cpt) <= RAM_DATA;
                else
                    w1_data((to_integer(unsigned(adr_index))))(cpt) <= RAM_DATA;
                end if;

                cpt := cpt + 1;

                if cpt < WIDTH then
                    EF <= wait_mem;
                else
                    EF <= idle;
                    

                    if lru_tab(to_integer(unsigned(adr_index))) = '0' then
                        W0_tags(to_integer(unsigned(adr_index))) <= adr_tag;
                        w0_data_valid(to_integer(unsigned(adr_index))) <= '1';
                        lru_tab(to_integer(unsigned(adr_index))) <= '1';
                    else
                        W1_tags(to_integer(unsigned(adr_index))) <= adr_tag;
                        w1_data_valid(to_integer(unsigned(adr_index))) <= '1';
                        lru_tab(to_integer(unsigned(adr_index))) <= '0';
                    end if;

                end if;
            else
                EF <= wait_mem;
            end if;


	end case;
end process;




end architecture;