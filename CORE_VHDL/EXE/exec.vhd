library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity exec is 
    port(
        -- global interface 
        clk, reset_n : in std_logic;

        OP1_RD, OP2_RD : in std_logic_vector(31 downto 0);
        RADR1_RD, RADR2_RD : in std_logic_vector(5 downto 0);
        MEM_DATA_RD : in std_logic_vector(31 downto 0);
        DEST_RD : in std_logic_vector(5 downto 0);
        CMD_RD : in std_logic_vector(1 downto 0);
        MEM_SIZE_RD : in std_logic_vector(1 downto 0);
        NEG_OP2_RD : in std_logic;
        WB_RD : in std_logic;
        MEM_SIGN_EXTEND_RD : in std_logic;
        SELECT_SHIFT_RD : in std_logic;
        MEM_LOAD_RD, MEM_STORE_RD : in std_logic;
        EXE2MEM_POP_SM : in std_logic;
        DEC2EXE_EMPTY_SD : in std_logic;
        SLT_RD, SLTU_RD : in std_logic;

        RES_RE : out std_logic_vector(31 downto 0);    
        MEM_DATA_RE : out std_logic_vector(31 downto 0);
        DEST_RE : out std_logic_vector(5 downto 0);
        MEM_SIZE_RE : out std_logic_vector(1 downto 0);
        WB_RE : out std_logic;
        MEM_SIGN_EXTEND_RE : out std_logic;
        MEM_LOAD_RE, MEM_STORE_RE : out std_logic;
        EXE2MEM_EMPTY_SE : out std_logic;
        DEC2EXE_POP_SE : out std_logic; 

        -- bypasses 
        BLOCK_BP_RD : in std_logic;
        MEM_DEST_RM : in std_logic_vector(5 downto 0); 
        MEM_RES_RM : in std_logic_vector(31 downto 0)
    );
end exec; 

architecture archi of exec is 
signal op1, op2 : std_logic_vector(31 downto 0);
signal alu_op2 : std_logic_vector(31 downto 0);
signal alu_res : std_logic_vector(31 downto 0);

signal shifter_res : std_logic_vector(31 downto 0);

signal exe_res : std_logic_vector(31 downto 0); 

signal exe2mem_push : std_logic;
signal exe2mem_full : std_logic;
signal exe2mem_empty : std_logic;

signal slt_res, sltu_res : std_logic_vector(31 downto 0);

signal stall_se : std_logic; 
signal blocked_se : std_logic;
signal bp_mem_data : std_logic_vector(31 downto 0);
signal r1_valid_se, r2_valid_se : std_logic; 

-- exe output readable 
signal exe_fifo_res : std_logic_vector(31 downto 0); 
signal exe_fifo_dest : std_logic_vector(5 downto 0);
signal exe_fifo_mem_load : std_logic;

component alu
    port(
        OP1_SE, OP2_SE : in std_logic_vector(31 downto 0);
        CIN_SE : in std_logic;
        CMD_SE : in std_logic_vector(1 downto 0);
        RES_SE : out std_logic_vector(31 downto 0)
    );
end component; 

component shifter 
    port(
        DIN_SE : in std_logic_vector(31 downto 0);
        SHIFT_VAL_SE : in std_logic_vector(4 downto 0);
        CMD_SE : in std_logic_vector(1 downto 0);
        DOUT_SE : out std_logic_vector(31 downto 0)
    );
end component; 

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

begin 

--  Instances
alu_i : alu
    port map(
        OP1_SE => op1,
        OP2_SE => alu_op2,
        CIN_SE => NEG_OP2_RD,
        CMD_SE => CMD_RD,
        RES_SE => alu_res 
    );

shifter_i : shifter
    port map(
        DIN_SE => OP1, 
        SHIFT_VAL_SE => OP2(4 downto 0),
        CMD_SE => CMD_RD, 
        DOUT_SE => shifter_res
    );

