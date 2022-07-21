library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work; 
use work.util.all; 

entity dec is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Reg interface
        RDATA1_SR, RDATA2_SR : in std_logic_vector(31 downto 0);
        REG_ADR1_SD, REG_ADR2_SD : out std_logic_vector(5 downto 0);
        WRITE_PC_SD : out std_logic_vector(31 downto 0);
        WRITE_PC_ENABLE_SD : out std_logic;
        READ_PC_SR : in std_logic_vector(31 downto 0);

        -- Exe interface 
        OP1_RD, OP2_RD : out std_logic_vector(31 downto 0);
        CMD_RD : out std_logic_vector(1 downto 0);
        NEG_OP2_RD : out std_logic; 
        WB_RD : out std_logic;
        DEST_RD : out std_logic_vector(5 downto 0);
        SELECT_OPERATION_RD : out std_logic_vector(3 downto 0);
        SLT_RD, SLTU_RD : out std_logic;    
        MEM_DATA_RD : out std_logic_vector(31 downto 0);
        MEM_LOAD_RD, MEM_STORE_RD, MEM_SIGN_EXTEND_RD : out std_logic;
        MEM_SIZE_RD : out std_logic_vector(1 downto 0);

        PC_DEC2EXE_RD : out std_logic_vector(31 downto 0);
        PC_BRANCH_VALUE_RD : out std_logic_vector(31 downto 0);

        CSR_WENABLE_RD  : out std_logic; 
        CSR_WADR_RD     : out std_logic_vector(11 downto 0);
        CSR_RDATA_RD    : out std_logic_vector(31 downto 0);

        -- dec2if interface
        DEC2IF_POP_SI : in std_logic; 
        DEC2IF_EMPTY_SD : out std_logic;
        PC_RD : out std_logic_vector(31 downto 0);

        -- if2dec interface
        INSTR_RI, PC_IF2DEC_RI : in std_logic_vector(31 downto 0);
        IF2DEC_EMPTY_SI : in std_logic;
        IF2DEC_POP_SD : out std_logic;
        IF2DEC_FLUSH_SD : out std_logic;

        -- dec2exe interface
        DEC2EXE_POP_SE : in std_logic;
        DEC2EXE_EMPTY_SD : out std_logic;

        -- Multiplier
        MULT_INST_RD    :   out std_logic;
        MULT_INST_RE    :   in  std_logic;
        MULT_INST_RM    :   in  std_logic;

        -- Bypasses
        BP_DEST_RE : in std_logic_vector(5 downto 0);
        BP_EXE_RES_RE : in std_logic_vector(31 downto 0);
        BP_MEM_LOAD_RE : in std_logic;
        BP_EXE2MEM_EMPTY_SE, BP_MEM2WBK_EMPTY_SM : in std_logic;
        BP_DEST_RM : in std_logic_vector(5 downto 0);
        BP_MEM_RES_RM : in std_logic_vector(31 downto 0);
        BP_R1_VALID_RD, BP_R2_VALID_RD : out std_logic;
        BP_RADR1_RD, BP_RADR2_RD : out std_logic_vector(5 downto 0);    
        BLOCK_BP_RD : out std_logic;

        CSR_WENABLE_RE, CSR_WENABLE_RM : in std_logic;
        CSR_RDATA_RE, CSR_RDATA_RM : in std_logic_vector(31 downto 0);

        CSR_RADR_SD : out std_logic_vector(11 downto 0);
        CSR_RDATA_SC : in std_logic_vector(31 downto 0);

        -- Exception 
        EXCEPTION_RI : in std_logic;
        ILLEGAL_INSTRUCTION_RD : out std_logic;
        ADRESS_MISALIGNED_RD : out std_logic; 
        ENV_CALL_U_MODE_RD, ENV_CALL_M_MODE_RD, ENV_CALL_S_MODE_RD : out std_logic; 
        ENV_CALL_WRONG_MODE_RD : out std_logic;
        INSTRUCTION_ACCESS_FAULT_RD : out std_logic;
        MRET_RD : out std_logic;
        EXCEPTION_RD : out std_logic;
        EBREAK_RD : out std_logic; 
        
        CURRENT_MODE_SM : in std_logic_vector(1 downto 0);
        EXCEPTION_SM    : in std_logic;
        MTVEC_VALUE_RC  : in std_logic_vector(31 downto 0);
        MCAUSE_WDATA_SM : in std_logic_vector(31 downto 0);
        MRET_SM         : in std_logic;
        RETURN_ADRESS_SM : in std_logic_vector(31 downto 0)

    );
end dec;

architecture archi of dec is 

constant inc_value : std_logic_vector(31 downto 0) := x"00000004";

signal reset_sync_sd : std_logic := '0';
signal resetting_sd : std_logic := '0' ; 
-- fifo 
signal dec2if_din, dec2if_dout : std_logic_vector(31 downto 0);
signal dec2if_full_sd, dec2if_push_sd : std_logic;

signal dec2exe_din, dec2exe_data, dec2exe_x, dec2exe_dout : std_logic_vector(251 downto 0);
signal dec2exe_full_sd, dec2exe_push_sd : std_logic;

-- Instructions
signal r_type_sd, i_type_sd, s_type_sd, b_type_sd, u_type_sd, j_type_sd, jalr_type_sd : std_logic;
signal load_type_sd, nmem_type_sd : std_logic;
signal m_type_sd : std_logic;

