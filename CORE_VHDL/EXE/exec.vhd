library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all;

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

        PC_DEC2EXE_RD : in std_logic_vector(31 downto 0);
        PC_EXE2MEM_RE : out std_logic_vector(31 downto 0);

        -- bypasses 
        BLOCK_BP_RD : in std_logic;
        MEM_DEST_RM : in std_logic_vector(5 downto 0); 
        MEM_RES_RM : in std_logic_vector(31 downto 0);
        CSR_WENABLE_RM : in std_logic;
        CSR_RDATA_RM : in std_logic_vector(31 downto 0);

        -- CSR 
        CSR_WENABLE_RD : in std_logic;
        CSR_WADR_RD   : in std_logic_vector(11 downto 0);
        CSR_RDATA_RD  : in std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_SM    : in std_logic;
        EXCEPTION_RD    : in std_logic;
        CURRENT_MODE_SM : in std_logic_vector(1 downto 0);
        PC_BRANCH_VALUE_RD : in std_logic_vector(31 downto 0);
        PC_BRANCH_VALUE_RE : out std_logic_vector(31 downto 0);

        ILLEGAL_INSTRUCTION_RD, ADRESS_MISALIGNED_RD, INSTRUCTION_ACCESS_FAULT_RD : in std_logic; 
        ENV_CALL_U_MODE_RD, ENV_CALL_S_MODE_RD, ENV_CALL_M_MODE_RD : in std_logic;
        ENV_CALL_WRONG_MODE_RD : in std_logic;
        MRET_RD : in std_logic;
        EBREAK_RD : in std_logic;

        EXCEPTION_RE    : out std_logic;
        ILLEGAL_INSTRUCTION_RE, ADRESS_MISALIGNED_RE, INSTRUCTION_ACCESS_FAULT_RE : out std_logic; 
        ENV_CALL_U_MODE_RE, ENV_CALL_S_MODE_RE, ENV_CALL_M_MODE_RE : out std_logic;
        ENV_CALL_WRONG_MODE_RE : out std_logic;
        LOAD_ADRESS_MISALIGNED_RE, LOAD_ACCESS_FAULT_RE : out std_logic;
        STORE_ADRESS_MISALIGNED_RE, STORE_ACCESS_FAULT_RE : out std_logic;
        MRET_RE : out std_logic;
        EBREAK_RE : out std_logic;

        CSR_WENABLE_RE  : out std_logic;
        CSR_WADR_RE     : out std_logic_vector(11 downto 0);
        CSR_RDATA_RE    : out std_logic_vector(31 downto 0)
    );
end exec; 

architecture archi of exec is 

signal exe2mem_data, exe2mem_x : std_logic_vector(198 downto 0);
signal exe2mem_din, exe2mem_dout : std_logic_vector(198 downto 0);

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

-- bypass 
signal bpc_ee1, bpc_me1 : std_logic; 
signal bpc_ee2, bpc_me2 : std_logic; 
signal bpc_disable1, bpc_disable2 : std_logic; 

-- exception 
signal adress_misaligned : std_logic := '0';
signal load_adress_misaligned_se : std_logic := '0';
signal store_adress_misaligned_se : std_logic := '0';
signal access_fault : std_logic := '0';
signal load_access_fault_se : std_logic := '0';
signal store_access_fault_se : std_logic := '0';
signal exception_se : std_logic := '0';

