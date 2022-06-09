library ieee; 
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dec is 
    port(
        -- global interface
        clk, reset_n : in std_logic;

        -- Reg interface
        RDATA1_SR, RDATA2_SR : in std_logic_vector(31 downto 0);
        ADR_DEST_SR : out std_logic_vector(5 downto 0);
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
        BP_EXE2MEM_EMPTY : in std_logic;
        BP_DEST_RM : in std_logic_vector(5 downto 0);
        BP_MEM_RES_RM : in std_logic_vector(31 downto 0);
        BP_R1_VALID_RD, BP_R2_VALID_RD : out std_logic;
        BP_RADR1_RD, BP_RADR2_RD : out std_logic_vector(5 downto 0)    );
end dec;

architecture archi of dec is 

constant inc_value : std_logic_vector(31 downto 0) := x"00000004";

-- fifo 
signal dec2if_din, dec2if_dout : std_logic_vector(31 downto 0);
signal dec2if_full_sd, dec2if_push_sd : std_logic;

signal dec2exe_din, dec2exe_dout : std_logic_vector(127 downto 0);
signal dec2exe_full_sd, dec2exe_push_sd : std_logic;

-- Instructions
signal r_type_sd, i_type_sd, s_type_sd, b_type_sd, u_type_sd, j_type_sd, jalr_type_sd : std_logic;
signal load_type_sd : std_logic;

signal add_i_sd, sub_i_sd, slt_i_sd, sltu_i_sd, and_i_sd, or_i_sd, xor_i_sd, sll_i_sd, srl_i_sd, sra_i_sd : std_logic;
signal addi_i_sd, slti_i_sd, sltiu_i_sd, andi_i_sd, ori_i_sd, xori_i_sd, slli_i_sd, srli_i_sd, srai_i_sd : std_logic;

signal lw_i_sd, lh_i_sd, lhu_i_sd, lb_i_sd, lbu_i_sd, sw_i_sd, sh_i_sd, sb_i_sd : std_logic; 

signal beq_i_sd, bne_i_sd, blt_i_sd, bge_i_sd, bltu_i_sd, bgeu_i_sd : std_logic;

signal lui_i_sd : std_logic;
signal auipc_i_sd : std_logic;

signal j_i_sd, jalr_i_sd : std_logic;

-- dec2exe data and commands
signal dec2exe_op1_sd, dec2exe_op2_sd, op1_u_type_sd, op2_i_type_sd : std_logic_vector(31 downto 0); 
signal radr1_sd, radr2_sd, rdest_sd : std_logic_vector(5 downto 0);
signal neg_op2_sd : std_logic;
signal alu_cmd_sd : std_logic_vector(1 downto 0);
signal select_shift_sd : std_logic;
signal mem_data_sd : std_logic_vector(31 downto 0);
signal mem_load_sd,mem_store_sd, mem_sign_extend_sd : std_logic;
signal mem_size_sd : std_logic_vector(1 downto 0);
signal wb_sd : std_logic;

-- branch and pc gestion
signal offset_branch_sd : std_logic_vector(31 downto 0);
signal inval_adr_dest, invalid_instr, invalid_i, inc_pc, inc_pc_sd, inc_pc_b_type : std_logic;
signal res : std_logic_vector(31 downto 0);
signal res_compare : std_logic_vector(32 downto 0);
signal add_offset_to_pc_sd : std_logic;
signal pc : std_logic_vector(31 downto 0);

-- bypass
signal stall, block_in_dec : std_logic;
signal r1_valid_sd, r2_valid_sd : std_logic;

-- readable signals (output ports)
signal mem_load_fifo : std_logic;
signal dec2exe_empty : std_logic;

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
    generic map(N => 128)
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
-- dec2if 
dec2if_push_sd <= not dec2if_full_sd;

-- if2dec 
IF2DEC_POP_SD <= '1' when (add_offset_to_pc_sd = '1' or (stall = '0' and IF2DEC_EMPTY_SI = '0' and dec2exe_full_sd = '0')) else 
                 '0'; 
IF2DEC_FLUSH_SD <= '1' when (add_offset_to_pc_sd = '1') else 
                   '0'; 