signal add_i_sd, sub_i_sd, slt_i_sd, sltu_i_sd, and_i_sd, or_i_sd, xor_i_sd, sll_i_sd, srl_i_sd, sra_i_sd : std_logic;
signal addi_i_sd, slti_i_sd, sltiu_i_sd, andi_i_sd, ori_i_sd, xori_i_sd, slli_i_sd, srli_i_sd, srai_i_sd : std_logic;

signal lw_i_sd, lh_i_sd, lhu_i_sd, lb_i_sd, lbu_i_sd, sw_i_sd, sh_i_sd, sb_i_sd : std_logic; 

signal beq_i_sd, bne_i_sd, blt_i_sd, bge_i_sd, bltu_i_sd, bgeu_i_sd : std_logic;

signal lui_i_sd : std_logic;
signal auipc_i_sd : std_logic;

signal j_i_sd, jalr_i_sd : std_logic;

signal mul_i_sd, mulh_i_sd, mulhsu_i_sd, mulhu_i_sd : std_logic;
signal div_i_sd, divu_i_sd, rem_i_sd, remu_i_sd : std_logic;

-- System instructions
signal system_inst_sd : std_logic;
signal ecall_i_sd : std_logic;
signal ebreak_i_sd : std_logic := '0';
signal csrrw_i_sd, csrrs_i_sd, csrrc_i_sd : std_logic;
signal csrrwi_i_sd, csrrsi_i_sd, csrrci_i_sd : std_logic;
signal sret_i_sd : std_logic;
signal mret_i_sd : std_logic := '0';
signal csr_in_progress : std_logic;

signal fence_i_sd : std_logic;

signal env_call_u_mode_sd : std_logic := '0'; 
signal env_call_s_mode_sd : std_logic := '0'; 
signal env_call_m_mode_sd : std_logic := '0';
signal env_call_wrong_mode : std_logic := '0';

signal exception_sd : std_logic := '0';

signal csr_wenable_sd : std_logic;

signal illegal_inst : std_logic := '0'; 
signal illegal_inst_sd : std_logic := '0';
signal instruction_access_fault_sd : std_logic := '0';
signal instruction_adress_misaligned_sd : std_logic := '0'; 
signal instruction_adress_fault_sd : std_logic := '0';

signal op1_csri_type_sd : std_logic_vector(31 downto 0); 

signal mtvec_value, mcause_val : std_logic_vector(31 downto 0);

signal csr_radr : std_logic_vector(11 downto 0);

-- dec2exe data and commands
signal dec2exe_op1_sd, dec2exe_op2_sd, op1_u_type_sd, op2_i_type_sd, op2_s_type_sd : std_logic_vector(31 downto 0); 
signal radr1_sd, radr2_sd, rdest_sd : std_logic_vector(5 downto 0);
signal rdata1_sd, rdata2_sd : std_logic_vector(31 downto 0);
signal neg_op2_sd : std_logic;
signal exe_cmd_sd : std_logic_vector(1 downto 0);
signal select_operation_sd : std_logic_vector(3 downto 0);
signal mem_data_sd : std_logic_vector(31 downto 0);
signal mem_load_sd,mem_store_sd, mem_sign_extend_sd : std_logic;
signal mem_size_sd : std_logic_vector(1 downto 0);
signal wb_sd : std_logic;

-- branch and pc gestion
signal offset_branch_sd, offset_branch_j, offset_branch_jalr, jalr_offset, offset_branch_b : std_logic_vector(31 downto 0);
signal jalr_offset_calc : std_logic_vector(31 downto 0);
signal jump_sd : std_logic;
signal different_sign : std_logic;
signal res : std_logic_vector(31 downto 0);
signal res_compare : std_logic_vector(31 downto 0);
signal add_offset_to_pc : std_logic;
signal pc : std_logic_vector(31 downto 0) := x"00000000";
signal init_pc : std_logic_vector(31 downto 0); 
signal new_pc : std_logic_vector(31 downto 0);
signal pc_branch_value_sd : std_logic_vector(31 downto 0);

-- bypass
signal stall_sd, block_in_dec : std_logic;
signal r1_valid_sd, r2_valid_sd : std_logic;
signal block_bp_sd : std_logic;
signal mult_dependency : std_logic;

-- Bypass Conditions 
signal bpc_instr_in_exe1, bpc_load_in_mem1, bpc_ed1, bpc_md1 : std_logic;  
signal bpc_instr_in_exe2, bpc_load_in_mem2, bpc_ed2, bpc_md2 : std_logic;  
signal bpc_mult_exe1, bpc_mult_exe2, bpc_mult_mem1, bpc_mult_mem2 : std_logic;

-- readable signals (output ports)
signal mem_load_fifo : std_logic;
signal dec2exe_empty : std_logic;
signal dec_fifo_rdest : std_logic_vector(5 downto 0);
signal csr_wenable_fifo : std_logic;
signal dec_fifo_mult_inst : std_logic;

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

-------------------------
-- Instanciation 
-------------------------
dec2if : fifo
    generic map(N => 32)
    port map(
        clk => clk, 
        reset_n => reset_n,
        DIN => dec2if_din, 
        PUSH => dec2if_push_sd, 
        POP => DEC2IF_POP_SI, 
        FULL => dec2if_full_sd, 
        EMPTY => DEC2IF_EMPTY_SD, 
        DOUT => dec2if_dout
    );