-- exe output readable 
signal exe_fifo_res : std_logic_vector(31 downto 0); 
signal exe_fifo_dest : std_logic_vector(5 downto 0);
signal exe_fifo_mem_store, exe_fifo_mem_load : std_logic;
signal exe_fifo_wb : std_logic; 

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
    generic map(N => 199)
    port map(
        clk => clk,
        reset_n => reset_n,
        DIN => exe2mem_din,
        PUSH => exe2mem_push,
        POP => EXE2MEM_POP_SM, 
        FULL => exe2mem_full,
        EMPTY => exe2mem_empty, 
        DOUT => exe2mem_dout 
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
r1_valid_se <= '1'; 
r2_valid_se <= '1';

bpc_disable1    <= '1' when RADR1_RD = "000000" or BLOCK_BP_RD = '1' else '0'; 
bpc_disable2    <= '1' when RADR2_RD = "000000" or BLOCK_BP_RD = '1' or MEM_LOAD_RD = '1' else '0'; 

bpc_ee1 <=  '1' when exe_fifo_dest = RADR1_RD and exe_fifo_mem_load = '0' and bpc_disable1 = '0' else '0';
bpc_me1 <=  '1' when MEM_DEST_RM = RADR1_RD and bpc_disable1 = '0' else '0'; 


bpc_ee2 <=  '1' when exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '0' and bpc_disable2 = '0' else '0';
bpc_me2 <=  '1' when MEM_DEST_RM = RADR2_RD and bpc_disable2 = '0' else '0'; 


--op1     <=  exe_fifo_res    when bpc_ee1 = '1' else 
--            MEM_RES_RM      when bpc_me1 = '1' else 
--            OP1_RD; 
--
--op2     <=  exe_fifo_res    when bpc_ee2 = '1' and MEM_STORE_RD = '0' else 
--            MEM_RES_RM      when bpc_me2 = '1' and MEM_STORE_RD = '0' else 
--            OP2_RD; 


--
--bp_mem_data     <=  exe_fifo_res    when bpc_ee2 = '1' and MEM_STORE_RD = '1' else 
--                    MEM_RES_RM      when bpc_me2 = '1' and MEM_STORE_RD = '1' else
--                    MEM_DATA_RD;                     
--



bp_mem_data <=  exe_fifo_res when (exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '0' and bpc_disable2 = '0') else 
                MEM_RES_RM when (MEM_DEST_RM = RADR2_RD and MEM_STORE_RD = '1' and bpc_disable2 = '0') else
                MEM_DATA_RD;

op1 <=  OP1_RD when RADR1_RD = "000000" or BLOCK_BP_RD = '1' else 
        exe_fifo_res when exe_fifo_dest = RADR1_RD and exe_fifo_mem_load = '0' else 
        MEM_RES_RM when MEM_DEST_RM = RADR1_RD else 
        OP1_RD;

op2 <=  OP2_RD when (RADR2_RD = "000000" or MEM_LOAD_RD = '1' or BLOCK_BP_RD = '1') else
        exe_fifo_res when exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '0' and MEM_STORE_RD = '0' else
        OP2_RD when MEM_DEST_RM = RADR2_RD and MEM_STORE_RD = '1' else 
        MEM_RES_RM when MEM_DEST_RM = RADR2_RD and MEM_STORE_RD = '0' else 
        OP2_RD;
-- need to verify this bypass
--exe_fifo_res when exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '0' else


blocked_se <=   '1' when ((exe_fifo_dest = RADR1_RD and exe_fifo_mem_load = '1' and exe2mem_empty = '0') 
                    or (exe_fifo_dest = RADR2_RD and exe_fifo_mem_load = '1' and exe2mem_empty = '0')) else 
                '0';

-- Exception 
adress_misaligned <=    '1' when   (alu_res(1 downto 0) = "11" and MEM_SIZE_RD = "00") 
                                or  (alu_res(0) = '1' and MEM_SIZE_RD = "01")
                            else 
                        '0'; 

load_adress_misaligned_se <= MEM_LOAD_RD and adress_misaligned; 
store_adress_misaligned_se <= MEM_STORE_RD and adress_misaligned;  

access_fault <= '1' when alu_res > kernel_adr and CURRENT_MODE_SM = "00" else '0'; 
load_access_fault_se <= MEM_LOAD_RD and access_fault; 
store_access_fault_se <= MEM_STORE_RD and access_fault; 

exception_se <= (EXCEPTION_RD or load_adress_misaligned_se or load_access_fault_se or store_access_fault_se or store_adress_misaligned_se) and reset_n;

-- Output
RES_RE <= exe_fifo_res;
DEST_RE <= exe_fifo_dest; 
EXE2MEM_EMPTY_SE <= exe2mem_empty;
MEM_LOAD_RE <= exe_fifo_mem_load when exception_se = '0' else '0'; 
MEM_STORE_RE <= exe_fifo_mem_store when exception_se = '0' else '0'; 
WB_RE <= exe_fifo_wb when exception_se = '0' else '0'; 

-- fifo 
exe2mem_data(31 downto 0) <= exe_res;
exe2mem_data(63 downto 32) <= bp_mem_data;
exe2mem_data(69 downto 64) <= DEST_RD;
exe2mem_data(71 downto 70) <= MEM_SIZE_RD; 
exe2mem_data(72) <= WB_RD;
exe2mem_data(73) <= MEM_LOAD_RD; 
exe2mem_data(74) <= MEM_STORE_RD; 
exe2mem_data(75) <= MEM_SIGN_EXTEND_RD;
exe2mem_data(107 downto 76) <= PC_DEC2EXE_RD; 
exe2mem_data(108) <= CSR_WENABLE_RD;
exe2mem_data(120 downto 109) <= CSR_WADR_RD;
exe2mem_data(152 downto 121) <= CSR_RDATA_RD; 
exe2mem_data(153) <= ENV_CALL_S_MODE_RD; 
exe2mem_data(154) <= ENV_CALL_WRONG_MODE_RD; 
exe2mem_data(155) <= ILLEGAL_INSTRUCTION_RD; 
exe2mem_data(156) <= ADRESS_MISALIGNED_RD; 
exe2mem_data(157) <= ENV_CALL_U_MODE_RD;
exe2mem_data(158) <= ENV_CALL_M_MODE_RD;
exe2mem_data(159) <= exception_se; 
exe2mem_data(160) <= load_adress_misaligned_se; 
exe2mem_data(161) <= load_access_fault_se; 
exe2mem_data(162) <= MRET_RD; 
exe2mem_data(163) <= store_adress_misaligned_se; 
exe2mem_data(164) <= store_access_fault_se; 
exe2mem_data(165) <= INSTRUCTION_ACCESS_FAULT_RD; 
exe2mem_data(166) <= EBREAK_RD; 
exe2mem_data(198 downto 167) <= PC_BRANCH_VALUE_RD; 

exe2mem_x(75 downto 0) <= (others => '0');
exe2mem_x(107 downto 76) <= PC_DEC2EXE_RD; 
exe2mem_x(198 downto 108) <= (others => '0');

exe2mem_din <=  exe2mem_data when EXCEPTION_SM = '0' else 
                exe2mem_x; 

exe_fifo_res <= exe2mem_dout(31 downto 0);
MEM_DATA_RE <= exe2mem_dout(63 downto 32); 
exe_fifo_dest <= exe2mem_dout(69 downto 64);  
MEM_SIZE_RE <= exe2mem_dout(71 downto 70); 
exe_fifo_wb <= exe2mem_dout(72);   
exe_fifo_mem_load <= exe2mem_dout(73);   
exe_fifo_mem_store <= exe2mem_dout(74);   
MEM_SIGN_EXTEND_RE <= exe2mem_dout(75);   
PC_EXE2MEM_RE <= exe2mem_dout(107 downto 76);
CSR_WENABLE_RE <= exe2mem_dout(108);
CSR_WADR_RE <= exe2mem_dout(120 downto 109);
CSR_RDATA_RE <= exe2mem_dout(152 downto 121);
ENV_CALL_S_MODE_RE <= exe2mem_dout(153);
ENV_CALL_WRONG_MODE_RE <= exe2mem_dout(154);
ILLEGAL_INSTRUCTION_RE <= exe2mem_dout(155);
ADRESS_MISALIGNED_RE <= exe2mem_dout(156);
ENV_CALL_U_MODE_RE <= exe2mem_dout(157);
ENV_CALL_M_MODE_RE <= exe2mem_dout(158);
EXCEPTION_RE <= exe2mem_dout(159);
LOAD_ADRESS_MISALIGNED_RE <= exe2mem_dout(160);
LOAD_ACCESS_FAULT_RE <= exe2mem_dout(161);
MRET_RE <= exe2mem_dout(162);
STORE_ADRESS_MISALIGNED_RE <= exe2mem_dout(163);
STORE_ACCESS_FAULT_RE <= exe2mem_dout(164);
INSTRUCTION_ACCESS_FAULT_RE <= exe2mem_dout(165);
EBREAK_RE <= exe2mem_dout(166);
PC_BRANCH_VALUE_RE <= exe2mem_dout(198 downto 167);


end archi;