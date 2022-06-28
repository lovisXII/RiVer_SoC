library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dec is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Reg interface
        RDATA1_SR, RDATA2_SR : in std_logic_vector(31 downto 0);
        RADR1_SR, RADR2_SR : out std_logic_vector(5 downto 0);
        WRITE_PC_SD : out std_logic_vector(31 downto 0);
        WRITE_PC_ENABLE_SD : out std_logic;
        READ_PC_SR : in std_logic_vector(31 downto 0);

        -- Exe interface 
        OP1_RD, OP2_RD : out std_logic_vector(31 downto 0);
        CMD_RD : out std_logic_vector(1 downto 0);
        NEG_OP2_RD : out std_logic; 
        WB_RD : out std_logic;
        DEST_RD : out std_logic_vector(5 downto 0);
        SELECT_SHIFT_RD : out std_logic;
        SLT_RD, SLTU_RD : out std_logic;    
        MEM_DATA_RD : out std_logic_vector(31 downto 0);
        MEM_LOAD_RD , MEM_STORE_RD, MEM_SIGN_EXTEND_RD : out std_logic;
        MEM_SIZE_RD : out std_logic_vector(1 downto 0);

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

        -- Bypasses
        BP_DEST_RE : in std_logic_vector(5 downto 0);
        BP_EXE_RES_RE : in std_logic_vector(31 downto 0);
        BP_MEM_LOAD_RE : in std_logic;
        BP_EXE2MEM_EMPTY_SE, BP_MEM2WBK_EMPTY_SM : in std_logic;
        BP_DEST_RM : in std_logic_vector(5 downto 0);
        BP_MEM_RES_RM : in std_logic_vector(31 downto 0);
        BP_R1_VALID_RD, BP_R2_VALID_RD : out std_logic;
        BP_RADR1_RD, BP_RADR2_RD : out std_logic_vector(5 downto 0);    
        BLOCK_BP_RD : out std_logic
    );
end dec;

architecture archi of dec is 

constant inc_value : std_logic_vector(31 downto 0) := x"00000004";

signal reset_sync_sd : std_logic := '0';
signal resetting_sd : std_logic := '0' ; 
-- fifo 
signal dec2if_din, dec2if_dout : std_logic_vector(31 downto 0);
signal dec2if_full_sd, dec2if_push_sd : std_logic;

signal dec2exe_din, dec2exe_dout : std_logic_vector(128 downto 0);
signal dec2exe_full_sd, dec2exe_push_sd : std_logic;

-- Instructions
signal r_type_sd, i_type_sd, s_type_sd, b_type_sd, u_type_sd, j_type_sd, jalr_type_sd, jal_type_sd : std_logic;
signal load_type_sd, nmem_type_sd : std_logic;

signal add_i_sd, sub_i_sd, slt_i_sd, sltu_i_sd, and_i_sd, or_i_sd, xor_i_sd, sll_i_sd, srl_i_sd, sra_i_sd : std_logic;
signal addi_i_sd, slti_i_sd, sltiu_i_sd, andi_i_sd, ori_i_sd, xori_i_sd, slli_i_sd, srli_i_sd, srai_i_sd : std_logic;

signal lw_i_sd, lh_i_sd, lhu_i_sd, lb_i_sd, lbu_i_sd, sw_i_sd, sh_i_sd, sb_i_sd : std_logic; 

signal beq_i_sd, bne_i_sd, blt_i_sd, bge_i_sd, bltu_i_sd, bgeu_i_sd : std_logic;

signal lui_i_sd : std_logic;
signal auipc_i_sd : std_logic;

signal j_i_sd, jalr_i_sd : std_logic;