dec2exe : fifo
    generic map(N => 252)
    port map(
        clk => clk, 
        reset_n => reset_n, 
        DIN => dec2exe_din, 
        PUSH => dec2exe_push_sd, 
        POP => DEC2EXE_POP_SE, 
        FULL => dec2exe_full_sd, 
        EMPTY => dec2exe_empty, 
        DOUT => dec2exe_dout 
    );

-------------------------
-- fifo gestion 
-------------------------
csr_in_progress <=  '1' when     (csr_wenable_fifo = '1' and dec2exe_empty = '0') 
                            or  (CSR_WENABLE_RE = '1' and BP_EXE2MEM_EMPTY_SE = '0') else 
                    '0';

stall_sd    <=  '1' when    (
                                (csr_in_progress = '1' or (
                                    (r1_valid_sd = '0' or r2_valid_sd = '0') 
                                    and 
                                    (b_type_sd = '1' or jalr_type_sd = '1' or j_type_sd = '1' or block_in_dec = '1' or mult_dependency = '1'))
                                ) 
                                or 
                                IF2DEC_EMPTY_SI = '1' or dec2exe_full_sd = '1'
                            )
                    else 
                '0'; 

-- if2dec 
IF2DEC_POP_SD   <=  not stall_sd; 

IF2DEC_FLUSH_SD <=  '1' when jump_sd = '1' and stall_sd = '0' else  
                    '0'; 

dec2if_push_sd  <=  '1' when    ((add_offset_to_pc = '0' and dec2if_full_sd = '0') 
                            or   (add_offset_to_pc = '1' and dec2if_full_sd = '0' and stall_sd = '0')) else 
                    '0';  

-- dec2exe
dec2exe_push_sd <=  not stall_sd; 

-------------------------
-- Instruction type 
-------------------------
r_type_sd       <= '1' when INSTR_RI(6 downto 0) = "0110011" else '0';
i_type_sd       <= '1' when (INSTR_RI(6 downto 0) = "0010011" or INSTR_RI(6 downto 0) = "0000011") else '0';
s_type_sd       <= '1' when INSTR_RI(6 downto 0) = "0100011" else '0';
b_type_sd       <= '1' when INSTR_RI(6 downto 0) = "1100011" else '0';
u_type_sd       <= '1' when INSTR_RI(6 downto 0) = "0110111" or INSTR_RI(6 downto 0) = "0010111" else '0';
j_type_sd       <= '1' when INSTR_RI(6 downto 0) = "1101111" else '0';
jalr_type_sd    <= '1' when INSTR_RI(6 downto 0) = "1100111" else '0';
load_type_sd    <= '1' when INSTR_RI(6 downto 0) = "0000011" else '0';
nmem_type_sd    <= '1' when INSTR_RI(6 downto 0) = "0010011" else '0'; -- not mem type but i type
system_inst_sd  <= '1' when INSTR_RI(6 downto 0) = "1110011" else '0'; 
m_type_sd       <= '1' when INSTR_RI(6 downto 0) = "0110011" and INSTR_RI(31 downto 25) = "0000001" else '0'; 

-------------------------
-- Instruction decoding  
-------------------------
-- R Type
add_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "000" and (INSTR_RI(30) = '0') else '0';
sub_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "000" and (INSTR_RI(30) = '1') else '0';
slt_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "010" and (INSTR_RI(30) = '0') else '0';
sltu_i_sd <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "011" and (INSTR_RI(30) = '0') else '0';
and_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "111" and (INSTR_RI(30) = '0') else '0';
or_i_sd   <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "110" and (INSTR_RI(30) = '0') else '0';
xor_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "100" and (INSTR_RI(30) = '0') else '0';
sll_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "001" and (INSTR_RI(30) = '0') else '0';
srl_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "101" and (INSTR_RI(30) = '0') else '0';
sra_i_sd  <= '1' when r_type_sd = '1' and INSTR_RI(14 downto 12) = "101" and (INSTR_RI(30) = '1') else '0';

-- I type
addi_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "000" else '0';
slti_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "010" else '0';
sltiu_i_sd <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "011" else '0';
andi_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "111" else '0';
ori_i_sd   <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "110" else '0';
xori_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "100" else '0';
slli_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0';
srli_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "101" and INSTR_RI(30) = '0' else '0';
srai_i_sd  <= '1' when nmem_type_sd = '1' and INSTR_RI(14 downto 12) = "101" and INSTR_RI(30) = '1' else '0';

-- B type_sd
beq_i_sd  <= '1' when b_type_sd = '1' and INSTR_RI(14 downto 12) = "000" else '0';
bne_i_sd  <= '1' when b_type_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0';
blt_i_sd  <= '1' when b_type_sd = '1' and INSTR_RI(14 downto 12) = "100" else '0';
bge_i_sd  <= '1' when b_type_sd = '1' and INSTR_RI(14 downto 12) = "101" else '0';
bltu_i_sd <= '1' when b_type_sd = '1' and INSTR_RI(14 downto 12) = "110" else '0';
bgeu_i_sd <= '1' when b_type_sd = '1' and INSTR_RI(14 downto 12) = "111" else '0';

-- U type 
lui_i_sd <= u_type_sd;
auipc_i_sd <= '1' when INSTR_RI(6 downto 0) = "0010111" else '0';