exe2mem : fifo
    generic map(N => 76)
    port map(
        clk => clk,
        reset_n => reset_n,

        DIN(31 downto 0) => exe_res,
        DIN(63 downto 32) => bp_mem_data,
        DIN(69 downto 64) => DEST_RD, 
        DIN(71 downto 70) => MEM_SIZE_RD, 
        DIN(72) => WB_RD,
        DIN(73) => MEM_LOAD_RD, 
        DIN(74) => MEM_STORE_RD, 
        DIN(75) => MEM_SIGN_EXTEND_RD,

        PUSH => exe2mem_push,
        POP => EXE2MEM_POP_SM, 
        FULL => exe2mem_full,
        EMPTY => exe2mem_empty, 

        DOUT(31 downto 0) => exe_fifo_res,
        DOUT(63 downto 32) => MEM_DATA_RE, 
        DOUT(69 downto 64) => exe_fifo_dest, 
        DOUT(71 downto 70) => MEM_SIZE_RE,
        DOUT(72) => WB_RE, 
        DOUT(73) => exe_fifo_mem_load,
        DOUT(74) => MEM_STORE_RE, 
        DOUT(75) => MEM_SIGN_EXTEND_RE
    );

-- ALU OP2 selection 
alu_op2 <= not op2 when NEG_OP2_RD = '1' else op2; 

-- SLT / SLTU oper 
slt_res <=  x"00000001" when (op1(31) = '1' and op2(31) = '0') else 
            x"00000000" when (op1(31) = '0' and op2(31) = '1') else
            x"00000000" when (op1 = op2)                       else
            (x"0000000"&"000"&(alu_res(31)));

sltu_res <= x"00000000" when (op1(31) = '1' and op2(31) = '0') else
            x"00000001" when (op1(31) = '0' and op2(31) = '1') else
            x"00000000" when (op1 = op2)                       else
            (x"0000000"&"000"&(alu_res(31)));

-- exe result selection
exe_res <=  shifter_res when SELECT_SHIFT_RD = '1' else 
            slt_res when SLT_RD = '1' else
            sltu_res when SLTU_RD = '1' else
            alu_res;

-- fifo 
stall_se <= exe2mem_full or DEC2EXE_EMPTY_SD or blocked_se or not(r1_valid_se) or not(r2_valid_se);
exe2mem_push <= not stall_se; 
DEC2EXE_POP_SE <= not stall_se;

-- Bypasses 
op1 <=  OP1_RD when RADR1_RD = "000000" or BLOCK_BP_RD = '1' else 
        exe_fifo_res when exe_fifo_dest = RADR1_RD and exe_fifo_mem_load = '0' else 
        MEM_RES_RM when exe_fifo_dest = RADR1_RD else 
        OP1_RD;

r1_valid_se <= '1'; 

op2 <=  OP2_RD when (RADR2_RD = "000000" or MEM_LOAD_RD = '1' or BLOCK_BP_RD = '1') else
        OP2_RD when MEM_DEST_RM = RADR2_RD and MEM_STORE_RD = '1' else 
        MEM_RES_RM when MEM_DEST_RM = RADR2_RD and MEM_STORE_RD = '0' else 
        OP2_RD;
-- need to verify this bypass
--exe_fifo_res when exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '0' else

r2_valid_se <= '1';

bp_mem_data <=  exe_fifo_res when (exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '0') else 
                MEM_RES_RM when (MEM_DEST_RM = RADR2_RD and MEM_STORE_RD = '1') else
                MEM_DATA_RD;

blocked_se <=   '1' when ((exe_fifo_dest = RADR1_RD and exe_fifo_mem_load = '1' and exe2mem_empty = '0') 
                    or (exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '1' and exe2mem_empty = '0')) else 
                '0';

-- Output
RES_RE <= exe_fifo_res;
DEST_RE <= exe_fifo_dest; 
EXE2MEM_EMPTY_SE <= exe2mem_empty;
MEM_LOAD_RE <= exe_fifo_mem_load; 

end archi;