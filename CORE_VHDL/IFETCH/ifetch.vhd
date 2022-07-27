library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all; 

entity ifetch is 
    port(
        -- global interface
        clk, reset_n        :   in  std_logic;

        -- Icache interface
        IC_INST_SI          :   in  std_logic_vector(31 downto 0);
        IC_STALL_SI         :   in  std_logic;
        ADR_SI              :   out std_logic_vector(31 downto 0);
        ADR_VALID_SI        :   out std_logic;

        -- dec2if interface 
        DEC2IF_EMPTY_SD     :   in  std_logic;
        DEC2IF_POP_SI       :   out std_logic;

        -- if2dec interface 
        IF2DEC_FLUSH_SD     :   in  std_logic;
        IF2DEC_POP_SD       :   in  std_logic; 
        IF2DEC_EMPTY_SI     :   out std_logic; 

        PC_RD               :   in  std_logic_vector(31 downto 0);
        INSTR_RI            :   out std_logic_vector(31 downto 0);
        PC_IF2DEC_RI        :   out std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_SM        :   in  std_logic; 
        EXCEPTION_RI        :   out std_logic;

        -- Branch prediction 
        PRED_FAILED_RD      :   in  std_logic;
        PRED_SUCCESS_RD     :   in  std_logic;
        BRANCH_INST_RD      :   in  std_logic;
        BRANCH_INST_ADR_RD  :   in  std_logic_vector(31 downto 0);
        ADR_TO_BRANCH_RD    :   in  std_logic_vector(31 downto 0);

        PRED_ADR_SD         :   in  std_logic_vector(31 downto 0);
        PRED_TAKEN_SD       :   in  std_logic;

        PUSH_ADR_RAS_RD     :   in  std_logic;
        POP_ADR_RAS_RD      :   in  std_logic;
        ADR_TO_RET_RD       :   in  std_logic_vector(31 downto 0);
        RET_INST_RD         :   in  std_logic;

        PRED_ADR_RI         :   out std_logic_vector(31 downto 0);
        PRED_TAKEN_RI       :   out std_logic
    );
end ifetch;

architecture archi of ifetch is 
signal if2dec_push_si, if2dec_full_si, if2dec_empty : std_logic;
signal stall_si : std_logic;

component fifo
    generic(N : integer);
    port(
        clk     : in    std_logic; 
        reset_n : in    std_logic; 
        DIN     : in    std_logic_vector(N-1 downto 0);
        PUSH    : in    std_logic;
        POP     : in    std_logic;
        FULL    : out   std_logic;
        EMPTY   : out   std_logic;
        DOUT    : out   std_logic_vector(N-1 downto 0)
    );
end component;

constant nop_i : std_logic_vector(31 downto 0) := x"00000013"; 
signal if2dec_din, if2dec_dout : std_logic_vector(96 downto 0);

-- Branch prediction
type pred_state is (strongly_taken, weakly_taken, weakly_not_taken, strongly_not_taken);
signal next_pred_state : pred_state; 

type pred_reg_t is array (0 to PRED_REG_SIZE-1) of std_logic_vector(31 downto 0);
signal branch_adr_reg, predicted_adr_reg : pred_reg_t;

type pred_state_t is array (0 to PRED_REG_SIZE-1) of pred_state;
signal pred_state_reg : pred_state_t; 

signal pred_valid_reg : std_logic_vector(PRED_REG_SIZE-1 downto 0);

signal pred_next_adr_si : std_logic_vector(31 downto 0) := x"11111100";

signal pred_write_pointer_si : std_logic_vector(PRED_POINTER_SIZE-1 downto 0);

-- Ret prediction 
type ret_adr_reg_t is array (0 to RET_PRED_REG_SIZE-1) of std_logic_vector(31 downto 0);
signal ret_adr_reg : ret_adr_reg_t;

type ret_stack_reg_t is array (0 to RET_STACK_SIZE-1) of std_logic_vector(31 downto 0);
signal ret_stack_reg : ret_stack_reg_t; 

signal ret_valid_reg : std_logic_vector(RET_PRED_REG_SIZE-1 downto 0);

signal ret_write_pointer_si : std_logic_vector(RET_PRED_POINTER_SIZE-1 downto 0);
signal pred_branch_next_adr : std_logic_vector(31 downto 0) := x"22222200";
signal ret_stack_pointer_si : std_logic_vector(RET_STACK_SIZE-1 downto 0);

signal pred_ret_next_adr : std_logic_vector(31 downto 0) := x"33333300"; 