-- J type 
j_i_sd <= j_type_sd; 
jalr_i_sd <= jalr_type_sd;

-- Mem access 
lw_i_sd  <= '1' when load_type_sd = '1' and INSTR_RI(14 downto 12) = "010" else '0';
lh_i_sd  <= '1' when load_type_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0';
lhu_i_sd <= '1' when load_type_sd = '1' and INSTR_RI(14 downto 12) = "101" else '0';
lb_i_sd  <= '1' when load_type_sd = '1' and INSTR_RI(14 downto 12) = "000" else '0';
lbu_i_sd <= '1' when load_type_sd = '1' and INSTR_RI(14 downto 12) = "100" else '0'; 

sw_i_sd <= '1' when s_type_sd = '1' and INSTR_RI(14 downto 12) = "010" else '0';
sh_i_sd <= '1' when s_type_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0';
sb_i_sd <= '1' when s_type_sd = '1' and INSTR_RI(14 downto 12) = "000" else '0';


-- M type 
mul_i_sd    <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "000" else '0'; 
mulh_i_sd   <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0'; 
mulhsu_i_sd <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "010" else '0';
mulhu_i_sd  <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "011" else '0';
div_i_sd    <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "100" else '0';  
divu_i_sd   <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "101" else '0'; 
rem_i_sd    <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "110" else '0'; 
remu_i_sd   <=  '1' when m_type_sd = '1' and INSTR_RI(14 downto 12) = "111" else '0'; 

-- System type 
ecall_i_sd  <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "000" and INSTR_RI(31 downto 20) = x"000" else '0';
ebreak_i_sd <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "000" and INSTR_RI(31 downto 20) = x"001" else '0';

csrrw_i_sd  <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0';
csrrs_i_sd  <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "010" else '0';
csrrc_i_sd  <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "011" else '0';
csrrwi_i_sd <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "101" else '0';
csrrsi_i_sd <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "110" else '0';
csrrci_i_sd <=  '1' when system_inst_sd = '1' and INSTR_RI(14 downto 12) = "111" else '0';

mret_i_sd   <=  '1' when INSTR_RI = x"30200073" else '0'; 
sret_i_sd   <=  '1' when INSTR_RI = x"10200073" else '0'; 

fence_i_sd  <=  '1' when INSTR_RI(6 downto 0) = "0001111" and INSTR_RI(14 downto 12) = "000" else '0'; 



illegal_inst    <=  not (add_i_sd or sub_i_sd or slt_i_sd or sltu_i_sd or and_i_sd or or_i_sd or xor_i_sd or sll_i_sd or srl_i_sd or sra_i_sd or
                    addi_i_sd or slti_i_sd or sltiu_i_sd or andi_i_sd or ori_i_sd or xori_i_sd or
                    slli_i_sd or srli_i_sd or srai_i_sd or
                    beq_i_sd or bne_i_sd or blt_i_sd or bge_i_sd or bltu_i_sd or bgeu_i_sd or lui_i_sd or auipc_i_sd or j_i_sd or jalr_i_sd or 
                    lw_i_sd or lh_i_sd or lhu_i_sd or lb_i_sd or lbu_i_sd or sw_i_sd or sh_i_sd or sb_i_sd or 
                    mul_i_sd or mulh_i_sd or mulhu_i_sd or mulhsu_i_sd or 
                    ecall_i_sd or ebreak_i_sd or csrrw_i_sd or csrrs_i_sd or csrrc_i_sd or csrrwi_i_sd or csrrsi_i_sd or csrrci_i_sd or mret_i_sd or sret_i_sd or fence_i_sd);

illegal_inst_sd <=  illegal_inst and not(IF2DEC_EMPTY_SI);

-- Env call  
env_call_u_mode_sd  <= '1' when CURRENT_MODE_SM = "00" and ecall_i_sd = '1' else '0';  
env_call_s_mode_sd  <= '1' when CURRENT_MODE_SM /= "10" and sret_i_sd = '1' else '0'; 
env_call_m_mode_sd  <= '1' when CURRENT_MODE_SM = "11" and ecall_i_sd = '1' else '0'; 

env_call_wrong_mode <= '1' when CURRENT_MODE_SM /= "11" and mret_i_sd = '1' else '0'; 

------------------------------------
-- Registers and operands selection
------------------------------------
-- Registers affectation 
radr1_sd <= '0'&INSTR_RI(19 downto 15) when ((r_type_sd or i_type_sd or s_type_sd or b_type_sd or jalr_type_sd or m_type_sd) = '1') 
                                        or  ((csrrw_i_sd or csrrs_i_sd or csrrc_i_sd) = '1') else
            "000000";

radr2_sd <= '0'&INSTR_RI(24 downto 20) when ((r_type_sd or s_type_sd or b_type_sd or m_type_sd) = '1') else
            "000000";

rdest_sd <= '0'&INSTR_RI(11 downto 7) when  ((r_type_sd or i_type_sd or u_type_sd or j_type_sd or jalr_type_sd or m_type_sd) = '1') 
                                        or  ((csrrw_i_sd or csrrs_i_sd or csrrc_i_sd or csrrwi_i_sd or csrrsi_i_sd or csrrci_i_sd) = '1') else
            "000000";