-- dec2exe data and commands
signal dec2exe_op1_sd, dec2exe_op2_sd, op1_u_type_sd, op2_i_type_sd, op2_s_type_sd : std_logic_vector(31 downto 0); 
signal radr1_sd, radr2_sd, rdest_sd : std_logic_vector(5 downto 0);
signal rdata1_sd, rdata2_sd : std_logic_vector(31 downto 0);
signal neg_op2_sd : std_logic;
signal alu_cmd_sd : std_logic_vector(1 downto 0);
signal select_shift_sd : std_logic;
signal mem_data_sd : std_logic_vector(31 downto 0);
signal mem_load_sd,mem_store_sd, mem_sign_extend_sd : std_logic;
signal mem_size_sd : std_logic_vector(1 downto 0);
signal wb_sd : std_logic;

-- branch and pc gestion
signal offset_branch_sd, offset_branch_j, offset_branch_jalr, jalr_offset, offset_branch_b : std_logic_vector(31 downto 0);
signal jalr_offset_calc : std_logic_vector(31 downto 0);
signal inval_adr_dest, invalid_instr, invalid_i, jump_sd : std_logic;
signal different_sign : std_logic;
signal res : std_logic_vector(31 downto 0);
signal res_compare : std_logic_vector(31 downto 0);
signal add_offset_to_pc : std_logic;
signal pc : std_logic_vector(31 downto 0) := x"00000000";
signal init_pc : std_logic_vector(31 downto 0); 
-- bypass
signal stall_sd, block_in_dec : std_logic;
signal r1_valid_sd, r2_valid_sd : std_logic;
signal block_bp_sd : std_logic;

-- ByPass Conditions 
signal bpc_instr_in_exe1, bpc_load_in_mem1, bpc_ed1, bpc_md1 : std_logic;  
signal bpc_instr_in_exe2, bpc_load_in_mem2, bpc_ed2, bpc_md2 : std_logic;  

