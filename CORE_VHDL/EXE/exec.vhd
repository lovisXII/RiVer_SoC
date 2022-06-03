library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
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

        EXE_RES_RE : out std_logic_vector(31 downto 0);    
        MEM_DATA_RE : out std_logic_vector(31 downto 0);
        DEST_RE : out std_logic_vector(5 downto 0);
        MEM_SIZE_RE : out std_logic_vector(1 downto 0);
        WB_RE : out std_logic;
        MEM_SIGN_EXTEND_RE : out std_logic;
        MEM_LOAD_RE, MEM_STORE_RE : out std_logic;
        EXE2MEM_EMPTY_SE : out std_logic;
        DEC2EXE_POP_SE : out std_logic
    );
end exec; 

architecture archi of exec is 
signal op1, op2 : std_logic_vector(31 downto 0);
signal alu_op2 : std_logic_vector(31 downto 0);
signal alu_res : std_logic_vector(31 downto 0);

signal shifter_res : std_logic_vector(31 downto 0);

signal exe_res : std_logic_vector(31 downto 0); 
signal exe_fifo_res : std_logic_vector(31 downto 0); -- exe result signal readable for bypass

signal exe2mem_push : std_logic;
signal exe2mem_full : std_logic;

signal fifo_stall : std_logic;

signal slt_res, sltu_res : std_logic_vector(31 downto 0);

signal blocked : std_logic;
signal bp_mem_data : std_logic_vector(31 downto 0);
begin 

--  Instances
alu_i : entity work.alu 
    port map(
        OP1_SE => op1,
        OP2_SE => alu_op2,
        CIN_SE => NEG_OP2_RD,
        CMD_SE => CMD_RD,
        RES_SE => alu_res 
    );

shifter_i : entity work.shifter
    port map(
        DIN_SE => OP1, 
        SHIFT_VAL_SE => OP2(4 downto 0),
        CMD_SE => CMD_RD, 
        DOUT_SE => shifter_res
    );

exe2mem : entity work.fifo 
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
        EMPTY => EXE2MEM_EMPTY_SE, 

        DOUT(31 downto 0) => exe_fifo_res,
        DOUT(63 downto 32) => MEM_DATA_RE, 
        DOUT(69 downto 64) => DEST_RE, 
        DOUT(71 downto 70) => MEM_SIZE_RE,
        DOUT(72) => WB_RE, 
        DOUT(73) => MEM_LOAD_RE,
        DOUT(74) => MEM_STORE_RE, 
        DOUT(75) => MEM_SIGN_EXTEND_RE
    );

-- ALU OP2 selection 
alu_op2 <= not op2 when NEG_OP2_RD = '1' else op2; 

-- SLT / SLTU operations 
slt_res <=  x"00000000" when (op1(31) = '1' and op2(31) = '0') else 
            x"00000001" when (op1(31) = '0' and op2(31) = '1') else
            (x"0000000"&"000"&not(alu_res(31)));

sltu_res <= x"00000001" when (op1(31) = '1' and op2(31) = '0') else
            x"00000000" when (op1(31) = '0' and op2(31) = '1') else
            (x"0000000"&"000"&not(alu_res(31)));

-- exe result selection
exe_res <=  shifter_res when SELECT_SHIFT_RD = '1' else 
            slt_res when SLT_RD = '1' else
            sltu_res when SLTU_RD = '1' else
            alu_res;

-- fifo 
exe2mem_push <= not (exe2mem_full or DEC2EXE_EMPTY_SD or blocked);
DEC2EXE_POP_SE <= not (exe2mem_full or DEC2EXE_EMPTY_SD or blocked);

-- Bypasses not implemented yet
op1 <= OP1_RD;
op2 <= OP2_RD;
bp_mem_data <= x"00000000";
blocked <= '0';

-- Output
EXE_RES_RE <= exe_fifo_res;

end archi;