csr_radr <=  INSTR_RI(31 downto 20) when system_inst_sd = '1' and ((csrrw_i_sd or csrrs_i_sd or csrrc_i_sd or csrrwi_i_sd or csrrsi_i_sd or csrrci_i_sd) = '1') else 
                x"000"; 

-- Operand 1 selection
op1_u_type_sd(31 downto 12) <= INSTR_RI(31 downto 12);
op1_u_type_sd(11 downto 0)  <= x"000";

op1_csri_type_sd(31 downto 5)   <= (others => '0'); 
op1_csri_type_sd(4 downto 0)    <= INSTR_RI(19 downto 15);

dec2exe_op1_sd <=   rdata1_sd               when ((r_type_sd or i_type_sd or s_type_sd or b_type_sd or csrrw_i_sd or csrrs_i_sd or m_type_sd) = '1') else 
                    not(rdata1_sd)          when csrrc_i_sd = '1' else 
                    op1_csri_type_sd        when ((csrrwi_i_sd or csrrsi_i_sd) = '1') else
                    not(op1_csri_type_sd)   when csrrci_i_sd = '1' else 
                    op1_u_type_sd           when u_type_sd = '1' else 
                    READ_PC_SR              when ((j_type_sd or jalr_type_sd) = '1') else 
                    x"00000000";

-- Operand 2 selection
op2_i_type_sd(31 downto 12) <=  (others => INSTR_RI(31));                            
op2_i_type_sd(11 downto 0)  <= INSTR_RI(31 downto 20);

op2_s_type_sd(31 downto 12) <=  (others => INSTR_RI(31));
op2_s_type_sd(11 downto 5)  <= INSTR_RI(31 downto 25);
op2_s_type_sd(4 downto 0)   <= INSTR_RI(11 downto 7);

dec2exe_op2_sd <=   rdata2_sd       when ((r_type_sd  or b_type_sd or (u_type_sd and not(auipc_i_sd)) or m_type_sd) = '1') else 
                    CSR_RDATA_SC    when ((csrrs_i_sd or csrrc_i_sd or csrrsi_i_sd or csrrci_i_sd) = '1') else
                    op2_i_type_sd   when i_type_sd = '1' else
                    op2_s_type_sd   when s_type_sd = '1' else
                    PC_IF2DEC_RI    when auipc_i_sd = '1' else 
                    x"00000000";

-------------------------
-- Exec commands  
-------------------------
-- neg
neg_op2_sd <= sub_i_sd or slt_i_sd or slti_i_sd or sltu_i_sd or sltiu_i_sd; 
-- alu 
exe_cmd_sd <=   "01" when ((and_i_sd or andi_i_sd or srl_i_sd or srli_i_sd or csrrc_i_sd or csrrci_i_sd or mul_i_sd or div_i_sd) = '1') else 
                "10" when ((or_i_sd or ori_i_sd or sra_i_sd or srai_i_sd or csrrs_i_sd or csrrsi_i_sd or mulh_i_sd or divu_i_sd) = '1') else
                "11" when ((xor_i_sd or xori_i_sd or mulhu_i_sd or rem_i_sd) = '1') else 
                "00";

select_operation_sd <=  "1000"  when    ((div_i_sd or divu_i_sd or rem_i_sd or remu_i_sd) = '1') else 
                        "0100"  when    ((mul_i_sd or mulh_i_sd or mulhsu_i_sd or mulhu_i_sd) = '1')                        else 
                        "0010"  when    ((sll_i_sd or slli_i_sd or srl_i_sd or srli_i_sd or sra_i_sd or srai_i_sd) = '1')   else 
                        "0001";

wb_sd <=    r_type_sd or i_type_sd or u_type_sd or b_type_sd or j_type_sd or jalr_type_sd or 
            csrrw_i_sd or csrrs_i_sd or csrrc_i_sd or csrrwi_i_sd or csrrsi_i_sd or csrrci_i_sd;

mem_data_sd <= rdata2_sd when s_type_sd = '1' else 
               x"00000000";
mem_load_sd <= lw_i_sd or lh_i_sd or lhu_i_sd or lb_i_sd or lbu_i_sd;

mem_store_sd <= sw_i_sd or sh_i_sd or sb_i_sd;

mem_size_sd <=  "00" when ((lw_i_sd or sw_i_sd)= '1') else              -- word size 
                "01" when ((lh_i_sd or lhu_i_sd or sh_i_sd) = '1') else -- halfword size
                "10" when ((lb_i_sd or lbu_i_sd or sb_i_sd) = '1') else -- byte size
                "11";                                                   -- not a mem access
 
mem_sign_extend_sd <= lh_i_sd or lb_i_sd; 


csr_wenable_sd  <= csrrw_i_sd or csrrs_i_sd or csrrc_i_sd or csrrwi_i_sd or csrrsi_i_sd or csrrci_i_sd; 

-------------------------
-- Branch offset
-------------------------
offset_branch_b(31 downto 12)   <=  (others => INSTR_RI(31)); 
offset_branch_b(11)             <=  INSTR_RI(7);
offset_branch_b(10 downto 5)    <=  INSTR_RI(30 downto 25);
offset_branch_b(4 downto 1)     <=  INSTR_RI(11 downto 8);
offset_branch_b(0)              <=  '0';