-- readable signals (output ports)
signal mem_load_fifo : std_logic;
signal dec2exe_empty : std_logic;
signal dec2exe_rdest_fifo : std_logic_vector(5 downto 0);

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
    generic map(N => 129)
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
stall_sd    <=  '1' when    (
                                (
                                    (r1_valid_sd = '0' or r2_valid_sd = '0') 
                                    and 
                                    (b_type_sd = '1' or jalr_type_sd = '1' or j_type_sd = '1' or block_in_dec = '1')
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
r_type_sd <= '1' when INSTR_RI(6 downto 0) = "0110011" else '0';
i_type_sd <= '1' when (INSTR_RI(6 downto 0) = "0010011" or INSTR_RI(6 downto 0) = "0000011") else '0';
s_type_sd <= '1' when INSTR_RI(6 downto 0) = "0100011" else '0';
b_type_sd <= '1' when INSTR_RI(6 downto 0) = "1100011" else '0';
u_type_sd <= '1' when INSTR_RI(6 downto 0) = "0110111" or INSTR_RI(6 downto 0) = "0010111" else '0';
j_type_sd <= '1' when INSTR_RI(6 downto 0) = "1101111" else '0';
jalr_type_sd    <= '1' when INSTR_RI(6 downto 0) = "1100111" else '0';
load_type_sd    <= '1' when INSTR_RI(6 downto 0) = "0000011" else '0';
nmem_type_sd    <= '1' when INSTR_RI(6 downto 0) = "0010011" else '0'; -- not mem type but i type
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

------------------------------------
-- Registers and operands selection
------------------------------------
-- Registers affectation 
radr1_sd <= '0'&INSTR_RI(19 downto 15) when ((r_type_sd or i_type_sd or s_type_sd or b_type_sd or jalr_type_sd) ='1') else
            "000000";

radr2_sd <= '0'&INSTR_RI(24 downto 20) when ((r_type_sd or s_type_sd or b_type_sd) = '1') else
            "000000";

rdest_sd <= '0'&INSTR_RI(11 downto 7) when ((r_type_sd or i_type_sd or u_type_sd or j_type_sd or jalr_type_sd) = '1') else
            "000000";

-- Operand 1 selection
op1_u_type_sd(31 downto 12) <= INSTR_RI(31 downto 12);
op1_u_type_sd(11 downto 0)  <= x"000";

dec2exe_op1_sd <= rdata1_sd when ((r_type_sd or i_type_sd or s_type_sd or b_type_sd) = '1') else 
               op1_u_type_sd when u_type_sd = '1' else 
               READ_PC_SR when ((j_type_sd or jalr_type_sd) = '1') else 
               x"00000000";

-- Operand 2 selection
op2_i_type_sd(31 downto 12) <=  (others => INSTR_RI(31));                            
op2_i_type_sd(11 downto 0)  <= INSTR_RI(31 downto 20);

op2_s_type_sd(31 downto 12) <=  (others => INSTR_RI(31));
op2_s_type_sd(11 downto 5)  <= INSTR_RI(31 downto 25);
op2_s_type_sd(4 downto 0)   <= INSTR_RI(11 downto 7);

dec2exe_op2_sd <= rdata2_sd when ((r_type_sd  or b_type_sd or (u_type_sd and not(auipc_i_sd))) = '1') else 
               op2_i_type_sd when i_type_sd = '1' else
               op2_s_type_sd when s_type_sd = '1' else
               PC_IF2DEC_RI when auipc_i_sd = '1' else 
               x"00000000";

-------------------------
-- Exec commands  
-------------------------
-- neg
neg_op2_sd <= sub_i_sd or slt_i_sd or slti_i_sd or sltu_i_sd or sltiu_i_sd; 
-- alu 
alu_cmd_sd <=   "01" when ((and_i_sd or andi_i_sd or srl_i_sd or srli_i_sd) = '1') else 
                "10" when ((or_i_sd or ori_i_sd or sra_i_sd or srai_i_sd) = '1') else
                "11" when ((xor_i_sd or xori_i_sd) = '1') else 
                "00";

select_shift_sd <= sll_i_sd or slli_i_sd or srl_i_sd or srli_i_sd or sra_i_sd or srai_i_sd;


wb_sd <=  r_type_sd or i_type_sd or u_type_sd or b_type_sd or j_type_sd or jalr_type_sd;

mem_data_sd <= rdata2_sd when s_type_sd = '1' else 
               x"00000000";
mem_load_sd <= lw_i_sd or lh_i_sd or lhu_i_sd or lb_i_sd or lbu_i_sd;

mem_store_sd <= sw_i_sd or sh_i_sd or sb_i_sd;

mem_size_sd <=  "00" when ((lw_i_sd or sw_i_sd)= '1') else              -- word size 
                "01" when ((lh_i_sd or lhu_i_sd or sh_i_sd) = '1') else -- halfword size
                "10" when ((lb_i_sd or lbu_i_sd or sb_i_sd) = '1') else -- byte size
                "11";                                                   -- not a mem access
 
mem_sign_extend_sd <= '1' when (((lh_i_sd and lhu_i_sd )= '1') or ((lb_i_sd and lbu_i_sd) = '1')) else
                      '0';

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
                
inval_adr_dest <= '1' when ((r_type_sd or i_type_sd or u_type_sd or j_type_sd or jalr_type_sd) = '1') else '0';

invalid_i <= '0'; -- idk the need of this signal 

invalid_instr <= invalid_i or IF2DEC_EMPTY_SI; 

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

WRITE_PC_SD <= pc; 
-------------------------
-- Bypass
-------------------------
block_in_dec <= '1' when (((radr1_sd = dec2exe_rdest_fifo) or (radr2_sd = dec2exe_rdest_fifo)) and mem_load_fifo = '1' and dec2exe_empty = '0') else '0';
block_bp_sd <= jalr_type_sd;      

 
-- Conditions
bpc_instr_in_exe1   <= '1' when radr1_sd = dec2exe_rdest_fifo and dec2exe_empty = '0' and radr1_sd /= "000000" else '0'; 
bpc_load_in_mem1    <= '1' when radr1_sd = BP_DEST_RE and BP_MEM_LOAD_RE = '1' and radr1_sd /= "000000" and BP_EXE2MEM_EMPTY_SE = '0' else '0';  
bpc_ed1             <= '1' when radr1_sd = BP_DEST_RE and BP_EXE2MEM_EMPTY_SE = '0' and radr1_sd /= "000000" else '0'; 
bpc_md1             <= '1' when radr1_sd = BP_DEST_RM and radr1_sd /= "000000" else '0';           

bpc_instr_in_exe2   <= '1' when radr2_sd = dec2exe_rdest_fifo and dec2exe_empty = '0' and radr2_sd /= "000000" else '0'; 
bpc_load_in_mem2    <= '1' when radr2_sd = BP_DEST_RE and BP_MEM_LOAD_RE = '1' and radr2_sd /= "000000" and BP_EXE2MEM_EMPTY_SE = '0' else '0';  
bpc_ed2             <= '1' when radr2_sd = BP_DEST_RE and BP_EXE2MEM_EMPTY_SE = '0' and radr2_sd /= "000000" else '0'; 
bpc_md2             <= '1' when radr2_sd = BP_DEST_RM and radr2_sd /= "000000" else '0';           

-- Affectations
rdata1_sd   <=  BP_EXE_RES_RE when bpc_ed1 = '1' else 
                BP_MEM_RES_RM when bpc_md1 = '1' else 
                RDATA1_SR; 

r1_valid_sd <=  not(bpc_instr_in_exe1 or bpc_load_in_mem1);

rdata2_sd   <=  BP_EXE_RES_RE when bpc_ed2 = '1' else 
                BP_MEM_RES_RM when bpc_md2 = '1' else 
                RDATA2_SR; 

r2_valid_sd <=  not(bpc_instr_in_exe2 or bpc_load_in_mem2);

-------------------------
-- Ouput
-------------------------
-- affectation 
DEC2EXE_EMPTY_SD <= dec2exe_empty; 
RADR1_SR <= radr1_sd;
RADR2_SR <= radr2_sd;
MEM_LOAD_RD <= mem_load_fifo; 
DEST_RD <= dec2exe_rdest_fifo;

-- fifo  
-- dec2if 
dec2if_din <= pc;   
PC_RD <= dec2if_dout; 

-- dec2exe 
dec2exe_din(128) <= block_bp_sd; 
dec2exe_din(127) <= r1_valid_sd; 
dec2exe_din(126) <= r2_valid_sd;
dec2exe_din(125 downto 120) <= radr1_sd; 
dec2exe_din(119 downto 114) <= radr2_sd;
dec2exe_din(113 downto 112) <= alu_cmd_sd;
dec2exe_din(111 downto 80) <= dec2exe_op1_sd;
dec2exe_din(79 downto 48) <= dec2exe_op2_sd;
dec2exe_din(47) <= neg_op2_sd; 
dec2exe_din(46) <= wb_sd; 
dec2exe_din(45 downto 14) <= mem_data_sd;
dec2exe_din(13) <= mem_load_sd;
dec2exe_din(12) <= mem_store_sd; 
dec2exe_din(11) <= mem_sign_extend_sd; 
dec2exe_din(10 downto 9) <= mem_size_sd; 
dec2exe_din(8) <= select_shift_sd; 
dec2exe_din(7 downto 2) <= rdest_sd; 
dec2exe_din(1) <= (slt_i_sd or slti_i_sd);
dec2exe_din(0) <= (sltu_i_sd or sltiu_i_sd);

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
SELECT_SHIFT_RD <= dec2exe_dout(8);
dec2exe_rdest_fifo <= dec2exe_dout(7 downto 2);
SLT_RD <= dec2exe_dout(1);
SLTU_RD <= dec2exe_dout(0);

end archi;