-- dec2exe
dec2exe_push_sd <= '0' when (stall = '1' or dec2exe_full_sd = '1' or IF2DEC_EMPTY_SI = '1') else
                '1'; 

-------------------------
-- Instruction type 
-------------------------
r_type_sd <= '1' when INSTR_RI(6 downto 0) = "0110011" else '0';
i_type_sd <= '1' when (INSTR_RI(6 downto 0) = "0010011" or INSTR_RI(6 downto 0) = "0000011") else '0';
s_type_sd <= '1' when INSTR_RI(6 downto 0) = "0100011" else '0';
b_type_sd <= '1' when INSTR_RI(6 downto 0) = "1100011" else '0';
u_type_sd <= '1' when INSTR_RI(6 downto 0) = "0110111" else '0';
j_type_sd <= '1' when INSTR_RI(6 downto 0) = "1101111" else '0';
jalr_type_sd  <= '1' when INSTR_RI(6 downto 0) = "1100111" else '0';
load_type_sd  <= '1' when INSTR_RI(6 downto 0) = "0000011" else '0';

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
addi_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "000" else '0';
slti_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "010" else '0';
sltiu_i_sd <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "011" else '0';
andi_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "111" else '0';
ori_i_sd   <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "110" else '0';
xori_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "100" else '0';
slli_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "001" else '0';
srli_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "101" and INSTR_RI(30) = '0' else '0';
srai_i_sd  <= '1' when i_type_sd = '1' and INSTR_RI(14 downto 12) = "101" and INSTR_RI(30) = '1' else '0';

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

rdest_sd <= '0'&INSTR_RI(11 downto 7) when ((r_type_sd or i_type_sd or s_type_sd or u_type_sd or j_type_sd or jalr_type_sd) = '1') else
            "000000";

-- Operand 1 selection
op1_u_type_sd(31 downto 12) <= INSTR_RI(31 downto 12);
op1_u_type_sd(11 downto 0)  <= x"000";

dec2exe_op1_sd <= RDATA1_SR when ((r_type_sd or i_type_sd or s_type_sd or b_type_sd) = '1') else 
               op1_u_type_sd when u_type_sd = '1' else 
               READ_PC_SR when ((j_type_sd or jalr_type_sd) = '1') else 
               x"00000000";

-- Operand 2 selection
op2_i_type_sd(31 downto 12) <= x"FFFFF" when RDATA2_SR(31) = '1' else 
                            x"00000";
op2_i_type_sd(11 downto 0)  <= INSTR_RI(31 downto 20);

dec2exe_op2_sd <= RDATA2_SR when ((r_type_sd or s_type_sd or b_type_sd or (u_type_sd and not(auipc_i_sd))) = '1') else 
               op2_i_type_sd when i_type_sd = '1' else
               PC_IF2DEC_RI when auipc_i_sd = '1' else 
               READ_PC_SR when ((j_type_sd or jalr_type_sd) = '1') else 
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

select_shift_sd <= sll_i_sd or slli_i_sd or srl_i_sd or sra_i_sd or srai_i_sd;


-- ?????????? and load_type ?
wb_sd <=  r_type_sd or i_type_sd or u_type_sd or b_type_sd or j_type_sd or jalr_type_sd;

mem_data_sd <= RDATA2_SR when s_type_sd = '1' else 
               x"00000000";
mem_load_sd <= lw_i_sd or lh_i_sd or lhu_i_sd or lb_i_sd or lbu_i_sd;

mem_store_sd <= sw_i_sd or sh_i_sd or sb_i_sd;

mem_size_sd <=  "00" when lw_i_sd = '1' else 
                "01" when ((lh_i_sd or lhu_i_sd or sh_i_sd) = '1') else 
                "10" when ((lb_i_sd or lbu_i_sd or sb_i_sd) = '1') else 
                "11";
 
mem_sign_extend_sd <= '1' when (((lh_i_sd and lhu_i_sd )= '1') or ((lb_i_sd and lbu_i_sd) = '1')) else
                      '0';

-------------------------
-- Branch and PC
-------------------------
offset_branch_sd(31 downto 13) <=   (others => INSTR_RI(31)) when b_type_sd = '1' else 
                                    (others => '0');