offset_branch_j(31 downto 20)   <=  (others => INSTR_RI(31));
offset_branch_j(19 downto 12)   <=  INSTR_RI(19 downto 12);
offset_branch_j(11)             <=  INSTR_RI(20);
offset_branch_j(10 downto 1)    <=  INSTR_RI(30 downto 21);
offset_branch_j(0)              <=  '0';

jalr_offset(31 downto 12)       <=  (others => INSTR_RI(31));
jalr_offset(11 downto 0)        <=  INSTR_RI(31 downto 20);

jalr_offset_calc    <=  std_logic_vector(signed(jalr_offset) + signed(rdata1_sd) - signed(READ_PC_SR) + signed(inc_value)); 

offset_branch_jalr(31 downto 1) <=  jalr_offset_calc(31 downto 1);
offset_branch_jalr(0)           <=  '0';

offset_branch_sd    <=  offset_branch_b when b_type_sd = '1' else 
                        offset_branch_j when j_type_sd = '1' else
                        offset_branch_jalr when jalr_type_sd = '1' else 
                        x"00000000";


res <= dec2exe_op1_sd xor dec2exe_op2_sd; 
res_compare <= std_logic_vector(signed(dec2exe_op1_sd) - signed(dec2exe_op2_sd));
different_sign <= dec2exe_op1_sd(31) xor dec2exe_op2_sd(31) ;

jump_sd <=  '1' when b_type_sd = '1'    and (   (bne_i_sd = '1' and (res /= x"00000000")) 
                                            
                                            or  (beq_i_sd = '1' and (res = x"00000000"))
                                              
                                            or  (blt_i_sd = '1' and ((different_sign = '1' and  dec2exe_op1_sd(31) = '1') 
                                                    or (different_sign = '0' and res_compare(31) = '1')))
                                              
                                            or  (bltu_i_sd = '1' and ((different_sign = '1' and  dec2exe_op2_sd(31) = '1') 
                                                    or (different_sign = '0' and res_compare(31) = '1')))

                                            or  (bge_i_sd = '1' and ((different_sign = '1' and  dec2exe_op2_sd(31) = '1') 
                                                    or (different_sign = '0' and res_compare(31) = '0'))) 

                                            or  (bgeu_i_sd = '1' and ((different_sign = '1' and  dec2exe_op1_sd(31) = '1') 
                                                    or (different_sign = '0' and res_compare(31) = '0'))))
                else 
            (j_type_sd or jalr_type_sd);
                

add_offset_to_pc <= jump_sd and not(IF2DEC_EMPTY_SI);

-- PC 
WRITE_PC_ENABLE_SD  <=  '1' when    ((add_offset_to_pc = '0' and dec2if_full_sd = '0') 
                                or   (add_offset_to_pc = '1' and dec2if_full_sd = '0' and stall_sd = '0')) else 
                        '0';  

-- initialize pc, maybe not the best way to do it, but it works...
process(clk, reset_n)
begin 
    if rising_edge(clk) then 
        if reset_n = '1' then 
            reset_sync_sd <= '1'; 
        end if;
    end if; 
end process; 

resetting_sd <= reset_sync_sd xor reset_n; 

pc  <=  READ_PC_SR when resetting_sd = '1' else
        std_logic_vector(unsigned(READ_PC_SR) + unsigned(inc_value)) when add_offset_to_pc = '0' and dec2if_full_sd = '0' and reset_n = '1' else 
        std_logic_vector(unsigned(PC_IF2DEC_RI) + unsigned(offset_branch_sd)) when add_offset_to_pc = '1' and dec2if_full_sd = '0' and stall_sd = '0' and reset_n = '1'else 
        x"00000000"; 

instruction_access_fault_sd <= '1' when EXCEPTION_SM = '0' and CURRENT_MODE_SM /= "11" and pc > kernel_adr else '0'; 
instruction_adress_misaligned_sd <= '1' when pc(1 downto 0) /= "00" or (RETURN_ADRESS_SM(1 downto 0) /= "00" and EXCEPTION_SM = '1') else '0';  

mtvec_value(31 downto 1)    <= MTVEC_VALUE_RC(31 downto 1);
mtvec_value(1 downto 0)     <= "00"; 

mcause_val(31 downto 2)     <= MCAUSE_WDATA_SM(29 downto 0);
mcause_val(1 downto 0)      <= "00";

new_pc  <=  MTVEC_VALUE_RC when MRET_SM = '0' and EXCEPTION_SM = '1' and MTVEC_VALUE_RC(1 downto 0) = "00" else 
            std_logic_vector(unsigned(mtvec_value) + unsigned(mcause_val)) when MRET_SM = '0' and EXCEPTION_SM = '1' and MTVEC_VALUE_RC(1 downto 0) = "01" else  
            RETURN_ADRESS_SM when MRET_SM = '1' and EXCEPTION_SM = '1' else 
            pc; 

pc_branch_value_sd <= new_pc; 

WRITE_PC_SD <= new_pc; 

-------------------------
-- Bypass
-------------------------
block_in_dec <= '1' when (((radr1_sd = dec_fifo_rdest) or (radr2_sd = dec_fifo_rdest)) and mem_load_fifo = '1' and dec2exe_empty = '0') else '0';
block_bp_sd <= jalr_type_sd;      