signal pred_branch_taken, pred_ret_taken : std_logic := '0'; 

begin 

-- Instanciation 
if2dec : fifo
    generic map(N => 97)
    port map(
        clk                 =>  clk,
        reset_n             =>  reset_n,
        DIN                 =>  if2dec_din, 
        PUSH                =>  if2dec_push_si, 
        POP                 =>  IF2DEC_POP_SD,
        FULL                =>  if2dec_full_si, 
        EMPTY               =>  if2dec_empty, 
        DOUT                =>  if2dec_dout
    );

stall_si <= IC_STALL_SI or if2dec_full_si or DEC2IF_EMPTY_SD;

if2dec_push_si  <= not stall_si when (IF2DEC_FLUSH_SD = '0') else '0'; 
DEC2IF_POP_SI   <= not stall_si when IF2DEC_FLUSH_SD = '0' else '1';
ADR_VALID_SI    <= not DEC2IF_EMPTY_SD when (IF2DEC_FLUSH_SD = '0' and EXCEPTION_SM /= '1') else '0';

ADR_SI <=   PRED_ADR_SD when    PRED_TAKEN_SD = '1' and PRED_FAILED_RD = '0'    else 
            PC_RD;
      

----------------------
-- Branch prediction
----------------------
pred_state_updt : process(clk, reset_n)
begin
    if reset_n = '0' then 
        next_pred_state <= weakly_taken; 
    elsif rising_edge(clk) then 
        if PRED_FAILED_RD = '1' or PRED_SUCCESS_RD = '1' then 
            l0 : for i in 0 to PRED_REG_SIZE-1 loop
                if branch_adr_reg(i) /= x"00000000" and BRANCH_INST_ADR_RD /= x"00000000" then -- ????????? 
                    case pred_state_reg(i) is 
                        when strongly_taken     =>  next_pred_state <=  strongly_taken;
                            if PRED_SUCCESS_RD  = '0' then 
                                next_pred_state <=  weakly_taken;
                            end if; 
                        
                        when weakly_taken       =>  next_pred_state <=  strongly_taken;
                            if PRED_SUCCESS_RD  = '0' then 
                                next_pred_state <=  weakly_not_taken;
                            end if; 

                        when weakly_not_taken   =>  next_pred_state <=  weakly_taken; 
                            if PRED_SUCCESS_RD  = '0' then 
                                next_pred_state <=  strongly_not_taken; 
                            end if; 

                        when strongly_not_taken =>  next_pred_state <=  weakly_not_taken; 
                            if PRED_SUCCESS_RD  = '0' then 
                                next_pred_state <=  strongly_not_taken;
                            end if; 
                    end case; 
                end if; 
            end loop; 
        end if; 
    end if;
end process;

pred_state_write_reg : process(clk, reset_n)
variable index : integer range 0 to PRED_REG_SIZE; 
variable found : std_logic; 

variable pred_write_pointer : std_logic_vector(PRED_POINTER_SIZE-1 downto 0);

begin 
    index := 0;
    if reset_n = '0' then 
        pred_write_pointer  := (others => '0');
        pred_branch_taken   <= '0'; 
    elsif falling_edge(clk) then 
        if BRANCH_INST_RD = '1' and if2dec_empty = '0' then 
            found := '0'; 
            l0 : for i in 0 to PRED_REG_SIZE-1 loop 
                if branch_adr_reg(i) = BRANCH_INST_ADR_RD then 
                    if found = '0' then 
                        index := i;
                    end if; 
                    found := '1'; 
                end if; 
            end loop;   

            if found = '0' then 
                branch_adr_reg(to_integer(unsigned(pred_write_pointer)))      <=  BRANCH_INST_ADR_RD;
                predicted_adr_reg(to_integer(unsigned(pred_write_pointer)))   <=  ADR_TO_BRANCH_RD;
                pred_state_reg(to_integer(unsigned(pred_write_pointer)))      <=  weakly_taken;
                pred_valid_reg(to_integer(unsigned(pred_write_pointer)))      <=  '1';
                pred_write_pointer                                          :=  std_logic_vector(unsigned(pred_write_pointer) + unsigned(one_ext_pred_size));
            else 
                PRED_STATE_REG(index)                   <=  next_pred_state;
            end if; 

            pred_write_pointer_si                       <=  pred_write_pointer;

        end if; 
    end if;
end process;