offset_branch_sd(12) <= INSTR_RI(31) when b_type_sd = '1' else '0';
offset_branch_sd(11) <= INSTR_RI(7) when b_type_sd = '1' else '0';
offset_branch_sd(10 downto 5) <= INSTR_RI(30 downto 25) when b_type_sd = '1' else "000000";
offset_branch_sd(4 downto 1) <= INSTR_RI(11 downto 8) when b_type_sd = '1' else "0000";
offset_branch_sd(0) <= '0'; 

res <= dec2exe_op1_sd xor dec2exe_op2_sd; 
res_compare <= '0'&(std_logic_vector(signed(dec2exe_op1_sd) - signed(dec2exe_op2_sd)));

-------- why res => 33 bits ??
inc_pc_b_type <= '1' when b_type_sd = '1' and ((bne_i_sd = '1' and (res = x"00000000")) 
                                              or (beq_i_sd = '1' and (res /= x"00000000"))
                                              or (blt_i_sd = '1' and res_compare(32 downto 31) = "00")
                                              or (bltu_i_sd = '1' and res_compare(32 downto 31) /= "00")
                                              or (bge_i_sd = '1' and res_compare(32 downto 31) /= "00")
                                              or (bgeu_i_sd = '1' and res_compare(32 downto 31) = "00"))
                     else 
                 '0';
                
inc_pc <= '1' when ((r_type_sd or i_type_sd or s_type_sd) = '1') else 
             inc_pc_b_type when b_type_sd = '1' else 
             '0';

inval_adr_dest <= '1' when ((r_type_sd or i_type_sd or u_type_sd or j_type_sd or jalr_type_sd) = '1') else '0';

invalid_i <= '0'; -- idk the need of this signal 

invalid_instr <= invalid_i or IF2DEC_EMPTY_SI; 

inc_pc_sd <= (inc_pc and dec2if_push_sd) or not invalid_instr;

add_offset_to_pc_sd <= not stall and not inc_pc and dec2if_push_sd and not invalid_instr;

-- PC 
process(READ_PC_SR, add_offset_to_pc_sd, inc_pc_sd)
begin 
    if inc_pc_sd = '1' then 
        pc <= std_logic_vector(signed(READ_PC_SR) + signed(inc_value)); 
        WRITE_PC_ENABLE_SD <= '1'; 
    elsif inc_pc_sd = '0' and add_offset_to_pc_sd = '1' then 
        pc <= std_logic_vector(signed(READ_PC_SR) - signed(inc_value));
        WRITE_PC_ENABLE_SD <= '1'; 
    else 
    pc <= READ_PC_SR;
    WRITE_PC_ENABLE_SD <= '0';
    end if; 
    -- dec2if_din <= pc; 
end process; 

WRITE_PC_SD <= pc;

-- Bypass...
block_in_dec <= '1' when (((radr1_sd = rdest_sd) or(radr2_sd = rdest_sd)) and mem_load_fifo = '1' and dec2exe_empty = '0') else '0';
stall <= (not r1_valid_sd or not r2_valid_sd) and (b_type_sd or j_type_sd or jalr_type_sd or block_in_dec);  
             
-- ... not really implemented yet
r1_valid_sd <= '1'; 
r2_valid_sd <= '1';

-------------------------
-- Ouput
-------------------------
-- affectation 
DEC2EXE_EMPTY_SD <= dec2exe_empty; 
RADR1_SR <= radr1_sd;
RADR2_SR <= radr2_sd;
DEST_RD <= rdest_sd;
MEM_LOAD_RD <= mem_load_fifo; 

-- fifo  
-- dec2if 
dec2if_din <= pc; 
PC_RD <= dec2if_dout; 

-- dec2exe 
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
MEM_LOAD_RD <= dec2exe_dout(13);
MEM_STORE_RD <= dec2exe_dout(12);
MEM_SIGN_EXTEND_RD <= dec2exe_dout(11);
MEM_SIZE_RD <= dec2exe_dout(10 downto 9);
SELECT_SHIFT_RD <= dec2exe_dout(8);
DEST_RD <= dec2exe_dout(7 downto 2);
SLT_RD <= dec2exe_dout(1);
SLTU_RD <= dec2exe_dout(0);

end archi;