mult_dependency     <= '1' when (   (radr1_sd = dec_fifo_rdest and dec_fifo_mult_inst = '1' and dec2exe_empty = '0')    or 
                                    (radr2_sd = dec_fifo_rdest and dec_fifo_mult_inst = '1' and dec2exe_empty = '0')    or
                                    (radr1_sd = BP_DEST_RE and MULT_INST_RE = '1' and BP_EXE2MEM_EMPTY_SE = '0')        or 
                                    (radr2_sd = BP_DEST_RE and MULT_INST_RE = '1' and BP_EXE2MEM_EMPTY_SE = '0')        or
                                    (radr1_sd = BP_DEST_RM and MULT_INST_RM = '1' and BP_MEM2WBK_EMPTY_SM = '0')        or
                                    (radr2_sd = BP_DEST_RM and MULT_INST_RM = '1' and BP_MEM2WBK_EMPTY_SM = '0')   
                                ) else 
                        '0'; 
-- Conditions
bpc_instr_in_exe1   <= '1' when radr1_sd = dec_fifo_rdest and dec2exe_empty = '0' and radr1_sd /= "000000" else '0'; 
bpc_load_in_mem1    <= '1' when radr1_sd = BP_DEST_RE and BP_MEM_LOAD_RE = '1' and radr1_sd /= "000000" and BP_EXE2MEM_EMPTY_SE = '0' else '0';  
bpc_ed1             <= '1' when radr1_sd = BP_DEST_RE and BP_EXE2MEM_EMPTY_SE = '0' and radr1_sd /= "000000" else '0'; 
bpc_md1             <= '1' when radr1_sd = BP_DEST_RM and BP_MEM2WBK_EMPTY_SM = '0' and radr1_sd /= "000000" else '0';           

bpc_instr_in_exe2   <= '1' when radr2_sd = dec_fifo_rdest and dec2exe_empty = '0' and radr2_sd /= "000000" else '0'; 
bpc_load_in_mem2    <= '1' when radr2_sd = BP_DEST_RE and BP_MEM_LOAD_RE = '1' and radr2_sd /= "000000" and BP_EXE2MEM_EMPTY_SE = '0' else '0';  
bpc_ed2             <= '1' when radr2_sd = BP_DEST_RE and BP_EXE2MEM_EMPTY_SE = '0' and radr2_sd /= "000000" else '0'; 
bpc_md2             <= '1' when radr2_sd = BP_DEST_RM and BP_MEM2WBK_EMPTY_SM = '0' and radr2_sd /= "000000" else '0';           


bpc_mult_exe1       <= '1' when radr1_sd = BP_DEST_RE and MULT_INST_RE = '1' and BP_EXE2MEM_EMPTY_SE = '0' else '0';
bpc_mult_exe2       <= '1' when radr2_sd = BP_DEST_RE and MULT_INST_RE = '1' and BP_EXE2MEM_EMPTY_SE = '0' else '0';

bpc_mult_mem1       <= '1' when radr1_sd = BP_DEST_RM and MULT_INST_RM = '1' and BP_MEM2WBK_EMPTY_SM = '0' else '0'; 
bpc_mult_mem2       <= '1' when radr2_sd = BP_DEST_RM and MULT_INST_RM = '1' and BP_MEM2WBK_EMPTY_SM = '0' else '0';

-- Affectations
rdata1_sd   <=  BP_EXE_RES_RE   when bpc_ed1 = '1' and CSR_WENABLE_RE = '0' else
                CSR_RDATA_RE    when bpc_ed1 = '1' and CSR_WENABLE_RE = '1' else 
                BP_MEM_RES_RM   when bpc_md1 = '1' and CSR_WENABLE_RM = '0' else 
                CSR_RDATA_RM    when bpc_md1 = '1' and CSR_WENABLE_RM = '1' else 
                RDATA1_SR; 

r1_valid_sd <=  not(bpc_instr_in_exe1 or bpc_load_in_mem1 or bpc_mult_exe1 or bpc_mult_mem1);

rdata2_sd   <=  BP_EXE_RES_RE   when bpc_ed2 = '1' and CSR_WENABLE_RE = '0' else 
                CSR_RDATA_RE    when bpc_ed2 = '1' and CSR_WENABLE_RE = '1' else 
                BP_MEM_RES_RM   when bpc_md2 = '1' and CSR_WENABLE_RM = '0' else 
                CSR_RDATA_RM    when bpc_md2 = '1' and CSR_WENABLE_RM = '1' else 
                RDATA2_SR; 

r2_valid_sd <=  not(bpc_instr_in_exe2 or bpc_load_in_mem2 or bpc_mult_exe2 or bpc_mult_mem2);


exception_sd <= (illegal_inst_sd or instruction_adress_misaligned_sd or env_call_u_mode_sd or
env_call_m_mode_sd or env_call_s_mode_sd or env_call_wrong_mode or mret_i_sd or
instruction_access_fault_sd or ebreak_i_sd);

-------------------------
-- Ouput
-------------------------
-- affectation 
DEC2EXE_EMPTY_SD <= dec2exe_empty; 
REG_ADR1_SD <= radr1_sd;
REG_ADR2_SD <= radr2_sd;
MEM_LOAD_RD <= mem_load_fifo; 
DEST_RD <= dec_fifo_rdest;
CSR_WENABLE_RD <= csr_wenable_fifo;
CSR_RADR_SD <= csr_radr;
MULT_INST_RD <= dec_fifo_mult_inst; 

-------------------------
-- fifo  
-------------------------