read_pred_reg : process(PC_RD)
variable found : std_logic;
begin 
    found := '0';
    search_branch : for i in 0 to PRED_REG_SIZE-1 loop 
        if branch_adr_reg(i) = PC_RD and pred_valid_reg(i) = '1' then 
            found := '1';
            pred_branch_next_adr    <=  predicted_adr_reg(i);
            if pred_state_reg(i) = strongly_taken or pred_state_reg(i) = weakly_taken then 
                pred_branch_taken <= '1'; 
            else 
                pred_branch_taken <= '0';
            end if; 
        end if; 
    end loop; 
    if found = '0' then 
        pred_branch_taken <= '0';
    end if;
end process;

----------------------
-- Ret prediction
----------------------
write_pred_ret : process(clk, reset_n)
variable found : std_logic; 
begin 
    if reset_n = '0' then 
        ret_write_pointer_si    <=  one_ext_ret_size;
    elsif falling_edge(clk) then 
        found := '0';
        if RET_INST_RD = '1' and if2dec_empty = '0' then 
            l0 : for i in 0 to RET_PRED_REG_SIZE-1 loop
                if ret_adr_reg(i) = BRANCH_INST_ADR_RD then 
                    found := '1';
                end if; 
            end loop;
            if found = '0' then 
                ret_adr_reg(to_integer(unsigned(ret_write_pointer_si)))     <=  BRANCH_INST_ADR_RD;
                ret_valid_reg(to_integer(unsigned(ret_write_pointer_si)))   <=  '1'; 
                ret_write_pointer_si                                        <=  ret_write_pointer_si(RET_PRED_POINTER_SIZE-2 downto 1) & '0'; 
            end if; 
        end if; 
    end if; 
end process; 

read_pred_ret : process(clk, reset_n)
variable found : std_logic;
variable ret_stack_pointer : std_logic_vector(RET_STACK_SIZE-1 downto 0);
begin
    if reset_n = '0' then 
        pred_ret_taken          <=  '0';
        ret_stack_pointer       :=  one_ext_ret_stack_size;

    elsif falling_edge(clk) then 
        ret_stack_pointer   :=  ret_stack_pointer_si;
        search_ret : for i in 0 to RET_PRED_REG_SIZE-1 loop
            if ret_adr_reg(i) = PC_RD and ret_valid_reg(i) = '1' then 
                found := '1'; 
            end if; 
        end loop;
        pred_ret_taken  <=  found; 

        if if2dec_empty = '0' then 
            if PUSH_ADR_RAS_RD = '1' then 
                ret_inst_search : for i in 0 to RET_PRED_REG_SIZE-1 loop
                    if ret_stack_pointer(i) = '1' then
                        ret_stack_reg(i) <= ADR_TO_RET_RD;
                    end if;
                end loop;
                ret_stack_pointer   :=  ret_stack_pointer(RET_PRED_REG_SIZE-2 downto 1) & '0'; 
            elsif POP_ADR_RAS_RD = '1' then 
                ret_stack_pointer :=    '0' & ret_stack_pointer(RET_STACK_SIZE-1 downto 1);
            end if; 

            if found = '1' then 
                found_ret : for i in 0 to RET_STACK_SIZE-2 loop
                    if ret_stack_pointer(i+1) = '1' then 
                        pred_ret_next_adr <= ret_stack_reg(i);
                    end if; 
                end loop;
                ret_stack_pointer :=    '0' & ret_stack_pointer(RET_PRED_REG_SIZE-1 downto 1);
            end if; 
        end if;  
    end if;
    ret_stack_pointer_si <= ret_stack_pointer;
end process; 

EXCEPTION_RI <= '0'; 
IF2DEC_EMPTY_SI <= if2dec_empty; 

----------------------
-- fifo
----------------------
-- Input
if2dec_din(31 downto 0)     <=  PRED_ADR_SD when    PRED_TAKEN_SD = '1' and PRED_FAILED_RD = '0' else 
                                PC_RD;
if2dec_din(63 downto 32)    <=  IC_INST_SI  when    EXCEPTION_SM = '0'  else
                                nop_i;
if2dec_din(95 downto 64)    <=  pred_branch_next_adr    when    pred_branch_taken = '1' else
                                pred_ret_next_adr       when    pred_ret_taken = '1'    else
                                x"44444400"; 
                
if2dec_din(96)              <=  pred_branch_taken or pred_ret_taken;

-- Ouput
PC_IF2DEC_RI                <=  if2dec_dout(31 downto 0);
INSTR_RI                    <=  if2dec_dout(63 downto 32);
PRED_ADR_RI                 <=  if2dec_dout(95 downto 64);
PRED_TAKEN_RI               <=  if2dec_dout(96);

end archi;