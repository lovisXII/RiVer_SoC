


// instruction type constants

const int opcode_R_type = 0b0110011;
const int opcode_I_type = 0b0010011;
const int opcode_I_type_load = 0b0000011;
const int opcode_I_type_jalr = 0b1100111;
const int opcode_S_type = 0b0100011; 
const int opcode_B_type = 0b1100011;
const int opcode_U_type = 0b0110111;
const int opcode_U_type_auipc = 0b0010111;
const int opcode_J_type = 0b1101111;

// funct3

const int func3_add_i = 0b000;
const int func3_sub_i = 0b000;

const int func3_sll_i = 0b001;

const int func3_slt_i  = 0b010;
const int func3_sltu_i = 0b011;

const int func3_xor_i = 0b100;

const int func3_srl_i = 0b101;
const int func3_sra_i = 0b101;

const int func3_or_i  = 0b110;

const int func3_and_i = 0b111;

const int func3_lrd_i = 0b011;
const int func3_scd_i = 0b011;