-- dec2if 
dec2if_din <= new_pc;  

PC_RD <= dec2if_dout; 

-- dec2exe 
dec2exe_data(251) <= mul_i_sd or mulh_i_sd or mulhsu_i_sd or mulhu_i_sd;  
dec2exe_data(250 downto 248)    <=  select_operation_sd(3 downto 1);
dec2exe_data(247 downto 216)    <=  pc_branch_value_sd;
dec2exe_data(215) <= ebreak_i_sd; 
dec2exe_data(214) <= instruction_access_fault_sd; 
dec2exe_data(213) <= mret_i_sd; 
dec2exe_data(212) <= exception_sd; 
dec2exe_data(211) <= env_call_wrong_mode;
dec2exe_data(210) <= env_call_u_mode_sd;
dec2exe_data(209) <= illegal_inst_sd;
dec2exe_data(208) <= instruction_adress_misaligned_sd;
dec2exe_data(207) <= env_call_m_mode_sd;
dec2exe_data(206) <= env_call_s_mode_sd;
dec2exe_data(205 downto 174) <= CSR_RDATA_SC; 
dec2exe_data(173) <= csr_wenable_sd;
dec2exe_data(172 downto 161) <= csr_radr;
dec2exe_data(160 downto 129) <= PC_IF2DEC_RI; 
dec2exe_data(128) <= block_bp_sd; 
dec2exe_data(127) <= r1_valid_sd; 
dec2exe_data(126) <= r2_valid_sd;
dec2exe_data(125 downto 120) <= radr1_sd; 
dec2exe_data(119 downto 114) <= radr2_sd;
dec2exe_data(113 downto 112) <= exe_cmd_sd;
dec2exe_data(111 downto 80) <= dec2exe_op1_sd;
dec2exe_data(79 downto 48) <= dec2exe_op2_sd;
dec2exe_data(47) <= neg_op2_sd; 
dec2exe_data(46) <= wb_sd; 
dec2exe_data(45 downto 14) <= mem_data_sd;
dec2exe_data(13) <= mem_load_sd;
dec2exe_data(12) <= mem_store_sd; 
dec2exe_data(11) <= mem_sign_extend_sd; 
dec2exe_data(10 downto 9) <= mem_size_sd; 
dec2exe_data(8) <= select_operation_sd(0); 
dec2exe_data(7 downto 2) <= rdest_sd; 
dec2exe_data(1) <= (slt_i_sd or slti_i_sd);
dec2exe_data(0) <= (sltu_i_sd or sltiu_i_sd);

dec2exe_x(251 downto 161)   <= (others => '0');
dec2exe_x(160 downto 129)   <= PC_IF2DEC_RI;
dec2exe_x(128 downto 0)     <= (others => '0');

dec2exe_din <=  dec2exe_x when EXCEPTION_SM = '1' else 
                dec2exe_data;

dec_fifo_mult_inst  <= dec2exe_dout(251);
SELECT_OPERATION_RD(3 downto 1) <= dec2exe_dout(250 downto 248);
PC_BRANCH_VALUE_RD <= dec2exe_dout(247 downto 216); 
EBREAK_RD <= dec2exe_dout(215);
INSTRUCTION_ACCESS_FAULT_RD <= dec2exe_dout(214);
MRET_RD <= dec2exe_dout(213);
EXCEPTION_RD <= dec2exe_dout(212);
ENV_CALL_WRONG_MODE_RD <= dec2exe_dout(211);
ENV_CALL_U_MODE_RD <= dec2exe_dout(210);
ILLEGAL_INSTRUCTION_RD <= dec2exe_dout(209);
ADRESS_MISALIGNED_RD <= dec2exe_dout(208);
ENV_CALL_M_MODE_RD <= dec2exe_dout(207);
ENV_CALL_S_MODE_RD <= dec2exe_dout(206);                
CSR_RDATA_RD <= dec2exe_dout(205 downto 174);
csr_wenable_fifo <= dec2exe_dout(173);
CSR_WADR_RD <= dec2exe_dout(172 downto 161);
PC_DEC2EXE_RD <= dec2exe_dout(160 downto 129);
BLOCK_BP_RD <= dec2exe_dout(128);
BP_R1_VALID_RD <= dec2exe_dout(127);
BP_R2_VALID_RD <= dec2exe_dout(126);
BP_RADR1_RD <= dec2exe_dout(125 downto 120);
BP_RADR2_RD <= dec2exe_dout(119 downto 114);
CMD_RD <= dec2exe_dout(113 downto 112);
OP1_RD <= dec2exe_dout(111 downto 80);
OP2_RD <= dec2exe_dout(79 downto 48);
NEG_OP2_RD <= dec2exe_dout(47);
WB_RD <= dec2exe_dout(46);
MEM_DATA_RD <= dec2exe_dout(45 downto 14);
mem_load_fifo <= dec2exe_dout(13);
MEM_STORE_RD <= dec2exe_dout(12);
MEM_SIGN_EXTEND_RD <= dec2exe_dout(11);
MEM_SIZE_RD <= dec2exe_dout(10 downto 9);
SELECT_OPERATION_RD(0) <= dec2exe_dout(8);
dec_fifo_rdest <= dec2exe_dout(7 downto 2);
SLT_RD <= dec2exe_dout(1);
SLTU_RD <= dec2exe_dout(0);

end archi;